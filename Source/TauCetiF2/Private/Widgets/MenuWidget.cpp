#include "TauCetiF2.h"
#include "MenuWidget.h"

void UMenuWidget::SwapWithTop(UObjectWidget* widget)
{
	if (!widget)
		return;

	Super::SwapWithTop(widget);

	if (switcher && switcher->IsValidLowLevel())
		switcher->SetActiveWidget(widget);
}