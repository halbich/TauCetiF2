#include "TauCetiF2.h"
#include "SwitcherWidget.h"


void USwitcherWidget::InitForBlock_Implementation(ABlock* block)
{
	Super::InitForBlock_Implementation(block);

	CurrentBlock = Cast<ASwitcher>(block);
	ensure(CurrentBlock);

	if (CurrentBlock && CurrentBlock->IsValidLowLevel() && CurrentBlock->ElectricityComponent && CurrentBlock->ElectricityComponent->IsValidLowLevel())
		Network = CurrentBlock->ElectricityComponent->Network;
}

void USwitcherWidget::GetControllables(TArray<ABlock*>& aviables, TArray<ABlock*>& controlled)
{
	if (!CurrentBlock)
		return;


	for (auto c : Network->ControllableBlocks)
	{
		if (!c || !c->IsValidLowLevel() || c->IsPendingKill())
			continue;

		if (CurrentBlock->controlledBlocks.Contains(c))
			continue;

		aviables.Add(c);
	}

	for (auto con : CurrentBlock->controlledBlocks)
	{
		controlled.Add(con);
	}
}

