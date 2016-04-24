

#pragma once

#include "Object.h"
#include "Blocks/Info/BlockInfo.h"
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
	
	
};
