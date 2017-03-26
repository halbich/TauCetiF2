#pragma once

#include "Widgets/ObjectWidget.h"
#include "MenuWidget.generated.h"

/**
 *
 */
UCLASS()
class TAUCETIF2_API UMenuWidget : public UObjectWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Transient, Category = "CustomWidgets|ObjectWidget")
		UWidgetSwitcher* switcher;

	UFUNCTION(BlueprintCallable, Category = "CustomWidgets|ObjectWidget")
		virtual void SwapWithTop(UObjectWidget* widget) override;
};
