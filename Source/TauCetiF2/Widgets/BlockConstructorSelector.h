

#pragma once

#include "Common/Enums.h"
#include "Widgets/SynchronizeWidget.h"
#include "BlockConstructorSelector.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class TAUCETIF2_API UBlockConstructorSelector : public USynchronizeWidget
{
	GENERATED_BODY()
public:

	UBlockConstructorSelector();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = TerminalBaseWidget)
		EBlockName BlockName;

	FBlockDefinition* Definition;

	virtual void SynchronizeProperties() override;
};
