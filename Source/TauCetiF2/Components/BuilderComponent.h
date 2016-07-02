

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

		if (!selector || !selector->IsValidLowLevel() || !worldController || !selector->IsValidLowLevel() || !currentBlockInfo || !currentBlockInfo->IsValidLowLevel() || !currentBuildableBlockInfo || !currentBuildableBlockInfo->IsValidLowLevel())
			return;

		switch (currentBuildableBlockInfo->Action)
		{
		case EBuildableObjectAction::None:			return;
		case EBuildableObjectAction::DeleteObject:

			if (!selector->SelectedTarget)
				return;

			if (worldController->DestroyWorldObject(selector->SelectedTarget))
				print(TEXT("deletion seccessfull"));

			return;
		case EBuildableObjectAction::ConstructObject:

			if (!currentSpawnedObject || !currentSpawnedObject->IsValidLowLevel())
				return;

			auto spawnBox = BlockHelpers::GetSpawnBox(currentDefinitionForBlock, currentBlockInfo);
			if (!worldController->IsValidSpawnPoint(spawnBox))
				return;

			auto box = spawnBox->GetBox();

			FVector origin;
			FVector extent;
			character->GetActorBounds(true, origin, extent);
			auto charBox = FBox::BuildAABB(origin, extent);

			if (!(box.Overlap(charBox) == 0))
			{
				print(TEXT("overlap"));
				return;
			}

			auto spawnBlock = NewObject<UBlockInfo>((UObject*)GetTransientPackage(), NAME_None, RF_NoFlags, currentBlockInfo);

			spawnBlock->UnderConstruction = false;

			if (worldController->IsValidSpawnPoint(BlockHelpers::GetSpawnBox(currentDefinitionForBlock, spawnBlock)))
				worldController->SpawnWorldObject(GetWorld(), spawnBlock, true);
			else
				print(TEXT("Invalid location"));

			return;
		}
	}


	void RotatePitch(float Value);
	void RotateRoll(float Value);
	void RotateYaw(float Value);

	FORCEINLINE void AddRotation(float Pitch, float Yaw, float Roll)
	{

		if (!character)
			return;

		auto grid = GameDefinitions::WorldGrid;
		auto gr = grid.Add(90, 0, 0);
		auto rotator = character->GetControlRotation().GridSnap(gr);

		print(*rotator.ToString());
		auto desiredRotation = rotator.UnrotateVector(FVector(Pitch, Roll, Yaw));

		currentBlockInfo->Rotation = (FQuat(FRotator(desiredRotation.X, desiredRotation.Z, desiredRotation.Y)) *  FQuat(currentBlockInfo->Rotation)).Rotator();
		currentBlockInfo->Rotation = currentBlockInfo->Rotation.GridSnap(GameDefinitions::WorldGrid);
		ForceRecomputePosition = true;
	}

	FORCEINLINE void toggleHiddenCurrentSpawned(bool newHidden = true)
	{
		currentSpawnedObject->SetActorHiddenInGame(newHidden);
		currentSpawnedObject->SetActorTickEnabled(!newHidden);

	}
};
