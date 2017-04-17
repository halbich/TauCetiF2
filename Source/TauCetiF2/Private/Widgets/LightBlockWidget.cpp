#include "TauCetiF2.h"
#include "LightBlockWidget.h"

void ULightBlockWidget::InitForBlock_Implementation(ABlock* block)
{
	Super::InitForBlock_Implementation(block);

	CurrentBlock = Cast<ALightBlock>(block);

	ensure(CurrentBlock);

}

ABlock* ULightBlockWidget::GetControllables(TArray<ABlock*>& aviables)
{

	if (!CurrentBlock)
		return NULL;

	auto n = CurrentBlock->ElectricityComponent->Network;

	for (auto con : n->ControllerBlocks)
	{
		auto c = Cast<ABlock>(con->GetOwner());
		if (!c || !c->IsValidLowLevel() || c->IsPendingKill())
			continue;

		if (CurrentBlock->usedController == c)
			continue;

		aviables.Add(c);
	}

	return CurrentBlock->usedController;

}