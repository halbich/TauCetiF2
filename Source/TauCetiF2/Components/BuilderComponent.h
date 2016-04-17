

#pragma once

#include "Components/ActorComponent.h"
#include "Components/SelectorComponent.h"
#include "Blocks/Info/BuildableBlockInfo.h"
#include "World/WorldController.h"
#include "Blocks/WorldObject.h"
#include "Blocks/Definitions/FBlockDefinitionHolder.h"
#include "Helpers/BlockHelpers.h"
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
		UBuildableBlockInfo* currentBuildableBlockInfo;

	UPROPERTY()
		UBlockInfo* currentBlockInfo;

	UPROPERTY()
		AWorldObject* currentSpawnedObject;

	FBlockDefinition* currentDefinitionForBlock;


	UPROPERTY()
		TMap<UBuildableBlockInfo*, AWorldObject*> usedObjects;


	UFUNCTION(BlueprintCallable, Category = BuilderComponent)
		void SetCurrentBuildingItem(UBuildableBlockInfo* blockInfo);

	UFUNCTION(BlueprintCallable, Category = BuilderComponent)
		void SetWorldController(AWorldController* controller);



	void DoAction() {

		if (!selector || !selector->IsValidLowLevel() || !worldController || !selector->IsValidLowLevel() || !currentBlockInfo || !currentBlockInfo->IsValidLowLevel() || !currentSpawnedObject || !currentSpawnedObject->IsValidLowLevel())
			return;

		if (currentBuildableBlockInfo->IsEmptyHand)
		{

			return;
		}


		auto spawnBlock = NewObject<UBlockInfo>((UObject*)GetTransientPackage(), NAME_None, RF_NoFlags, currentBlockInfo);

		spawnBlock->UnderConstruction = false;

		if (worldController->IsValidSpawnPoint(BlockHelpers::GetSpawnBox(currentDefinitionForBlock, spawnBlock)))
		{
			print(TEXT("doing something"));
			worldController->SpawnWorldObject(World, spawnBlock, true);
		}

	}


};
