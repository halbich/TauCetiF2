

#include "TauCetiF2.h"
#include "ObjectWidget.h"




void UObjectWidget::RequestClose()
{
	OnWidgetCloseRequest.Broadcast();
}

void UObjectWidget::OnEnterKey()
{


}

bool UObjectWidget::OnEscapeKey()
{
	return true;
}

void UObjectWidget::WidgetShown()
{
	OnWidgetShown.Broadcast();
}