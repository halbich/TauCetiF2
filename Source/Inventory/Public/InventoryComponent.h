﻿#pragma once

#include "Components/ActorComponent.h"
#include "Blocks/Public/Info/BuildableBlockInfo.h"
#include "Blocks/Public/Info/InventoryBuildableBlockInfo.h"
#include "InventoryTags.h"
#include "TagGroup.h"
#include "InventoryTagGroup.h"

#include "GameSave/Public/SaveGameCarrier.h"
#include "GameSave/Public/Inventory/FTagGroup.h"
#include "GameSave/Public/Inventory/FInventoryTagGroup.h"
#include "GameSave/Public/Inventory/FInventoryTags.h"

#include "InventoryHelpers.h"
#include "Blocks/Public/Info/BlockHolder.h"

#include "Commons/Public/Enums.h"
#include "GameSave/Public/SaveGameCarrier.h"
#include "GameSave/Public/BlockSaveInfo.h"
#include "Blocks/Public/Helpers/BlockHelpers.h"

#include "Blocks/Public/Info/BuildableBlockInfo.h"
#include "Blocks/Public/Info/InventoryBuildableBlockInfo.h"

#include "InventoryTags.h"
#include "TagGroup.h"
#include "InventoryTagGroup.h"

#include "GameSave/Public/Inventory/FTagGroup.h"
#include "GameSave/Public/Inventory/FInventoryTagGroup.h"
#include "GameSave/Public/Inventory/FInventoryTags.h"

#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHudBuildableItemsChanged, bool, ShowGroupName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCurrentSelectedIndexChanged, int32, NewIndex, UBuildableBlockInfo*, BlockInfo);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INVENTORY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | InventoryComponent")
		TArray<UBuildableBlockInfo*> BuildableItems;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | InventoryComponent")
		TArray<UInventoryBuildableBlockInfo*> InventoryItems;

	UPROPERTY(BlueprintAssignable, Transient, Category = "TCF2 | InventoryComponent")
		FHudBuildableItemsChanged OnHudBuildableItemsChanged;

	UPROPERTY(BlueprintAssignable, Transient, Category = "TCF2 | InventoryComponent")
		FCurrentSelectedIndexChanged OnCurrentSelectedIndexChanged;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | InventoryComponent")
		UInventoryTags* InventoryTags;


	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | InventoryComponent")
		int32 CurrentSelectedIndex;


	UFUNCTION(BlueprintCallable, Category = "TCF2 | InventoryComponent")
		void ForceItemsChanged(bool showGroupName);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | InventoryComponent")
		TArray<UBuildableBlockInfo*> GetItemsForCurrentBank();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | InventoryComponent")
		TArray<UBuildableBlockInfo*> GetItemsForBank(UInventoryTagGroup* filterGroup);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | InventoryComponent")
		FString GetCurrentBankName();

	FDelegateHandle ListeningHandle;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void InventoryTagsSelectionChanged();

	void LoadFromCarrier(USaveGameCarrier* carrier, TArray<FText>& validationErrors);

	void SaveToCarrier(USaveGameCarrier* carrier);

	void SelectNextBank();
	void SelectPrevBank();

	UFUNCTION(BlueprintCallable, Category = "TCF2 | InventoryComponent")
	void SelectNextItem();

	UFUNCTION(BlueprintCallable, Category = "TCF2 | InventoryComponent")
	void SelectPrevItem();

	UFUNCTION(BlueprintCallable, Category = "TCF2 | InventoryComponent")
	void EmptyHand();

	void AddItem(UBuildableBlockInfo* block);

	void AddItem(UInventoryBuildableBlockInfo* block);

	void ItemBuilt(UInventoryBuildableBlockInfo* block);

private:
	FORCEINLINE void rebuildBuildableCache(UInventoryTagGroup* filterGroup)
	{
		filterGroup->BuildableCache.Empty();
		for (auto b : BuildableItems)
		{
			if (b->BlockDefinition->IsPlaceable && filterGroup->IsSatisfied(b->Tags))
				filterGroup->BuildableCache.Add(b);
		}
		filterGroup->IsBuildableCacheValid = true;
	}

	FORCEINLINE void rebuildInventoryCache(UInventoryTagGroup* filterGroup)
	{
		filterGroup->InventoryCache.Empty();
		for (auto b : InventoryItems)
		{
			if (b->BlockDefinition->IsInventoryObject && filterGroup->IsSatisfied(b->Tags))
				filterGroup->InventoryCache.Add(b);
		}
		filterGroup->IsInventoryCacheValid = true;
	}
};
