

#pragma once

#include "Components/ActorComponent.h"
#include "Components/SelectorComponent.h"
#include "Blocks/Info/BuildableBlockInfo.h"
#include "World/WorldController.h"
#include "Blocks/WorldObject.h"
#include "Blocks/Definitions/FBlockDefinitionHolder.h"
#include "BuilderComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TAUCETIF2_API UBuilderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBuilderComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY()
		USelectorComponent* selector;

	UPROPERTY()
		AWorldController* worldController;


	UPROPERTY()
		UBuildableBlockInfo* currentBlockInfo;

	UPROPERTY()
		AWorldObject* currentSpawnedObject;


	UPROPERTY()
		TMap<UBuildableBlockInfo*, AWorldObject*> usedObjects;

	UPROPERTY()
		FRotator currentBlockRotation;

	UPROPERTY()
		FVector currentValidSpawnPoint;


	UFUNCTION(BlueprintCallable, Category = BuilderComponent)
		void SetCurrentBuildingItem(UBuildableBlockInfo* blockInfo);

	UFUNCTION(BlueprintCallable, Category = BuilderComponent)
		void SetWorldController(AWorldController* controller);


	FVector GetSpawnPoint() {

		check(currentBlockInfo != nullptr);

		auto baseLocation = selector->ImpactPointWithSnap / GameDefinitions::CubeMinSize;
		auto rotatedScale = currentBlockRotation.RotateVector(currentBlockInfo->Scale);
		auto offset = selector->ImpactNormal * UHelpers::GetSpawnOffset(currentBlockRotation, currentBlockInfo->Scale);
		auto normalAdd = selector->ImpactNormal * rotatedScale * 0.5 - offset;
		auto normA = FVector(FMath::FloorToInt(normalAdd.X), FMath::FloorToInt(normalAdd.Y), FMath::FloorToInt(normalAdd.Z));
		auto result = baseLocation + normA;

		return result;

	}


	void DoAction() {

		if (!selector || !selector->IsValidLowLevel() || !worldController || !selector->IsValidLowLevel() || !currentBlockInfo || !currentBlockInfo->IsValidLowLevel() || !currentSpawnedObject || !currentSpawnedObject->IsValidLowLevel())
			return;

		/*if (currentBlockInfo->IsEmptyHand)
		{

			return;
		}

		print(TEXT("doing something"));

		auto used = usedObjects.Find(currentBlockInfo);
		check(used && "Failed to find currentBlockInfo object");


		auto spawnBlock = NewObject<UBlockInfo>((UObject*)GetTransientPackage(), NAME_None, RF_NoFlags, currentSpawnedObject->WorldObjectComponent->BlockInfo);

		spawnBlock->Location = currentValidSpawnPoint;
		spawnBlock->Rotation = currentBlockRotation;
		spawnBlock->UnderConstruction = false;

		worldController->SpawnWorldObject(World, spawnBlock, true);*/

	}


};
