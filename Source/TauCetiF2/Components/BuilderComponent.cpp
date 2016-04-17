

#include "TauCetiF2.h"
#include "BuilderComponent.h"


// Sets default values for this component's properties
UBuilderComponent::UBuilderComponent() : currentDefinitionForBlock(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBuilderComponent::BeginPlay()
{
	Super::BeginPlay();

	currentBlockInfo = NewObject<UBlockInfo>();
	currentBlockInfo->UnderConstruction = true;
}

// Called every frame
void UBuilderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!selector || !selector->IsValidLowLevelFast() || !currentBuildableBlockInfo || !currentBuildableBlockInfo->IsValidLowLevelFast())
		return;

	/*if (!selector->SelectedActor)
	{
		if (currentSpawnedObject->IsActorTickEnabled())
		{
			currentSpawnedObject->SetActorHiddenInGame(true);
			currentSpawnedObject->SetActorTickEnabled(false);
		}
	}
	else
	{
		if (!currentSpawnedObject->IsActorTickEnabled())
		{
			currentSpawnedObject->SetActorHiddenInGame(false);
			currentSpawnedObject->SetActorTickEnabled(true);
		}
	}*/

	check(currentBuildableBlockInfo);

	if (currentBuildableBlockInfo->IsEmptyHand)
		return;

	if (!currentSpawnedObject)
	{
		SetCurrentBuildingItem(currentBuildableBlockInfo);
		return;  // Spawning might failed, probably due to invalid location. If not, we want to update next time.
	}

	check(currentDefinitionForBlock);

	auto newLocation = BlockHelpers::GetSpawnPoint(selector->ImpactPointWithSnap, selector->ImpactNormal, currentDefinitionForBlock, currentBlockInfo);

	

	if (currentBlockInfo->Location == newLocation)
		return;

	auto oldLocation = currentBlockInfo->Location;
	currentBlockInfo->Location = newLocation;
	if (!worldController->IsValidSpawnPoint(BlockHelpers::GetSpawnBox(currentDefinitionForBlock, currentBlockInfo)))
	{
		currentBlockInfo->Location = oldLocation;
		return;
	}

	currentSpawnedObject->SetActorTransform(BlockHelpers::GetSpawnTransform(currentDefinitionForBlock, currentBlockInfo));
}



void UBuilderComponent::SetCurrentBuildingItem(UBuildableBlockInfo* blockInfo)
{
	if (!blockInfo || !blockInfo->IsValidLowLevel())
		return;

	if (!selector || !selector->IsValidLowLevel() || !worldController || !worldController->IsValidLowLevel())
		return;

	if (currentSpawnedObject)
	{
		currentSpawnedObject->SetActorHiddenInGame(true);
		currentSpawnedObject->SetActorTickEnabled(false);
		selector->traceIgnoreActor = nullptr;
		currentSpawnedObject = nullptr;

	}
	currentBuildableBlockInfo = blockInfo;

	if (currentBuildableBlockInfo->IsEmptyHand)
		return;

	currentBlockInfo->ID = currentBuildableBlockInfo->ID;
	if (!currentDefinitionForBlock || currentDefinitionForBlock->ID != currentBlockInfo->ID)
		currentDefinitionForBlock = FBlockDefinitionHolder::Instance().GetDefinition(currentBuildableBlockInfo->ID);

	currentBlockInfo->Rotation = FRotator::ZeroRotator;
	currentBlockInfo->Scale = currentBuildableBlockInfo->Scale;
	currentBlockInfo->Location = BlockHelpers::GetSpawnPoint(selector->ImpactPointWithSnap, selector->ImpactNormal, currentDefinitionForBlock, currentBlockInfo);

	if (!worldController->IsValidSpawnPoint(BlockHelpers::GetSpawnBox(currentDefinitionForBlock, currentBlockInfo)))
		return;

	auto used = usedObjects.FindRef(currentBuildableBlockInfo);
	if (!used)
	{
		used = worldController->SpawnWorldObject(World, currentBlockInfo, false);
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

