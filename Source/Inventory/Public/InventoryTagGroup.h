

#pragma once

#include "Object.h"

#include "TagGroup.h"
//#include "GameSave/Public/Inventory/FInventoryTagGroup.h"
#include "InventoryTagGroup.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class INVENTORY_API UInventoryTagGroup : public UObject
{
	GENERATED_BODY()
public:


	UPROPERTY(BlueprintReadWrite, Transient, Category = InventoryTagGroup)
		TArray<UTagGroup*> GroupList;

	UPROPERTY(BlueprintReadWrite, Transient, Category = InventoryTagGroup)
		FString Name;

	UPROPERTY(BlueprintReadWrite, Transient, Category = InventoryTagGroup)
		bool IsEnabled;
	

	//FORCEINLINE FInventoryTagGroup ToContainer() {
	//	FInventoryTagGroup result;
	//	result.Name = Name;
	//	result.IsEnabled = IsEnabled;

	//	for (auto tagGroup : GroupList)
	//	{
	//		if (tagGroup && tagGroup->IsValidLowLevel())
	//			result.GroupList.Add(tagGroup->ToContainer());
	//	}


	//	return result;
	//}

	//FORCEINLINE void FromContainer(const FInventoryTagGroup& invTagGroup) {


	//	Name = invTagGroup.Name;
	//	IsEnabled = invTagGroup.IsEnabled;

	//	for (auto groupList : invTagGroup.GroupList)
	//	{
	//		auto gl = NewObject<UTagGroup>(this);
	//		gl->FromContainer(groupList);
	//		GroupList.Add(gl);
	//	}

	//}


	bool IsSatisfied(TArray<FString>& tags)
	{
		for (auto tagGroup : GroupList)
		{
			if (!tagGroup->IsSatisfied(tags))
				return false;
		}

		return true;
	}
};
