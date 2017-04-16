#pragma once

#include "Widgets/ObjectWidget.h"
#include "Blocks/Public/Special/TerminalBlock.h"
#include "Blocks/Public/Components/Electricity/ElectricNetwork.h"
#include "TerminalBaseWidget.generated.h"

/**
 *
 */
UCLASS()
class TAUCETIF2_API UTerminalBaseWidget : public UObjectWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, Transient, EditAnywhere, Category = "TCF2 | TerminalBaseWidget")
		FText BaseControlDisplayName;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | TerminalBaseWidget")
		UElectricNetwork* Network;

	virtual void InitForBlock_Implementation(ABlock* block) override;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | SwitcherWidget")
		void GetControllables(TArray<ABlock*>& aviable);
};
