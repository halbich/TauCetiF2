

#pragma once

#include "Object.h"
#include "Game/Inventory/InventoryTagGroup.h"
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

	UPROPERTY(BlueprintReadOnly, Category = InventoryTags)
		TArray<UInventoryTagGroup*> InventoryGroupList;

	UPROPERTY(BlueprintReadOnly, Category = InventoryTags)
		int32 CurrentActiveIndex;

	static const int32 DefaultCount;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = InventoryTags)
		static UInventoryTags* GetDefault();

	FORCEINLINE FString GetCurrentActiveTagGroupName()
	{
		return InventoryGroupList[CurrentActiveIndex]->Name;
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
	void PrevBank()
	{
		auto length = InventoryGroupList.Num();
		auto start =  CurrentActiveIndex - 1 + length;
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


};
