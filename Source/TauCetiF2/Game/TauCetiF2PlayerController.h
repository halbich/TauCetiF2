

#pragma once

#include "Common/Enums.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/ObjectWidget.h"
#include "Helpers/Helpers.h"
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

	UPROPERTY()
		UObjectWidget* InGameMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UObjectWidget> wBaseControl;

	UPROPERTY()
		UObjectWidget* BaseControl;

	// Override BeginPlay()
	virtual void BeginPlay() override;

	void OnEnterKey();

	UFUNCTION()
		void OnEscapeKey();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void ShowWidget(const EShownWidget widget);

private:


	FORCEINLINE void updateState(UObjectWidget* focus = nullptr) {

		auto anythingShown = CurrentShownWidget != EShownWidget::None;

		if (anythingShown)
		{
			FInputModeGameAndUI mode;
			mode.SetLockMouseToViewport(true);
			mode.SetHideCursorDuringCapture(true);
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
