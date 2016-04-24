

#pragma once
#include "Object.h"
#include "Blocks/Info/BlockInfo.h"
#include "Blocks/Definitions/FBlockDefinition.h"
#include "Blocks/Definitions/FBlockDefinitionHolder.h"
#include "PatternElement.generated.h"

/**
 * 
 */
UCLASS()
class TAUCETIF2_API UPatternElement : public UObject
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
		UBlockInfo* BlockInfo;
};




