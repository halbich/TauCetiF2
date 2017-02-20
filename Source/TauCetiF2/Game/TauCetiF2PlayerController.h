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

		if (anythingShown)
		{
			FInputModeGameAndUI mode;
			mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);

			if (focus)
				mode.SetWidgetToFocus(focus->TakeWidget());

			SetInputMode(mode);
		}
		else
		{
			FInputModeGameOnly mode;
			SetInputMode(mode);
		}

		bShowMouseCursor = anythingShown;
		SetIgnoreMoveInput(anythingShown);
		SetIgnoreLookInput(anythingShown);
	}
};
