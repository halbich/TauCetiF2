﻿#pragma once

#include "Object.h"
#include "CmbItem.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class TAUCETIF2_API UCmbItem : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, Category = "TCF2 | UI | ComboBoxItem")
		int32 Value;

	UPROPERTY(BlueprintReadOnly, Category = "TCF2 | UI | ComboBoxItem")
		FText Text;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | UI | ComboBoxItem")
		static UCmbItem* MakeCmbItem(FText displayText, int32 itemValue);
};
