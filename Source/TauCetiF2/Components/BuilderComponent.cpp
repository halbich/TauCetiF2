

#include "TauCetiF2.h"
#include "BuilderComponent.h"


// Sets default values for this component's properties
UBuilderComponent::UBuilderComponent()
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

	// ...

}

// Called every frame
void UBuilderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!currentSpawnedObject || !selector || !selector->IsValidLowLevelFast())
		return;

	if (!selector->SelectedActor)
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
	}

	auto newSpawnPoint = GetSpawnPoint();
	if (newSpawnPoint == currentValidSpawnPoint)
		return;

	auto trans = UHelpers::GetSpawnTransform(GetSpawnPoint(), currentBlockInfo->Scale, currentBlockRotation);
	if (!worldController->IsValidSpawnPoint(trans))
		return;

	currentValidSpawnPoint = newSpawnPoint;

	currentSpawnedObject->SetActorLocation(UHelpers::GetSpawnCoords(currentValidSpawnPoint, currentBlockInfo->Scale, currentBlockRotation));
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
	currentBlockInfo = blockInfo;


	if (currentBlockInfo->IsEmptyHand)
		return;

	print(TEXT("doing something"));

	auto used = usedObjects.FindRef(currentBlockInfo);
	if (!used)
	{
		print(TEXT("Item not used"));
		UBlockInfo* info = NewObject<UBlockInfo>(this);

		auto cont = currentBlockInfo->ToBaseContainer();
		info->FromBaseContainer(cont);
		info->Location = GetSpawnPoint();					//TODO
		info->Rotation = currentBlockRotation;
		info->UnderConstruction = true;

		auto spawnActor = worldController->SpawnWorldObject(World, info, false);
		if (!spawnActor)
			return;


		spawnActor->SetActorEnableCollision(false);
		currentValidSpawnPoint = info->Location;
		usedObjects.Add(currentBlockInfo, spawnActor);
		currentSpawnedObject = spawnActor;
		selector->traceIgnoreActor = currentSpawnedObject;
		return;
	}

	used->SetActorHiddenInGame(false);
	used->SetActorTickEnabled(true);

	used->SetActorLocationAndRotation(UHelpers::GetSpawnCoords(GetSpawnPoint(), currentBlockInfo->Scale, currentBlockRotation), FQuat(currentBlockRotation));

	currentSpawnedObject = used;
	selector->traceIgnoreActor = currentSpawnedObject;


}

void UBuilderComponent::SetWorldController(AWorldController* controller)
{
	check(controller != nullptr);

	worldController = controller;

}

