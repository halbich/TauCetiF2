#pragma once

#include "Object.h"

#include "TagGroup.h"
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
