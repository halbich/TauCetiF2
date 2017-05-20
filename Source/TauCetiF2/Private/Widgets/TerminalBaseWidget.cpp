#include "TauCetiF2.h"
#include "TerminalBaseWidget.h"

void UTerminalBaseWidget::InitForBlock_Implementation(ABlock* block)
{
	Super::InitForBlock_Implementation(block);

	Terminal = Cast<ATerminalBlock>(block);
	ensure(Terminal);

	if (Terminal && Terminal->IsValidLowLevel() && Terminal->ElectricityComponent && Terminal->ElectricityComponent->IsValidLowLevel())
		Network = Terminal->ElectricityComponent->Network;
}

void UTerminalBaseWidget::GetControllables(TArray<ABlock*>& aviable)
{
	for (auto con : Network->ElectricityProducers)
	{
		auto c = Cast<ABlock>(con->GetOwner());
		if (!c || !c->IsValidLowLevel() || c->IsPendingKill())
			continue;

		aviable.AddUnique(c);
	}

	for (auto con : Network->ElectricityConsumers)
	{
		auto c = Cast<ABlock>(con->GetOwner());
		if (!c || !c->IsValidLowLevel() || c->IsPendingKill())
			continue;

		aviable.AddUnique(c);
	}
}

UTerminalSelectorBaseWidget* UTerminalBaseWidget::GetSelectorWidget_Implementation()
{
	return NULL;
}