

#pragma once

#include "Widgets/ObjectWidget.h"
#include "Game/Inventory/InventoryTags.h"
#include "InventoryScreen.generated.h"

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
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "InventoryScreen")
		bool CanUncheck();


	UFUNCTION(BlueprintCallable, Category = "InventoryScreen")
		void CheckCurrentSelection();
};
