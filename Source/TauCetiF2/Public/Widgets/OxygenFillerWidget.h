#pragma once

#include "Widgets/ObjectWidget.h"
#include "Game/TauCetiF2Character.h"
#include "Blocks/Public/Special/Oxygen/OxygenTankFillerBlock.h"
#include "OxygenFillerWidget.generated.h"

/**
 *
 */
UCLASS()
class TAUCETIF2_API UOxygenFillerWidget : public UObjectWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | OxygenFillerWidget")
		UInventoryComponent* InventoryComponent;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | OxygenFillerWidget")
		AOxygenTankFillerBlock* CurrentBlock;

	virtual void InitForBlock_Implementation(ABlock* block) override;
};
