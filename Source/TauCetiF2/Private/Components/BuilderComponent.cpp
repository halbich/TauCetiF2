#include "TauCetiF2.h"
#include "BuilderComponent.h"

// Sets default values for this component's properties
UBuilderComponent::UBuilderComponent() : currentDefinitionForBlock(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UBuilderComponent::BeginPlay()
{
	Super::BeginPlay();

	currentBlockInfo = NewObject<UBlockInfo>(this);

	currentBlockInfo->UnderConstruction = true;

	auto owner = GetOwner();
	ensure(owner);

	character = Cast<ACharacter>(owner);
	ensure(character);

	inventory->OnCurrentSelectedIndexChanged.AddDynamic(this, &UBuilderComponent::InventorySelectedChangedEvent);
}

// Called every frame
void UBuilderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!currentBuildableBlockInfo || !currentBuildableBlockInfo->IsValidLowLevelFast())
		return;

	if (currentBuildableBlockInfo->Action == EBuildableObjectAction::None)
		return;

	if (!currentSpawnedObject)
	{
		setCurrentBuildingItem(currentBuildableBlockInfo);
		return;  // if spawn was successful, we have actual data (spawn point etc)
	}

	if (!selector->IsValidBuildingLocation)
	{
		toggleHiddenCurrentSpawned();
		return;
	}

	check(currentDefinitionForBlock);

	auto oldLocation = currentBlockInfo->Location;
	auto newLocation = BlockHelpers::GetSpawnPoint(selector->ImpactPointWithSnap, selector->ImpactNormal, currentDefinitionForBlock, currentBlockInfo);

	currentBlockInfo->Location = newLocation;
	auto spawnBlock = WorldHelpers::GetSpawnBox(currentDefinitionForBlock, currentBlockInfo);
	if (!worldController->IsValidSpawnPoint(spawnBlock))
	{
		TMap<FVector, float> tempSuccesses;

		const int32 plusMinus = 4;

		auto plusMinusX = selector->ImpactNormal.X == 0 ? plusMinus : 0;
		auto plusMinusY = selector->ImpactNormal.Y == 0 ? plusMinus : 0;
		auto plusMinusZ = selector->ImpactNormal.Z == 0 ? plusMinus : 0;

		for (int8 x = -1 * plusMinusX; x <= plusMinusX; x++)
		{
			for (int8 y = -1 * plusMinusY; y <= plusMinusY; y++)
			{
				for (int8 z = -1 * plusMinusZ; z <= plusMinusZ; z++)
				{
					if (x == 0 && y == 0 && z == 0)		// we don't want the same failed result
						continue;

					auto offsetVect = FVector(x, y, z);
					auto newTestLocation = newLocation + offsetVect;
					currentBlockInfo->Location = newTestLocation;
					auto testSpawnBlock = WorldHelpers::GetSpawnBox(currentDefinitionForBlock, currentBlockInfo);
					if (worldController->IsValidSpawnPoint(testSpawnBlock))
						tempSuccesses.Add(offsetVect, FVector::DistSquared(newTestLocation, newLocation));	// we do not need to do sqrt on results;
				}
			}
		}

		if (tempSuccesses.Num() == 0)
		{
			// we don't have any usable results
			currentBlockInfo->Location = newLocation;
			return;
		}

		// get the closest point to our target point
		tempSuccesses.ValueSort([](const float& l, const float& r) {
			return l < r;
		});

		// and select the first one, using it as valid spawn location
		for (auto keyValue : tempSuccesses)
		{
			auto targetOffset = keyValue.Key;
			currentBlockInfo->Location = newLocation + targetOffset;
			spawnBlock = WorldHelpers::GetSpawnBox(currentDefinitionForBlock, currentBlockInfo);
			break;
		}
	}

	auto box = spawnBlock->GetBox();

	FVector origin;
	FVector extent;
	character->GetActorBounds(true, origin, extent);
	auto charBox = FBox::BuildAABB(origin, extent);

	if (!(box.Overlap(charBox) == FBox(EForceInit::ForceInitToZero)))
	{
		toggleHiddenCurrentSpawned();
		return;
	}

	toggleHiddenCurrentSpawned(false);
	ForceRecomputePosition = false;
	currentSpawnedObject->SetActorTransform(BlockHelpers::GetSpawnTransform(currentDefinitionForBlock, currentBlockInfo));
}

