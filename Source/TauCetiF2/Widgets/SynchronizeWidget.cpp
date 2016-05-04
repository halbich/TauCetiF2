

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

