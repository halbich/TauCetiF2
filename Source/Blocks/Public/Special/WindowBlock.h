

#pragma once

#include "Block.h"
#include "WindowBlock.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKS_API AWindowBlock : public ABlock
{
	GENERATED_BODY()
	
public:
	AWindowBlock();
	
		UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | WindowBlock", meta = (AllowPrivateAcces = "true"))
		UElectricityComponent* ElectricityComponent;
	
};
