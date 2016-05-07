

#pragma once

#include "Object.h"
#include "Game/Inventory/InventoryTagGroup.h"
#include "InventoryTags.generated.h"

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


	static const int32 DefaultCount;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = InventoryTags)
		static UInventoryTags* GetDefault();
};
