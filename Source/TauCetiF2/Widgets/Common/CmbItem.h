#pragma once

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

	UPROPERTY(BlueprintReadOnly, Category = CmbItem)
		int32 Value;

	UPROPERTY(BlueprintReadOnly, Category = CmbItem)
		FText Text;
};
