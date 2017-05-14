#pragma once

#include "GameFramework/Character.h"
#include "Components/ActorComponent.h"
#include "Components/SelectorComponent.h"
#include "Inventory/Public/InventoryComponent.h"
#include "Blocks/Public/Info/BuildableBlockInfo.h"
#include "Blocks/Public/Info/InventoryBuildableBlockInfo.h"
#include "World/WorldController.h"
#include "Blocks/Public/Info/BlockHolder.h"
#include "Blocks/Public/Components/ElectricityComponent.h"
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
		UInventoryComponent* inventory;

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
		UElectricityComponent* BuilderElectricityComponent;

	UPROPERTY(Transient)
		ACharacter* character;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | BuilderComponent")
		void SetWorldController(AWorldController* controller);

	UPROPERTY(Transient)
		bool IsInCreative;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BuilderComponent")
		UBlockHolder* blockHolder;

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RotatePitch(float Value);
	void RotateRoll(float Value);
	void RotateYaw(float Value);

	void DoAction();

	UFUNCTION()
		void InventorySelectedChangedEvent(int32 NewIndex, UBuildableBlockInfo* BlockInfo);

	FORCEINLINE void AddRotation(float Pitch, float Yaw, float Roll)
	{
		if (!character)
			return;

		auto grid = GameDefinitions::WorldGrid;
		auto gr = grid.Add(90, 0, 0);
		auto rotator = character->GetControlRotation().GridSnap(gr);

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

	FORCEINLINE void ensureHolder()
	{
		if (blockHolder && blockHolder->IsValidLowLevel())
			return;

		auto inst = Cast<UTCF2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		ensure(inst);
		blockHolder = Cast<UBlockHolder>(inst->BlockHolder);
		ensure(blockHolder);
	}

private:
	void setCurrentBuildingItem(UBuildableBlockInfo* blockInfo);
};
