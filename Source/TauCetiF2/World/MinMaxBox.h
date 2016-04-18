

#pragma once

#include "Object.h"
#include "Helpers/Helpers.h"
#include "MinMaxBox.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TAUCETIF2_API UMinMaxBox : public UObject
{
	GENERATED_BODY()
	
public:

	UMinMaxBox* InitBox(FVector min, FVector max);
	UMinMaxBox* InitBoxChecked(FVector min, FVector max);

	UPROPERTY()
		FVector Min;

	UPROPERTY()
		FVector Max;
	
	
};
