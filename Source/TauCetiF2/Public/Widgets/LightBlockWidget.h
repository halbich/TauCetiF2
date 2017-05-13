#pragma once

#include "Widgets/ObjectWidget.h"
#include "Blocks/Public/Special/Electricity/LightBlock.h"
#include "Blocks/Public/Components/Electricity/ElectricNetwork.h"
#include "LightBlockWidget.generated.h"

/**
 *
 */
UCLASS()
class TAUCETIF2_API ULightBlockWidget : public UObjectWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | LightBlockWidget")
		ALightBlock* CurrentBlock;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | LightBlockWidget")
		UElectricNetwork* Network;

	virtual void InitForBlock_Implementation(ABlock* block) override;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | LightBlockWidget")
		ABlock* GetControllables(TArray<ABlock*>& aviables);
};
