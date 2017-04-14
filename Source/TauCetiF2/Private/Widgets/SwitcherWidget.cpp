

#include "TauCetiF2.h"
#include "SwitcherWidget.h"





void USwitcherWidget::InitForBlock_Implementation(ABlock* block)
{
	Super::InitForBlock_Implementation(block);


	CurrentBlock = Cast<ASwitcher>(block);

	ensure(CurrentBlock);

}