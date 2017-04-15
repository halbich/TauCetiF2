#include "TauCetiF2.h"
#include "SwitcherWidget.h"

void USwitcherWidget::InitForBlock_Implementation(ABlock* block)
{
	Super::InitForBlock_Implementation(block);

	CurrentBlock = Cast<ASwitcher>(block);

	ensure(CurrentBlock);
}


void USwitcherWidget::GetControllables(TArray<ABlock*>& aviables, TArray<ABlock*>& controlled)
{
	auto n = CurrentBlock->ElectricityComponent->Network;

	for (auto con : n->ControllableBlocks)
	{
			auto c = Cast<ABlock>(con->GetOwner());
			if (!c || !c->IsValidLowLevel() || c->IsPendingKill())
				continue;

			aviables.Add(c);
	}
}