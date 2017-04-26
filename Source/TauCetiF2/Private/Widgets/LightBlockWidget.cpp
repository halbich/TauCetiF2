#include "TauCetiF2.h"
#include "LightBlockWidget.h"

void ULightBlockWidget::InitForBlock_Implementation(ABlock* block)
{
	Super::InitForBlock_Implementation(block);

	CurrentBlock = Cast<ALightBlock>(block);
	ensure(CurrentBlock);

	if (CurrentBlock && CurrentBlock->IsValidLowLevel() && CurrentBlock->ElectricityComponent && CurrentBlock->ElectricityComponent->IsValidLowLevel())
		Network = CurrentBlock->ElectricityComponent->Network;
}

ABlock* ULightBlockWidget::GetControllables(TArray<ABlock*>& aviables)
{
	if (!CurrentBlock)
		return NULL;

	for (auto c : Network->ControllerBlocks)
	{
		if (!c || !c->IsValidLowLevel() || c->IsPendingKill())
			continue;

		if (CurrentBlock->usedController == c)
			continue;

		aviables.Add(c);
	}

	return CurrentBlock->usedController;
}