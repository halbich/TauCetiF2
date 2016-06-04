

#pragma once

#include "Common/Enums.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/ObjectWidget.h"
#include "Widgets/MainMenuWidget.h"
#include "Widgets/InventoryScreen.h"
#include "Helpers/Helpers.h"
#include "Common/SaveGameCarrier.h"
#include "TauCetiF2PlayerController.generated.h"




/**
 *
 */
UCLASS()
class TAUCETIF2_API ATauCetiF2PlayerController : public APlayerController
{
	GENERATED_BODY()


public:

	UPROPERTY()
		EShownWidget CurrentShownWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UObjectWidget> wInGameMenu;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		UObjectWidget* InGameMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UObjectWidget> wBaseControl;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		UObjectWidget* BaseControl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UMainMenuWidget> wMainMenu;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		UMainMenuWidget* MainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UInventoryScreen> wInventory;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		UInventoryScreen* Inventory;

	// Override BeginPlay()
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void OnEnterKey();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void OnEscapeKey();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void ShowWidget(const EShownWidget widget);

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
			mode.SetLockMouseToViewport(true);

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
