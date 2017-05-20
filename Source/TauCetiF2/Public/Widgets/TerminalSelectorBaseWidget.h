

#pragma once

#include "Blueprint/UserWidget.h"
#include "TerminalSelectorBaseWidget.generated.h"

class UTerminalSelectorBaseWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelected, UTerminalSelectorBaseWidget*, source);

/**
 * 
 */
UCLASS()
class TAUCETIF2_API UTerminalSelectorBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable, Transient, Category = "TCF2 | CustomWidgets | TerminalSelectorWidget")
		FSelected OnSelected;
	
	UFUNCTION(BlueprintCallable, Category = "TCF2 | CustomWidgets | TerminalSelectorWidget")
		void SelectClick();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TCF2 | TerminalBaseWidget")
		FText GetSelectorHeader();

	virtual FText GetSelectorHeader_Implementation();
	
};
