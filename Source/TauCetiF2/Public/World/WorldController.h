#pragma once

#include "GameFramework/Actor.h"
#include "Blocks/Public/Interfaces/PickableBlock.h"
#include "Blueprint/UserWidget.h"
#include "Blocks/Public/Interfaces/BlockWithShowableWidget.h"
#include "Helpers/BlockHelpers.h"
#include "Helpers/WorldHelpers.h"
#include "MinMaxBox.h"
#include "KDTree.h"
#include "Game/Patterns/PatternDefinition.h"
#include "Game/Patterns/PatternDefinitionsHolder.h"
#include "Components/BaseControlComponent.h"
#include "Inventory/Public/InventoryComponent.h"
#include "Blocks/Public/Info/BlockInfo.h"
#include "Components/GameWeatherComponent.h"
#include "Components/GameElectricityComponent.h"
#include "Game/TauCetiF2PlayerController.h"

#include "Blocks/Public/Components/ElectricityComponent.h"

#include "WorldController.generated.h"


UCLASS()
class TAUCETIF2_API AWorldController : public AActor
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | WorldController")
		TArray<UBlockInfo*> UsedBlocks;

	UPROPERTY(Transient)
		UKDTree* RootBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TCF2 | WorldController", meta = (AllowPrivateAccess = "true"))
		UBaseControlComponent* BaseControl;

	UPROPERTY(Transient)
		UGameWeatherComponent* weatherComponent;

	UPROPERTY(Transient)
		UGameElectricityComponent* electricityComponent;


	UPROPERTY(Transient)
		bool debugBoxesShown;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | WorldController")
		void DEBUGShowMinMaxBoxes();

	UFUNCTION(BlueprintCallable, Category = "TCF2 | WorldController")
		void DEBUGHideMinMaxBoxes();

	UFUNCTION(BlueprintCallable, Category = "TCF2 | WorldController")
		void LoadDataFromCarrier(USaveGameCarrier* carrier);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | WorldController")
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

	TMap<ABlock*, FDelegateHandle> showableWidgetDelegates;

	UFUNCTION()
		void DestroyRequestEventHandler(ABlock* block);

private:

	void loadBlocksArray(TArray<UBlockInfo*>& blocks);

	void onPickupItem(ABlock* pickingItem);

	void onShowWidgetRequest(ABlock* block, TSubclassOf<UUserWidget> widget);

};