void UBuilderComponent::setCurrentBuildingItem(UBuildableBlockInfo* blockInfo)
{
	if (currentSpawnedObject)
	{
		toggleHiddenCurrentSpawned();
		selector->traceIgnoreActor = nullptr;
		currentSpawnedObject = nullptr;
		currentDefinitionForBlock = nullptr;
	}

	currentBuildableBlockInfo = blockInfo;
	if (!blockInfo || !blockInfo->IsValidLowLevel())
	{
		selector->IsUsableAllowed = true;
		selector->SetOutlining(false, 0);
		return;
	}

	selector->IsUsableAllowed = currentBuildableBlockInfo->BlockDefinition->AllowUsable;
	selector->SetOutlining(currentBuildableBlockInfo->AllowOutlineOnSelected, currentBuildableBlockInfo->StencilOverride);

	if (currentBuildableBlockInfo->Action == EBuildableObjectAction::None)
		return;

	ensureHolder();

	currentBlockInfo->ID = currentBuildableBlockInfo->ID;
	if (!currentDefinitionForBlock || currentDefinitionForBlock->BlockID != currentBlockInfo->ID)
		currentDefinitionForBlock = blockHolder->GetDefinitionFor(currentBuildableBlockInfo->ID);

	if (!currentDefinitionForBlock)
		return;

	auto defRot = currentBuildableBlockInfo->BlockDefinition->DefaultBuildingRotation;
	auto curRot = character->GetActorTransform().GetRotation().Rotator().GridSnap(GameDefinitions::WorldGrid);

	currentBlockInfo->Rotation = (FQuat(defRot) * FQuat(curRot)).Rotator();
	currentBlockInfo->Scale = currentBuildableBlockInfo->Scale;
	currentBlockInfo->Location = BlockHelpers::GetSpawnPoint(selector->ImpactPointWithSnap, selector->ImpactNormal, currentDefinitionForBlock, currentBlockInfo);
	currentBlockInfo->AdditionalFlags = currentBuildableBlockInfo->AdditionalFlags;

	if (!worldController || !worldController->IsValidLowLevel())
		return;

	if (!worldController->IsValidSpawnPoint(WorldHelpers::GetSpawnBox(currentDefinitionForBlock, currentBlockInfo)))
		return;

	auto used = usedObjects.FindRef(currentBuildableBlockInfo);
	if (!used)
	{
		if (currentBuildableBlockInfo->IsA(UInventoryBuildableBlockInfo::StaticClass()))
		{
			auto temp = Cast<UInventoryBuildableBlockInfo>(currentBuildableBlockInfo);
			currentBlockInfo->ElectricityInfo = temp->ElectricityInfo;
			currentBlockInfo->OxygenInfo = temp->OxygenInfo;
		}
		TArray<FText> validationErrors;
		used = worldController->SpawnWorldObject(GetWorld(), currentBlockInfo, validationErrors, false);
		if (!used)
			return;

		used->SetActorEnableCollision(false);
		usedObjects.Add(currentBuildableBlockInfo, used);
		return;
	}

	// after really fast scroll we could have "lost" few preview blocks on a screen and they would be visible
	for (auto u : usedObjects)
	{
		u.Value->SetActorHiddenInGame(true);
		u.Value->SetActorTickEnabled(true);
	}

	used->SetActorHiddenInGame(false);
	used->SetActorTickEnabled(true);

	used->SetActorTransform(BlockHelpers::GetSpawnTransform(currentDefinitionForBlock, currentBlockInfo));

	currentSpawnedObject = used;
	selector->traceIgnoreActor = currentSpawnedObject;
}

void UBuilderComponent::SetWorldController(AWorldController* controller)
{
	check(controller != nullptr);

	worldController = controller;
}

