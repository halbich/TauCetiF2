#include "TauCetiF2.h"
#include "TerminalBaseWidget.h"

void UTerminalBaseWidget::InitForBlock_Implementation(ABlock* block)
{
	Super::InitForBlock_Implementation(block);

	auto terminal = Cast<ATerminalBlock>(block);
	if (terminal && terminal->IsValidLowLevel() && terminal->ElectricityComponent && terminal->ElectricityComponent->IsValidLowLevel())
		Network = terminal->ElectricityComponent->Network;
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


FText UTerminalBaseWidget::GetDisplayTextExtended_Implementation()
{
	return BaseControlDisplayName;
}