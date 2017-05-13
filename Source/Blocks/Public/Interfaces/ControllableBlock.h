#pragma once

#include "ControllableBlock.generated.h"

class ABlock;

UINTERFACE(BlueprintType)
class BLOCKS_API UControllableBlock : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class BLOCKS_API IControllableBlock
{
	GENERATED_IINTERFACE_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TCF2 | ControllableBlock")
		void SetControlState(bool isOn);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TCF2 | ControllableBlock")
		void SetOutputPowerPercentage(float percentage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TCF2 | ControllableBlock")
		void SetController(ABlock* controller);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TCF2 | ControllableBlock")
		ABlock* GetController();
};