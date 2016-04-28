

#include "TauCetiF2.h"

#include "TauCetiF2PlayerController.h"




void ATauCetiF2PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (wInGameMenu)
	{
		InGameMenu = CreateWidget<UObjectWidget>(this, wInGameMenu);
		InGameMenu->OnWidgetCloseRequest.AddDynamic(this, &ATauCetiF2PlayerController::OnEscapeKey);
	}

	if (wBaseControl)
	{
		BaseControl = CreateWidget<UObjectWidget>(this, wBaseControl);

		BaseControl->OnWidgetCloseRequest.AddDynamic(this, &ATauCetiF2PlayerController::OnEscapeKey);
	}

}


void ATauCetiF2PlayerController::OnEscapeKey()
{

	print(TEXT("Escape pressed"));

	switch (CurrentShownWidget)
	{
	case EShownWidget::None:
		ShowWidget(EShownWidget::InGameMenu);
		return;
	case EShownWidget::InGameMenu:
		if (InGameMenu->OnEscapeKey())
		{
			InGameMenu->RemoveFromViewport();
			CurrentShownWidget = EShownWidget::None;
			updateState();
		}
		break;
	case EShownWidget::BaseControl:
		if (BaseControl->OnEscapeKey())
		{
			BaseControl->RemoveFromViewport();
			CurrentShownWidget = EShownWidget::None;
			updateState();
		}
		break;
	default:
		checkNoEntry();
	}


}

void ATauCetiF2PlayerController::OnEnterKey()
{
	print(TEXT("Enter pressed"));

	switch (CurrentShownWidget)
	{
	case EShownWidget::None: break;
	case EShownWidget::InGameMenu:
		InGameMenu->OnEnterKey();
		break;
	case EShownWidget::BaseControl:
		BaseControl->OnEnterKey();
		break;
	default:
		checkNoEntry();
	}
}

void ATauCetiF2PlayerController::ShowWidget(const EShownWidget widget)
{

	if (CurrentShownWidget != EShownWidget::None || widget == EShownWidget::None)
		return;

	UObjectWidget* focus = nullptr;
	switch (widget)
	{
	case EShownWidget::InGameMenu:
		focus = InGameMenu;
		break;
	case EShownWidget::BaseControl:
		focus = BaseControl;
		break;
	default:
		checkNoEntry();
	}

	check(focus);
	focus->AddToViewport();
	CurrentShownWidget = widget;
	updateState();
	focus->WidgetShown();
}