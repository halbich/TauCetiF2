

#pragma once

#include "Blueprint/UserWidget.h"
#include "ObjectWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWidgetCloseRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWidgetShown);

/**
 * 
 */
UCLASS(Blueprintable)
class TAUCETIF2_API UObjectWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "CustomWidgets|ObjectWidget")
		FWidgetCloseRequest OnWidgetCloseRequest;
	
	UFUNCTION(BlueprintCallable, Category = "CustomWidgets|ObjectWidget")
		void RequestClose();

	UPROPERTY(BlueprintAssignable, Category = "CustomWidgets|ObjectWidget")
		FWidgetShown OnWidgetShown;
	
	void OnEnterKey();
	
	// Returns if caller should be hidden from screen
	bool OnEscapeKey();

	void WidgetShown();
};
