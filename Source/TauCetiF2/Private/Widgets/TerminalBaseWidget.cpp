#include "TauCetiF2.h"
#include "TerminalBaseWidget.h"


void UTerminalBaseWidget::InitForBlock_Implementation(ABlock* block)
{
	Super::InitForBlock_Implementation(block);

	auto terminal = Cast<ATerminalBlock>(block);
	if (terminal && terminal->IsValidLowLevel() && terminal->ElectricityComponent && terminal->ElectricityComponent->IsValidLowLevel())
		Network = terminal->ElectricityComponent->Network;

}