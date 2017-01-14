

#pragma once

#include "Blueprint/UserWidget.h"
#include "Widgets/SynchronizeWidget.h"
#include "TerminalBaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class TAUCETIF2_API UTerminalBaseWidget : public USynchronizeWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, Transient, EditAnywhere, Category = TerminalBaseWidget)
		FText BaseControlDisplayName;
	
	


};
