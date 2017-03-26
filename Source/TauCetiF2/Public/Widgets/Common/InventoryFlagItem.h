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

	UPROPERTY(BlueprintReadOnly, Category = CmbItem)
		FString TagName;

	UPROPERTY(BlueprintReadOnly, Category = CmbItem)
		FText DisplayText;

	UPROPERTY(BlueprintReadOnly, Category = CmbItem)
		TArray<UCmbItem*> AviableValues;

	UPROPERTY(BlueprintReadOnly, Category = CmbItem)
		int32 TagValue;

	UPROPERTY(BlueprintReadOnly, Category = CmbItem)
		bool TagReadOnly;
};
