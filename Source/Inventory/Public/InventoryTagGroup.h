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

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | InventoryTagGroup")
		TArray<UTagGroup*> GroupList;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | InventoryTagGroup")
		FString GroupName;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | InventoryTagGroup")
		bool IsGroupEnabled;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | InventoryTagGroup")
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
