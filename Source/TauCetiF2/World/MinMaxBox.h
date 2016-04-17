

#pragma once

#include "Object.h"
#include "Helpers/Helpers.h"
#include "Common/WorldObject.h"
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
	UMinMaxBox* InitBox(const FTransform& transform);

	UPROPERTY()
		FVector Min;

	UPROPERTY()
		FVector Max;
	
	
};
