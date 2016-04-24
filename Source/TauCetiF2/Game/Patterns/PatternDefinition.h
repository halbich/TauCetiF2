

#pragma once

#include "Object.h"
#include "Blocks/Info/BlockInfo.h"
#include "Blocks/Definitions/FBlockDefinition.h"
#include "Blocks/Definitions/FBlockDefinitionHolder.h"
#include "PatternDefinition.generated.h"

/**
 * 
 */
UCLASS()
class TAUCETIF2_API UPatternDefinition : public UObject
{
	GENERATED_BODY()
	
public:

	TArray<UBlockInfo*> UsedBlocks;
	
	void InitData();

	UPROPERTY()
		bool InitDone;

	UPROPERTY()
		UMinMaxBox* objectDimensions;

	void DEBUGSpawnPattern(const FVector& worldOffset, TArray<UBlockInfo*>& outArray);
};
