#include "TauCetiF2.h"
#include "LightBlockWidget.h"

void ULightBlockWidget::InitForBlock_Implementation(ABlock* block)
{
	Super::InitForBlock_Implementation(block);

	CurrentBlock = Cast<ALightBlock>(block);

	ensure(CurrentBlock);
}