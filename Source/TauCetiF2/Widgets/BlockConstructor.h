

#pragma once

#include "Widgets/TerminalBaseWidget.h"
#include "Blocks/Definitions/FBlockDefinitionHolder.h"
#include "BlockConstructor.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TAUCETIF2_API UBlockConstructor : public UTerminalBaseWidget
{
	GENERATED_BODY()
	
public:

	UBlockConstructor();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = BlockConstructorSelector)
		TArray<int32> AllAviableBlocks;
	
	
};
