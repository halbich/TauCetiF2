

#pragma once
#include "Object.h"
#include "Blocks/Public/Info/BlockInfo.h"
#include "PatternElement.generated.h"

/**
 * 
 */
UCLASS()
class TAUCETIF2_API UPatternElement : public UObject
{
	GENERATED_BODY()
public:
	
	UPROPERTY(Transient)
		UBlockInfo* BlockInfo;
};




