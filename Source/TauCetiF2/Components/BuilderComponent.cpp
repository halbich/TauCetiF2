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
}

// Called every frame
void UBuilderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!selector || !selector->IsValidLowLevelFast() || !currentBuildableBlockInfo || !currentBuildableBlockInfo->IsValidLowLevelFast())
		return;

	check(currentBuildableBlockInfo);

	if (currentBuildableBlockInfo->Action == EBuildableObjectAction::None)
		return;

	if (!currentSpawnedObject)
	{
		SetCurrentBuildingItem(currentBuildableBlockInfo);
		return;  // Spawning might failed, probably due to invalid location. If not, we want to update next time.
	}

	check(currentDefinitionForBlock);

	auto newLocation = BlockHelpers::GetSpawnPoint(selector->ImpactPointWithSnap, selector->ImpactNormal, currentDefinitionForBlock, currentBlockInfo);

	if (!ForceRecomputePosition && currentBlockInfo->Location == newLocation)
		return;

	currentBlockInfo->Location = newLocation;
	auto spawnBlock = BlockHelpers1::GetSpawnBox(currentDefinitionForBlock, currentBlockInfo);
	if (!worldController->IsValidSpawnPoint(spawnBlock))
	{
		toggleHiddenCurrentSpawned();
		return;
	}

	auto box = spawnBlock->GetBox();

	FVector origin;
	FVector extent;
	character->GetActorBounds(true, origin, extent);
	auto charBox = FBox::BuildAABB(origin, extent);

	if (!(box.Overlap(charBox) == 0))
	{
		toggleHiddenCurrentSpawned();
		return;
	}

	toggleHiddenCurrentSpawned(false);
	ForceRecomputePosition = false;
	currentSpawnedObject->SetActorTransform(BlockHelpers::GetSpawnTransform(currentDefinitionForBlock, currentBlockInfo));
}

void UBuilderComponent::SetCurrentBuildingItem(UBuildableBlockInfo* blockInfo)
{
	check(selector && selector->IsValidLowLevel());

	if (currentSpawnedObject)
	{
		toggleHiddenCurrentSpawned();
		selector->traceIgnoreActor = nullptr;
		currentSpawnedObject = nullptr;
	}

	if (!blockInfo || !blockInfo->IsValidLowLevel())
	{
		selector->IsUsableAllowed = true;
		return;
	}

	currentBuildableBlockInfo = blockInfo;
	selector->IsUsableAllowed = currentBuildableBlockInfo->BlockDefinition->AllowUsable;
	selector->SetOutlining(currentBuildableBlockInfo->AllowOutlineOnSelected, currentBuildableBlockInfo->StencilOverride);


	if (currentBuildableBlockInfo->Action == EBuildableObjectAction::None)
		return;

	currentBlockInfo->ID = currentBuildableBlockInfo->ID;
	if (!currentDefinitionForBlock || currentDefinitionForBlock->BlockID != currentBlockInfo->ID)
		currentDefinitionForBlock = UBlockHolderComponent::GetInstance()->GetDefinitionFor(currentBuildableBlockInfo->ID);

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

	if (!worldController->IsValidSpawnPoint(BlockHelpers1::GetSpawnBox(currentDefinitionForBlock, currentBlockInfo)))
		return;

	auto used = usedObjects.FindRef(currentBuildableBlockInfo);
	if (!used)
	{
		used = worldController->SpawnWorldObject(GetWorld(), currentBlockInfo, false);
		if (!used)
			return;

		used->SetActorEnableCollision(false);
		usedObjects.Add(currentBuildableBlockInfo, used);
		return;
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