#include "Blocks.h"
#include "BlockWithShowableWidget.h"

UBlockWithShowableWidget::UBlockWithShowableWidget(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void IBlockWithShowableWidget::CallShowWidget(ABlock* block, TSubclassOf<UUserWidget> widgetToShow)
{
	MyShowWidgetForBlockEvent.Broadcast(block, widgetToShow);
}

FDelegateHandle IBlockWithShowableWidget::AddShowWidgetForBlockEventListener(FShowWidgetForBlockDelegate& ShowWidgetForBlockDelegate)
{
	return MyShowWidgetForBlockEvent.Add(ShowWidgetForBlockDelegate);
}

void IBlockWithShowableWidget::RemoveShowWidgetForBlockEventListener(FDelegateHandle DelegateHandle)
{
	MyShowWidgetForBlockEvent.Remove(DelegateHandle);
}