

#pragma once

#include "Object.h"
#include "InventoryTagGroup.h"
//#include "GameSave/Public/Inventory/FInventoryTags.h"
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
		return InventoryGroupList.IsValidIndex(CurrentActiveIndex) ? GetCurrentActiveTagGroup()->Name : FString();
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





};
