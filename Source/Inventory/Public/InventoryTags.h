#pragma once

#include "Object.h"
#include "Commons/Public/Enums.h"
#include "InventoryTagGroup.h"
#include "InventoryTags.generated.h"

DECLARE_DELEGATE(FSelectionChanged);
DECLARE_EVENT(UInventoryTags, FSelectionChangedEvent);

/**
 *
 */
UCLASS(Blueprintable)
class INVENTORY_API UInventoryTags : public UObject
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
		return InventoryGroupList.IsValidIndex(CurrentActiveIndex) ? InventoryGroupList[CurrentActiveIndex] : NULL;
	}

	FORCEINLINE FString GetCurrentActiveTagGroupName()
	{
		return InventoryGroupList.IsValidIndex(CurrentActiveIndex) ? GetCurrentActiveTagGroup()->GroupName : FString();
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

			if (InventoryGroupList[index]->IsGroupEnabled)
			{
				CurrentActiveIndex = index;
				break;
			}
		}

		OnSelectionChanged.Broadcast();
	}

	FORCEINLINE void PrevBank()
	{
		auto length = InventoryGroupList.Num();
		auto start = CurrentActiveIndex - 1 + length;
		auto end = CurrentActiveIndex;

		for (int32 i = start; i > end; i--)
		{
			auto index = i  % length;

			check(InventoryGroupList.IsValidIndex(index));

			if (InventoryGroupList[index]->IsGroupEnabled)
			{
				CurrentActiveIndex = index;
				break;
			}
		}

		OnSelectionChanged.Broadcast();
	}

	FORCEINLINE int32 GetEnabledGroupsCount()
	{
		int32 res(0);

		for (auto g : InventoryGroupList)
		{
			if (g->IsGroupEnabled)
				res++;
		}

		return res;
	}

	FORCEINLINE void CheckCurrentSelection() {
		if (InventoryGroupList[CurrentActiveIndex]->IsGroupEnabled)
			return;

		NextBank();
	}
};
