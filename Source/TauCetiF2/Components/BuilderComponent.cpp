

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

	currentSpawnedObject->SetActorLocation(selector->ImpactPointWithSnap);
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
	}
	currentBlockInfo = blockInfo;


	if (currentBlockInfo->IsEmptyHand)
		return;

	print(TEXT("doing something"));

	auto used = usedObjects.FindRef(currentBlockInfo);
	if (!used)
	{
		print(TEXT("Item not used"));
		UBlockInfo* info = NewObject<UBlockInfo>();

		auto cont = currentBlockInfo->ToBaseContainer();
		info->FromBaseContainer(cont);
		info->Location = selector->ImpactPointWithSnap;					//TODO
		info->UnderConstruction = true;

		auto spawnActor = worldController->SpawnWorldObject(GetWorld(), info, false);

		ensure(spawnActor != nullptr);
		spawnActor->SetActorEnableCollision(false);

		usedObjects.Add(currentBlockInfo, spawnActor);
		currentSpawnedObject = spawnActor;
		selector->traceIgnoreActor = currentSpawnedObject;
		return;
	}

	used->SetActorHiddenInGame(true);
	used->SetActorTickEnabled(false);

	used->SetActorLocationAndRotation(selector->ImpactPointWithSnap, FQuat(FRotator::ZeroRotator));

	currentSpawnedObject = used;
	selector->traceIgnoreActor = currentSpawnedObject;

	
}

void UBuilderComponent::SetWorldController(AWorldController* controller)
{
	check(controller != nullptr);

	worldController = controller;

}

