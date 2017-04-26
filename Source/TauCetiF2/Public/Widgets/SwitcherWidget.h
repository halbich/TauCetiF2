#pragma once

#include "Widgets/ObjectWidget.h"
#include "Blocks/Public/Special/Electricity/Switcher.h"
#include "Blocks/Public/Components/Electricity/ElectricNetwork.h"
#include "SwitcherWidget.generated.h"

/**
 *
 */
UCLASS()
class TAUCETIF2_API USwitcherWidget : public UObjectWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | SwitcherWidget")
		ASwitcher* CurrentBlock;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | SwitcherWidget")
		UElectricNetwork* Network;

	virtual void InitForBlock_Implementation(ABlock* block) override;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | SwitcherWidget")
		void GetControllables(TArray<ABlock*>& aviables, TArray<ABlock*>& controlled);
};
