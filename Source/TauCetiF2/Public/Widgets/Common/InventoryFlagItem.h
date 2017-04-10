#pragma once

#include "Object.h"
#include "CmbItem.h"
#include "InventoryFlagItem.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class TAUCETIF2_API UInventoryFlagItem : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "TCF2 | UI | ComboBoxItem")
		FString TagName;

	UPROPERTY(BlueprintReadOnly, Category = "TCF2 | UI | ComboBoxItem")
		FText DisplayText;

	UPROPERTY(BlueprintReadOnly, Category = "TCF2 | UI | ComboBoxItem")
		TArray<UCmbItem*> AviableValues;

	UPROPERTY(BlueprintReadOnly, Category = "TCF2 | UI | ComboBoxItem")
		int32 TagValue;

	UPROPERTY(BlueprintReadOnly, Category = "TCF2 | UI | ComboBoxItem")
		bool TagReadOnly;
};
