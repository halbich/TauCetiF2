#include "TauCetiF2.h"
#include "SwitcherWidget.h"



#pragma optimize("", off)

void USwitcherWidget::InitForBlock_Implementation(ABlock* block)
{
	Super::InitForBlock_Implementation(block);

	CurrentBlock = Cast<ASwitcher>(block);

	ensure(CurrentBlock);
}


void USwitcherWidget::GetControllables(TArray<ABlock*>& aviables, TArray<ABlock*>& controlled)
{

	if (!CurrentBlock)
		return;

	auto n = CurrentBlock->ElectricityComponent->Network;

	for (auto c : n->ControllableBlocks)
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

#pragma optimize("", on)