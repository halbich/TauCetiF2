#pragma once

#include "Widgets/ObjectWidget.h"
#include "Blocks/Public/Special/Electricity/LightBlock.h"
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

	virtual void InitForBlock_Implementation(ABlock* block) override;


	UFUNCTION(BlueprintCallable, Category = "TCF2 | LightBlockWidget")
		ABlock* GetControllables(TArray<ABlock*>& aviables);

};
