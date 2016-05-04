

#pragma once

#include "Common/Enums.h"
#include "Widgets/SynchronizeWidget.h"
#include "Blocks/Definitions/FBlockDefinition.h"
#include "Blocks/Definitions/FBlockDefinitionHolder.h"
#include "Blocks/Info/BuildableBlockInfo.h"
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = BlockConstructorSelector)
		EBlockName BlockName;

	UPROPERTY(BlueprintReadOnly, Category = BlockConstructorSelector)
		UTexture2D* BlockTexture;

	FBlockDefinition* Definition;

	virtual void SynchronizeProperties() override;
};
