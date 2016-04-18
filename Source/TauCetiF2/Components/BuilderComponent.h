

#pragma once

#include "GameFramework/Character.h"
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

	UPROPERTY()
		bool ForceRecomputePosition;

	UPROPERTY()
		ACharacter* character;

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
		else
			print(TEXT("Invalid location"));

	}


	void RotatePitch(float Value);
	void RotateRoll(float Value);
	void RotateYaw(float Value);

	FORCEINLINE void AddRotation(float Pitch, float Yaw, float Roll)
	{

		if (!character)
			return;

		auto rotator = character->GetControlRotation().GridSnap(GameDefinitions::WorldGrid);
		auto desiredRotation = rotator.UnrotateVector(FVector(Pitch,Yaw, Roll));
		currentBlockInfo->Rotation.Add(desiredRotation.X, desiredRotation.Z, desiredRotation.Y);

		currentBlockInfo->Rotation = currentBlockInfo->Rotation.GridSnap(GameDefinitions::WorldGrid);
		ForceRecomputePosition = true;
	}
};
