

#pragma once

#include "Widgets/ObjectWidget.h"
#include "Game/Inventory/InventoryTags.h"
#include "InventoryScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryTagsSet);

/**
 * 
 */
UCLASS()
class TAUCETIF2_API UInventoryScreen : public UObjectWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, Category = InventoryScreen)
		UInventoryTags* InventoryTags;
	
	UPROPERTY(BlueprintAssignable, Category = InventoryComponent)
		FInventoryTagsSet OnInventoryTagsSet;
	
	FORCEINLINE void SetInventoryTags(UInventoryTags* tags) {
		InventoryTags = tags;
		OnInventoryTagsSet.Broadcast();
	}

};
