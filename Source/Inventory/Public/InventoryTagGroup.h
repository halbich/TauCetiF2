#pragma once

#include "Object.h"

#include "TagGroup.h"
#include "Commons/Public/Enums.h"
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
		FString GroupName;

	UPROPERTY(BlueprintReadWrite, Transient, Category = InventoryTagGroup)
		bool IsGroupEnabled;

	UPROPERTY(BlueprintReadWrite, Transient, Category = InventoryTagGroup)
		EInventoryGroupType GroupType;

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
