

#include "TauCetiF2.h"
#include "TerminalSelectorBaseWidget.h"




void UTerminalSelectorBaseWidget::SelectClick()
{
	OnSelected.Broadcast(this);
}

FText UTerminalSelectorBaseWidget::GetSelectorHeader_Implementation()
{
	return FText();
}