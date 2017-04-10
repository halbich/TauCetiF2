#pragma once

#include "Block.h"
#include "Blueprint/UserWidget.h"
#include "BlockWithShowableWidget.generated.h"

DECLARE_DELEGATE_TwoParams(FShowWidgetForBlockDelegate, ABlock*, TSubclassOf<UUserWidget>);
DECLARE_EVENT_TwoParams(IBlockWithShowableWidget, FShowWidgetForBlockEvent, ABlock*, TSubclassOf<UUserWidget>);

UINTERFACE(BlueprintType)
class BLOCKS_API UBlockWithShowableWidget : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class BLOCKS_API IBlockWithShowableWidget
{
	GENERATED_IINTERFACE_BODY()


public:

	IBlockWithShowableWidget()
	{
		MyShowWidgetForBlockEvent = FShowWidgetForBlockEvent();
	};

	void CallShowWidget(ABlock* block, TSubclassOf<UUserWidget> widgetToShow);

	FDelegateHandle AddShowWidgetForBlockEventListener(FShowWidgetForBlockDelegate& ShowWidgetForBlockDelegate);
	void RemoveShowWidgetForBlockEventListener(FDelegateHandle DelegateHandle);

	virtual void SetDisplayedWidget(UUserWidget* widget) = 0;

	virtual UUserWidget* GetShownWidget() = 0;

protected:
	FShowWidgetForBlockEvent MyShowWidgetForBlockEvent;
};