void UBuilderComponent::RotatePitch(float Value)
{
	if (!currentDefinitionForBlock)
		return;

	if (!currentDefinitionForBlock->AllowPitch)
		return;

	AddRotation(Value * 90, 0, 0);
}
void UBuilderComponent::RotateRoll(float Value)
{
	if (!currentDefinitionForBlock)
		return;

	if (!currentDefinitionForBlock->AllowRoll)
		return;

	AddRotation(0, 0, Value * 90);
}
void UBuilderComponent::RotateYaw(float Value)
{
	if (!currentDefinitionForBlock || !currentBlockInfo)
		return;

	if (!currentDefinitionForBlock->AllowYaw)
		return;

	AddRotation(0, Value * 90, 0);
}

void UBuilderComponent::InventorySelectedChangedEvent(int32 NewIndex, UBuildableBlockInfo* BlockInfo)
{
	setCurrentBuildingItem(BlockInfo);
}

void UBuilderComponent::DoAction() {
	if (!currentBuildableBlockInfo && selector->SelectedBlock)
	{
		selector->TryUse(true);
		return;
	}

	if (!worldController || !currentBlockInfo || !currentBlockInfo->IsValidLowLevel() || !currentBuildableBlockInfo || !currentBuildableBlockInfo->IsValidLowLevel())
		return;

	switch (currentBuildableBlockInfo->Action)
	{
	case EBuildableObjectAction::None:
		if (!selector->SelectedBlock)
			return;

		selector->TryUse(true);

		return;
	case EBuildableObjectAction::DeleteObject:

		if (!selector->SelectedBlock)
			return;

		worldController->DestroyWorldObject(selector->SelectedBlock);

		return;
	case EBuildableObjectAction::ConstructObject:

		if (!currentSpawnedObject || !currentSpawnedObject->IsValidLowLevel())
			return;

		auto spawnBox = WorldHelpers::GetSpawnBox(currentDefinitionForBlock, currentBlockInfo);
		if (!worldController->IsValidSpawnPoint(spawnBox))
			return;

		auto box = spawnBox->GetBox();

		FVector origin;
		FVector extent;
		character->GetActorBounds(true, origin, extent);
		const FBox charBox = FBox::BuildAABB(origin, extent);

		if (!(box.Overlap(charBox) == FBox(EForceInit::ForceInitToZero)))
			return;

		auto spawnBlock = NewObject<UBlockInfo>(this, NAME_None, RF_NoFlags, currentBlockInfo);

		spawnBlock->UnderConstruction = false;

		if (!worldController->IsValidSpawnPoint(WorldHelpers::GetSpawnBox(currentDefinitionForBlock, spawnBlock)))
			return;

		UInventoryBuildableBlockInfo* invBuildable = nullptr;
		if (currentBuildableBlockInfo->IsA(UInventoryBuildableBlockInfo::StaticClass()))
		{
			invBuildable = Cast<UInventoryBuildableBlockInfo>(currentBuildableBlockInfo);
			spawnBlock->ElectricityInfo = invBuildable->ElectricityInfo;
			spawnBlock->OxygenInfo = invBuildable->OxygenInfo;
		}

		// check if we can really build this block
		if (!BuilderElectricityComponent || !BuilderElectricityComponent->IsValidLowLevel())
			BuilderElectricityComponent = Cast<UElectricityComponent>(character->GetComponentByClass(UElectricityComponent::StaticClass()));

		check(BuilderElectricityComponent);

		float actuallyObtainedEnergy = 0.0f;
		if (IsInCreative || BuilderElectricityComponent->ObtainAmount(currentBuildableBlockInfo->BuildingEnergyRequired, actuallyObtainedEnergy, true))
		{
			TArray<FText> validationErrors;
			float actuallyReturnedEnergy = 0.0f;
			auto builtBlock = worldController->SpawnWorldObject(GetWorld(), spawnBlock, validationErrors, true);
			if (!builtBlock)
				BuilderElectricityComponent->PutAmount(actuallyObtainedEnergy, actuallyReturnedEnergy);
			else
			{
				if (invBuildable)
				{
					builtBlock->PropagateInventoryBuildableTags(invBuildable->GetTagsFlatlined());
					inventory->ItemBuilt(invBuildable);
				}
			}
		}
	}
}