

#include "TauCetiF2.h"
#include "ObjectWidget.h"




void UObjectWidget::RequestClose()
{
	print(TEXT("request close"));
	OnWidgetCloseRequest.Broadcast();
}

void UObjectWidget::OnEnterKey()
{
	auto top = ItemsStack.Top();
	if (top)
		top->OnEnterKey();
}

bool UObjectWidget::OnEscapeKey()
{
	if (ItemsStack.Num() == 0)
		return true;

	auto topEscape = ItemsStack.Top()->OnEscapeKey();
	if (topEscape)
		pop();

	return false;

}

void UObjectWidget::WidgetShown()
{
	OnWidgetShown.Broadcast();
}

void UObjectWidget::AddToStack(UObjectWidget* widget)
{
	ensure(widget);

	ItemsStack.Push(widget);
	widget->WidgetShown();
}

void UObjectWidget::SwapWithTop(UObjectWidget* widget)
{
	if (ItemsStack.Num() != 0)
		pop();

	AddToStack(widget);
}

void UObjectWidget::AddToScreen(UObjectWidget* widget, int32 ZOrder)
{
	ensure(widget);

	ItemsStack.Push(widget);
	widget->WidgetShown();
	widget->AddToViewport(ZOrder);
}
