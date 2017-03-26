#pragma once

#include "Commons/Public/Enums.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/ObjectWidget.h"
#include "Widgets/MainMenuWidget.h"
#include "Widgets/InventoryScreen.h"
#include "Helpers/Helpers.h"
#include "GameSave/Public/SaveGameCarrier.h"
#include "TauCetiF2PlayerController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameShouldBePaused, bool, NewPaused);


/**
 *
 */
UCLASS()
class TAUCETIF2_API ATauCetiF2PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient)
		EShownWidget CurrentShownWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UObjectWidget> wInGameMenu;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Widgets")
		UObjectWidget* InGameMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UObjectWidget> wBaseControl;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Widgets")
		UObjectWidget* BaseControl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UMainMenuWidget> wMainMenu;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Widgets")
		UMainMenuWidget* MainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UInventoryScreen> wInventory;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Widgets")
		UInventoryScreen* Inventory;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Widgets")
		FGameShouldBePaused OnGameShouldBePaused;


	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void OnEnterKey();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void OnEscapeKey();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void ShowWidget(const EShownWidget widget);

	// Override BeginPlay()
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void ToggleInventory();

	FORCEINLINE bool NothingShown()
	{
		return CurrentShownWidget == EShownWidget::None;
	}

private:

	FORCEINLINE void updateState(UObjectWidget* focus = nullptr) {
		auto anythingShown = CurrentShownWidget != EShownWidget::None;

		bShowMouseCursor = anythingShown;
		bEnableClickEvents = anythingShown;
		bEnableMouseOverEvents = anythingShown;
		SetIgnoreMoveInput(anythingShown);
		SetIgnoreLookInput(anythingShown);

		if (anythingShown)
		{
			FInputModeGameAndUI mode;
			mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);

			if (focus)
				mode.SetWidgetToFocus(focus->TakeWidget());

			SetInputMode(mode);

			// https://answers.unrealengine.com/questions/69251/disabling-mouse-cursor-requires-extra-click-to-re.html
			auto CurrentFocus = FSlateApplication::Get().GetKeyboardFocusedWidget();
			FSlateApplication::Get().ClearKeyboardFocus(EKeyboardFocusCause::SetDirectly);
			FSlateApplication::Get().SetKeyboardFocus(CurrentFocus);
		}
		else
		{
			FInputModeGameOnly mode;
			SetInputMode(mode);
		}

		OnGameShouldBePaused.Broadcast(CurrentShownWidget == EShownWidget::InGameMenu);
	}
};
