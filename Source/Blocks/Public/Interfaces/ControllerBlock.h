#pragma once

#include "ControllerBlock.generated.h"

class ABlock;

UINTERFACE(BlueprintType)
class BLOCKS_API UControllerBlock : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class BLOCKS_API IControllerBlock
{
	GENERATED_IINTERFACE_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TCF2 | ControllerBlock")
		bool BindControl(ABlock* controllableBlock);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TCF2 | ControllerBlock")
		bool UnbindControl(ABlock* controllableBlock);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TCF2 | ControllerBlock")
		bool GetControlState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TCF2 | ControllerBlock")
		TArray<ABlock*> GetControlledBlocks();
};