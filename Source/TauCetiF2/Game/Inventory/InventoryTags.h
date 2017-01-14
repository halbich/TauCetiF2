

#pragma once

#include "Object.h"
#include "Game/Inventory/InventoryTagGroup.h"
#include "Game/Inventory/Saving/FInventoryTags.h"
#include "InventoryTags.generated.h"


DECLARE_DELEGATE(FSelectionChanged);
DECLARE_EVENT(UInventoryTags, FSelectionChangedEvent);

/**
 *
 */
UCLASS(Blueprintable)
class TAUCETIF2_API UInventoryTags : public UObject
{
	GENERATED_BODY()
public:
	static const int32 DefaultCount;

	UPROPERTY(BlueprintReadOnly, Transient, Category = InventoryTags)
		TArray<UInventoryTagGroup*> InventoryGroupList;

	UPROPERTY(BlueprintReadOnly, Transient, Category = InventoryTags)
		int32 CurrentActiveIndex;


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = InventoryTags)
		static UInventoryTags* GetDefault();
	
	FORCEINLINE UInventoryTagGroup* GetCurrentActiveTagGroup()
	{
		return InventoryGroupList[CurrentActiveIndex];
	}
	
	FORCEINLINE FString GetCurrentActiveTagGroupName()
	{
		return GetCurrentActiveTagGroup()->Name;
	}

public:
	FDelegateHandle AddEventListener(FSelectionChanged& UseDelegate);
	void RemoveEventListener(FDelegateHandle DelegateHandle);
private:
	FSelectionChangedEvent OnSelectionChanged;
public:


	FORCEINLINE void NextBank()
	{
		auto length = InventoryGroupList.Num();
		auto start = (CurrentActiveIndex + 1) % length;
		auto end = start + length;
		for (int32 i = start; i < end; i++)
		{
			auto index = i % length;
			check(InventoryGroupList.IsValidIndex(index));

			if (InventoryGroupList[index]->IsEnabled)
			{
				CurrentActiveIndex = index;
				break;
			}
		}

		OnSelectionChanged.Broadcast();
	}

#pragma optimize("", off)

	// TODO FORCEINLINE
	void PrevBank()
	{
		auto length = InventoryGroupList.Num();
		auto start = CurrentActiveIndex - 1 + length;
		auto end = CurrentActiveIndex;

		for (int32 i = start; i > end; i--)
		{
			auto index = i  % length;

			check(InventoryGroupList.IsValidIndex(index));

			if (InventoryGroupList[index]->IsEnabled)
			{
				CurrentActiveIndex = index;
				break;
			}
		}

		OnSelectionChanged.Broadcast();
	}

#pragma optimize("", on)


	FORCEINLINE int32 GetEnabledGroupsCount()
	{
		int32 res(0);

		for (auto g : InventoryGroupList)
		{
			if (g->IsEnabled)
				res++;
		}

		return res;

	}

	FORCEINLINE void CheckCurrentSelection() {
		if (InventoryGroupList[CurrentActiveIndex]->IsEnabled)
			return;

		NextBank();
	}



	FORCEINLINE FInventoryTags ToContainer() {
		FInventoryTags result;
		result.CurrentActiveIndex = CurrentActiveIndex;

		for (auto invTagGroup : InventoryGroupList)
		{
			if (invTagGroup && invTagGroup->IsValidLowLevel())
				result.InventoryGroupList.Add(invTagGroup->ToContainer());
		}


		return result;
	}

	FORCEINLINE void FromContainer(const FInventoryTags& invTags) {


		CurrentActiveIndex = invTags.CurrentActiveIndex;

		for (auto invTagGroup : invTags.InventoryGroupList)
		{
			auto igl = NewObject<UInventoryTagGroup>();
			igl->FromContainer(invTagGroup);
			InventoryGroupList.Add(igl);
		}

	}


};
