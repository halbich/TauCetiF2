#pragma once

#include "Widgets/ObjectWidget.h"
#include "Widgets/TerminalBaseWidget.h"
#include "Widgets/BlockConstructor.h"
#include "Blocks/Public/Special/TerminalBlock.h"
#include "Blocks/Public/Components/Electricity/ElectricNetwork.h"
#include "Blocks/Public/Info/PatternImplementation/CreatorPatternGroupInfo.h"
#include "BaseControlWidget.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class TAUCETIF2_API UBaseControlWidget : public UObjectWidget
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient, Category = "TCF2 | BaseControlWidget")
		TArray<UTerminalBaseWidget*> ContainingTerminalWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCF2 | BaseControlWidget")
		TSubclassOf<class UTerminalBaseWidget> wTerminalBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCF2 | BaseControlWidget")
		TSubclassOf<class UBlockConstructor> wBlockConstructor;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BaseControlWidget")
		UElectricNetwork* Network;

	virtual void InitForBlock_Implementation(ABlock* block) override;
};
