#include "TauCetiF2.h"
#include "ModalWidget.h"

bool UModalWidget::IsButtonEnabled(const EModalDialogResult buttonWithResult)
{
	return EnabledButtons.Contains(buttonWithResult);
}

void UModalWidget::SetDecisionMade(const EModalDialogResult decision)
{
	OnDecisionMade.Broadcast(decision);
	RequestClose();
	RemoveFromViewport();
}

void UModalWidget::SetDesignationYesNo()
{
	EnabledButtons.Empty();
	EnabledButtons.Add(EModalDialogResult::Yes);
	EnabledButtons.Add(EModalDialogResult::No);
}

void UModalWidget::SetDesignationOkCancel()
{
	EnabledButtons.Empty();
	EnabledButtons.Add(EModalDialogResult::OK);
	EnabledButtons.Add(EModalDialogResult::Cancel);
}

void UModalWidget::OnEnterKey()
{
	print(TEXT("Modal enter key"));

	if (IsButtonEnabled(EModalDialogResult::OK))
	{
		SetDecisionMade(EModalDialogResult::OK);
		return;
	}

	if (IsButtonEnabled(EModalDialogResult::Yes))
	{
		SetDecisionMade(EModalDialogResult::Yes);
		return;
	}
}

bool UModalWidget::OnEscapeKey()
{
	if (IsButtonEnabled(EModalDialogResult::Cancel))
	{
		SetDecisionMade(EModalDialogResult::Cancel);
		return false;
	}

	if (IsButtonEnabled(EModalDialogResult::No))
	{
		SetDecisionMade(EModalDialogResult::No);
		return false;
	}
	return false;
}