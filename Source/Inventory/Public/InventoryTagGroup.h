#pragma once

#include "Object.h"

#include "TagGroup.h"
#include "Commons/Public/Enums.h"
#include "InventoryTagGroup.generated.h"

class UBuildableBlockInfo;

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

	UPROPERTY(Transient)
		bool IsBuildableCacheValid;

	UPROPERTY(Transient)
		TArray<UBuildableBlockInfo*> BuildableCache;

	UPROPERTY(Transient)
		bool IsInventoryCacheValid;

	UPROPERTY(Transient)
		TArray<UBuildableBlockInfo*> InventoryCache;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | InventoryTagGroup")
		UTagGroup* AddTagGroup(UTagGroup* group);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | InventoryTagGroup")
		bool RemoveTagGroup(UTagGroup* group);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | InventoryTagGroup")
		void InvalidateGroup();

	bool IsSatisfied(TArray<FString>& tags, TArray<FString>& implicitTags)
	{
		for (auto tagGroup : GroupList)
		{
			if (!tagGroup->IsSatisfied(tags, implicitTags))
				return false;
		}

		return true;
	}
};
