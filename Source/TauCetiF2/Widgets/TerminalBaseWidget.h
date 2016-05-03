

#pragma once

#include "Blueprint/UserWidget.h"
#include "TerminalBaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class TAUCETIF2_API UTerminalBaseWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = TerminalBaseWidget)
		FString DisplayName;
	
	
	
};
