

#pragma once

#include "Object.h"

#include "Game/Inventory/TagGroup.h"
#include "Game/Inventory/Saving/FInventoryTagGroup.h"
#include "InventoryTagGroup.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TAUCETIF2_API UInventoryTagGroup : public UObject
{
	GENERATED_BODY()
public:


	UPROPERTY(BlueprintReadWrite, Category = InventoryTagGroup)
		TArray<UTagGroup*> GroupList;

	UPROPERTY(BlueprintReadWrite, Category = InventoryTagGroup)
		FString Name;

	UPROPERTY(BlueprintReadWrite, Category = InventoryTagGroup)
		bool IsEnabled;
	

	FORCEINLINE FInventoryTagGroup ToContainer() {
		FInventoryTagGroup result;
		result.Name = Name;
		result.IsEnabled = IsEnabled;

		for (auto tagGroup : GroupList)
		{
			if (tagGroup && tagGroup->IsValidLowLevel())
				result.GroupList.Add(tagGroup->ToContainer());
		}


		return result;
	}

	FORCEINLINE void FromContainer(const FInventoryTagGroup& invTagGroup) {


		Name = invTagGroup.Name;
		IsEnabled = invTagGroup.IsEnabled;

		for (auto groupList : invTagGroup.GroupList)
		{
			auto gl = NewObject<UTagGroup>();
			gl->FromContainer(groupList);
			GroupList.Add(gl);
		}

	}

};
