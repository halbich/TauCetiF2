#pragma once

#include "Widgets/ObjectWidget.h"
#include "WidgetSwitcher.h"
#include "MenuWidget.generated.h"

/**
 *
 */
UCLASS()
class TAUCETIF2_API UMenuWidget : public UObjectWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | CustomWidgets | MenuWidget")
		UWidgetSwitcher* switcher;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | CustomWidgets |  MenuWidget")
		virtual void SwapWithTop(UObjectWidget* widget) override;
};
