#pragma once

#include "Commons/Public/Enums.h"
#include "Widgets/ObjectWidget.h"
#include "ModalWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModalDialogResult, EModalDialogResult, DialogResult);

/**
 *
 */
UCLASS()
class TAUCETIF2_API UModalWidget : public UObjectWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Transient, Category = "CustomWidgets|ModalWidget")
		FModalDialogResult OnDecisionMade;

	UPROPERTY(Transient)
		TArray<EModalDialogResult> EnabledButtons;

	UFUNCTION(BlueprintCallable, Category = "CustomWidgets|ModalWidget")
		bool IsButtonEnabled(const EModalDialogResult buttonWithResult);

	UFUNCTION(BlueprintCallable, Category = "CustomWidgets|ModalWidget")
		void SetDecisionMade(const EModalDialogResult decision);

	UFUNCTION(BlueprintCallable, Category = "CustomWidgets|ModalWidget")
		void SetDesignationYesNo();

	UFUNCTION(BlueprintCallable, Category = "CustomWidgets|ModalWidget")
		void SetDesignationOkCancel();

	virtual void OnEnterKey() override;

	virtual bool OnEscapeKey() override;
};
