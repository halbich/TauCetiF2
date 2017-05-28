#include "TauCetiF2.h"
#include "SynchronizeWidget.h"

void USynchronizeWidget::OnSynchronizeProperties_Implementation()
{
	// Override this event in Blueprint
}

void USynchronizeWidget::SynchronizeProperties() {
	Super::SynchronizeProperties();

	OnSynchronizeProperties();
}

void USynchronizeWidget::TutorialHighlight_Implementation(float showForTime)
{

}

void USynchronizeWidget::TutorialUnHighlight_Implementation()
{

}

void USynchronizeWidget::TutorialHide_Implementation() {}
void USynchronizeWidget::TutorialShow_Implementation() {}



void USynchronizeWidget::MakeHighlightScreen(TArray<USynchronizeWidget*> ToHighlight)
{
	for (auto h : hidden)
		if (h && h->IsValidLowLevel())
			h->TutorialShow();

	hidden.Empty();

	for (auto h : highlighted)
		if (h && h->IsValidLowLevel())
			h->TutorialUnHighlight();

	highlighted.Empty();


	for (auto w : ToHighlight)
	{
		AllTutorialScreenWidgets.AddUnique(w);
		w->TutorialHighlight(5);
		highlighted.AddUnique(w);
	}

	for (auto w : AllTutorialScreenWidgets)
	{
		if (highlighted.Find(w) == INDEX_NONE)
		{
			// we need to hide it
			w->TutorialHide();
			hidden.AddUnique(w);
		}
	}
}

void USynchronizeWidget::MakeHighlightScreenSingle(USynchronizeWidget* ToHighlight)
{
	TArray<USynchronizeWidget*> ha;
	ha.Add(ToHighlight);

	MakeHighlightScreen(ha);
}

void USynchronizeWidget::ShowHide(bool show, USynchronizeWidget* widget, UTextBlock* text)
{
	if (show)
	{
		widget->TutorialShow();
		text->SetOpacity(1.0f);
	}
	else
	{

		widget->TutorialHide();
		text->SetOpacity(0.25f);
	}
}