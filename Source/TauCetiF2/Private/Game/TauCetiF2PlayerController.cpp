#include "TauCetiF2.h"

#include "TauCetiF2PlayerController.h"

void ATauCetiF2PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (wMainMenu)
	{
		MainMenu = CreateWidget<UMainMenuWidget>(this, wMainMenu);
		MainMenu->OnWidgetCloseRequest.AddDynamic(this, &ATauCetiF2PlayerController::OnEscapeKey);
	}

	if (wInGameMenu)
	{
		InGameMenu = CreateWidget<UObjectWidget>(this, wInGameMenu);
		InGameMenu->OnWidgetCloseRequest.AddDynamic(this, &ATauCetiF2PlayerController::OnEscapeKey);
	}

	if (wInventory)
	{
		Inventory = CreateWidget<UInventoryScreen>(this, wInventory);
		Inventory->OnWidgetCloseRequest.AddDynamic(this, &ATauCetiF2PlayerController::OnEscapeKey);
	}
}

void ATauCetiF2PlayerController::OnEscapeKey()
{
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
	case EShownWidget::MainMenu:
		if (MainMenu->OnEscapeKey())
		{
			MainMenu->OnMainMenuCloseRequest.Broadcast();
		}
		break;
	case EShownWidget::Inventory:
		if (Inventory->OnEscapeKey())
		{
			Inventory->RemoveFromViewport();
			CurrentShownWidget = EShownWidget::None;
			updateState();
		}
		break;

	case EShownWidget::Registered:
		ensure(currentShownRegisteredWidget);
		if (currentShownRegisteredWidget->OnEscapeKey())
		{
			currentShownRegisteredWidget->RemoveFromViewport();
			CurrentShownWidget = EShownWidget::None;
			currentShownRegisteredWidget = NULL;
			updateState();
		}
		break;
	default:
		checkNoEntry();
	}
}

void ATauCetiF2PlayerController::OnEnterKey()
{
	switch (CurrentShownWidget)
	{
	case EShownWidget::None: break;
	case EShownWidget::InGameMenu:
		InGameMenu->OnEnterKey();
		break;
	case EShownWidget::MainMenu:
		MainMenu->OnEnterKey();
		break;
	case EShownWidget::Inventory:
		MainMenu->OnEnterKey();
		break;
	case EShownWidget::Registered:
		ensure(currentShownRegisteredWidget);
		currentShownRegisteredWidget->OnEnterKey();
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
	case EShownWidget::MainMenu:
		focus = MainMenu;
		break;
	case EShownWidget::Inventory:
		focus = Inventory;
		break;
	case EShownWidget::Registered:
		ensure(currentShownRegisteredWidget);
		focus = currentShownRegisteredWidget;
		break;
	default:
		checkNoEntry();
	}

	check(focus);
	focus->AddToViewport();
	CurrentShownWidget = widget;
	updateState(focus);
	focus->WidgetShown();



}

void ATauCetiF2PlayerController::ToggleInventory()
{
	if (CurrentShownWidget != EShownWidget::None && CurrentShownWidget != EShownWidget::Inventory)
		return;

	if (CurrentShownWidget == EShownWidget::None)
		ShowWidget(EShownWidget::Inventory);
	else
	{
		while (CurrentShownWidget != EShownWidget::None)
		{
			OnEscapeKey();
		}
	}
}

void ATauCetiF2PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

FString ATauCetiF2PlayerController::EnsureRegisterWidget(TSubclassOf<UObjectWidget> widget)
{
	ensure(widget);

	auto widgDefaults = widget.GetDefaultObject();

	auto id = widgDefaults->WidgetID;
	ensure(id.Len() > 0);

	auto w = registeredWidgets.Find(id);

	if (!w)
	{
		auto widg = CreateWidget<UObjectWidget>(this, widget);

		widg->OnWidgetCloseRequest.AddDynamic(this, &ATauCetiF2PlayerController::OnEscapeKey);
		&registeredWidgets.Add(id, widg);
	}

	return id;
}

void ATauCetiF2PlayerController::ShowRegisteredWidget(const FString widgetID, ABlock* block)
{
	auto w = registeredWidgets.Find(widgetID);
	ensure(w);

	currentShownRegisteredWidget = *w;

	if (block)
	{
		currentShownRegisteredWidget->InitForBlock_Implementation(block);

		auto showable = Cast<IBlockWithShowableWidget>(block);
		if (showable)
			showable->SetDisplayedWidget(currentShownRegisteredWidget);
	}

	ShowWidget(EShownWidget::Registered);

}