#pragma once

#include "GameFramework/Character.h"
#include "Components/ActorComponent.h"
#include "Components/SelectorComponent.h"
#include "Blocks/Public/Info/BuildableBlockInfo.h"
#include "World/WorldController.h"
#include "Blocks/Public/Components/BlockHolderComponent.h"
#include "Helpers/BlockHelpers.h"
#include "BuilderComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TAUCETIF2_API UBuilderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBuilderComponent();

	UPROPERTY(Transient)
		USelectorComponent* selector;

	UPROPERTY(Transient)
		AWorldController* worldController;

	UPROPERTY(Transient)
		UBuildableBlockInfo* currentBuildableBlockInfo;

	UPROPERTY(Transient)
		UBlockInfo* currentBlockInfo;

	UPROPERTY(Transient)
		ABlock* currentSpawnedObject;

	UPROPERTY(Transient)
		UBlockDefinition* currentDefinitionForBlock;

	UPROPERTY(Transient)
		TMap<UBuildableBlockInfo*, ABlock*> usedObjects;

	UPROPERTY(Transient)
		bool ForceRecomputePosition;

	UPROPERTY(Transient)
		ACharacter* character;

	UFUNCTION(BlueprintCallable, Category = BuilderComponent)
		void SetCurrentBuildingItem(UBuildableBlockInfo* blockInfo);

	UFUNCTION(BlueprintCallable, Category = BuilderComponent)
		void SetWorldController(AWorldController* controller);

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RotatePitch(float Value);
	void RotateRoll(float Value);
	void RotateYaw(float Value);

	// TODO FORCEINLINE
	void DoAction() {
		if (!worldController || !selector->IsValidLowLevel() || !currentBlockInfo || !currentBlockInfo->IsValidLowLevel() || !currentBuildableBlockInfo || !currentBuildableBlockInfo->IsValidLowLevel())
			return;

		switch (currentBuildableBlockInfo->Action)
		{
		case EBuildableObjectAction::None:			return;
		case EBuildableObjectAction::DeleteObject:

			if (!selector->SelectedBlock)
				return;

			if (worldController->DestroyWorldObject(selector->SelectedBlock))
				print(TEXT("TODO! deletion seccessfull")); // TODO Localization!

			return;
		case EBuildableObjectAction::ConstructObject:

			if (!currentSpawnedObject || !currentSpawnedObject->IsValidLowLevel())
				return;

			auto spawnBox = BlockHelpers1::GetSpawnBox(currentDefinitionForBlock, currentBlockInfo);
			if (!worldController->IsValidSpawnPoint(spawnBox))
				return;

			auto box = spawnBox->GetBox();

			FVector origin;
			FVector extent;
			character->GetActorBounds(true, origin, extent);
			auto charBox = FBox::BuildAABB(origin, extent);

			if (!(box.Overlap(charBox) == 0))
			{
				print(TEXT("overlap")); // TODO Localization!
				return;
			}

			auto spawnBlock = NewObject<UBlockInfo>(this, NAME_None, RF_NoFlags, currentBlockInfo);

			spawnBlock->UnderConstruction = false;

			if (worldController->IsValidSpawnPoint(BlockHelpers1::GetSpawnBox(currentDefinitionForBlock, spawnBlock)))
				worldController->SpawnWorldObject(GetWorld(), spawnBlock, true);
			else
				print(TEXT("Invalid location")); // TODO Localization!

			return;
		}
	}

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
