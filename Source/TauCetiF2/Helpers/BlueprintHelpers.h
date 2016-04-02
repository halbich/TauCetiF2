

#pragma once

#include "BlueprintHelpers.generated.h"

/**
 * 
 */
UCLASS()
class TAUCETIF2_API UBlueprintHelpers : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = TCF2Helpers)
		static TArray<FString> GetAllSaveGameSlotNames();
};
