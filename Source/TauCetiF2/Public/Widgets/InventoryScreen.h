﻿#pragma once

#include "Widgets/ObjectWidget.h"
#include "Inventory/Public/InventoryComponent.h"
#include "Inventory/Public/InventoryTags.h"
#include "InventoryScreen.generated.h"

/**
 *
 */
UCLASS()
class TAUCETIF2_API UInventoryScreen : public UObjectWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | InventoryScreen")
		UInventoryComponent* InventoryComponent;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | InventoryScreen")
		bool CanUncheck();

	UFUNCTION(BlueprintCallable, Category = "TCF2 | InventoryScreen")
		void CheckCurrentSelection();
};
