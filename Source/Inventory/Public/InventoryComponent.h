#pragma once

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCurrentGroupChanged);
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
		FCurrentGroupChanged OnCurrentGroupChanged;

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

	UFUNCTION(BlueprintCallable, Category = "TCF2 | InventoryComponent")
		void AddItem(UInventoryBuildableBlockInfo* block);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | InventoryComponent")
		void ItemBuilt(UInventoryBuildableBlockInfo* block);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | InventoryComponent")
		bool TryRemove(UBuildableBlockInfo* blockInfo);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | InventoryComponent")
		bool TryRemoveInventory(UInventoryBuildableBlockInfo* blockInfo);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | InventoryComponent")
		void ClearBuildableItems();

private:
	FORCEINLINE void rebuildBuildableCache(UInventoryTagGroup* filterGroup)
	{
		filterGroup->BuildableCache.Empty();
		for (auto b : BuildableItems)
		{
			if (b->BlockDefinition->IsPlaceable && filterGroup->IsSatisfied(b->Tags, b->ImplicitTags))
				filterGroup->BuildableCache.Add(b);
		}

		filterGroup->IsBuildableCacheValid = true;
		filterGroup->SatisfiedCount = filterGroup->BuildableCache.Num();
		filterGroup->TotalItemsCount = BuildableItems.Num();
	}

	FORCEINLINE void rebuildInventoryCache(UInventoryTagGroup* filterGroup)
	{
		filterGroup->InventoryCache.Empty();
		for (auto b : InventoryItems)
		{
			if (b->BlockDefinition->IsInventoryObject && filterGroup->IsSatisfied(b->Tags, b->ImplicitTags))
				filterGroup->InventoryCache.Add(b);
		}
		filterGroup->IsInventoryCacheValid = true;

		filterGroup->SatisfiedCount = filterGroup->InventoryCache.Num();
		filterGroup->TotalItemsCount = InventoryItems.Num();
	}

	FORCEINLINE void selectItem(int32 offset)
	{
		UBuildableBlockInfo* infoToSend = NULL;
		auto filterGroup = InventoryTags->GetCurrentActiveTagGroup();
		switch (filterGroup->GroupType)
		{
		case EInventoryGroupType::Building: {
			if (!filterGroup->IsBuildableCacheValid) rebuildBuildableCache(filterGroup);
			auto count = filterGroup->BuildableCache.Num();
			if (count > 0)
			{
				if (CurrentSelectedIndex == -1 && offset < 0)
					CurrentSelectedIndex = 0;

				CurrentSelectedIndex = offset == 0 && (CurrentSelectedIndex < 0 || CurrentSelectedIndex >= count) ? -1 : (CurrentSelectedIndex + count + offset) % count;
				if (filterGroup->BuildableCache.IsValidIndex(CurrentSelectedIndex))
					infoToSend = filterGroup->BuildableCache[CurrentSelectedIndex];
			}
			else
				CurrentSelectedIndex = -1;

			break;
		}
		case EInventoryGroupType::Inventory: {
			if (!filterGroup->IsInventoryCacheValid) rebuildInventoryCache(filterGroup);
			auto count = filterGroup->InventoryCache.Num();
			if (count > 0)
			{
				if (CurrentSelectedIndex == -1 && offset < 0)
					CurrentSelectedIndex = 0;

				CurrentSelectedIndex = offset == 0 && (CurrentSelectedIndex < 0 || CurrentSelectedIndex >= count) ? -1 : (CurrentSelectedIndex + count + offset) % count;
				if (filterGroup->InventoryCache.IsValidIndex(CurrentSelectedIndex))
					infoToSend = filterGroup->InventoryCache[CurrentSelectedIndex];
			}
			else
				CurrentSelectedIndex = -1;

			break;
		}
		default:
			checkNoEntry();
			break;
		}

		OnCurrentSelectedIndexChanged.Broadcast(CurrentSelectedIndex, infoToSend);
	}
};
