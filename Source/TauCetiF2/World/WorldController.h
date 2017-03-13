#pragma once

#include "GameFramework/Actor.h"
#include "Blocks/Public/Components/BlockHolderComponent.h"
#include "Blocks/Public/Interfaces/PickableBlock.h"
#include "Helpers/BlockHelpers.h"
#include "Helpers/WorldHelpers.h"
#include "MinMaxBox.h"
#include "KDTree.h"
#include "Game/Patterns/PatternDefinition.h"
#include "Game/Patterns/PatternDefinitionsHolder.h"
#include "Components/BaseControlComponent.h"
//#include "Game/TauCetiF2Character.h"

#include "WorldController.generated.h"

UCLASS()
class TAUCETIF2_API AWorldController : public AActor
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Transient, Category = WorldController)
		TArray<UBlockInfo*> UsedBlocks;

	UPROPERTY(Transient)
		UKDTree* RootBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldController, meta = (AllowPrivateAccess = "true"))
		UBlockHolderComponent* BlockHolder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldController, meta = (AllowPrivateAccess = "true"))
		UBaseControlComponent* BaseControl;

	UPROPERTY(Transient)
		bool debugBoxesShown;

	UFUNCTION(BlueprintCallable, Category = WorldController)
		void DEBUGShowMinMaxBoxes();

	UFUNCTION(BlueprintCallable, Category = WorldController)
		void DEBUGHideMinMaxBoxes();

	UFUNCTION(BlueprintCallable, Category = WorldController)
		void DEBUGSpawnPatterns(const FVector & startingPoint);

	UFUNCTION(BlueprintCallable, Category = WorldController)
		void DEBUGUsedPatternElements(const FVector & startingPoint);

	UFUNCTION(BlueprintCallable, Category = WorldController)
		void LoadDataFromCarrier(USaveGameCarrier* carrier);

	UFUNCTION(BlueprintCallable, Category = WorldController)
		void SaveDataToCarrier(USaveGameCarrier* carrier);

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReasonType) override;

	ABlock* SpawnWorldObject(UWorld* world, UBlockInfo* block, bool addToRoot);

	bool DestroyWorldObject(ABlock* object);

	FORCEINLINE bool IsValidSpawnPoint(const UMinMaxBox* box)
	{
		return RootBox->IsPlaceEmpty(box);
	}

	TMap<ABlock*, FDelegateHandle> pickableDelegates;

private:

	void loadBlocksArray(TArray<UBlockInfo*>& blocks);

	void onPickupItem(ABlock* pickingItem);
};
