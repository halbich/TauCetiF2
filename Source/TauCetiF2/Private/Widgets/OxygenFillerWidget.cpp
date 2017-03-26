

#include "TauCetiF2.h"
#include "OxygenFillerWidget.h"




void UOxygenFillerWidget::InitForBlock_Implementation(ABlock* block)
{
	Super::InitForBlock_Implementation(block);

	auto pc = Cast<ATauCetiF2Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	ensure(pc);

	InventoryComponent = Cast<UInventoryComponent>(pc->GetComponentByClass(UInventoryComponent::StaticClass()));

	ensure(InventoryComponent);

	CurrentBlock = Cast<AOxygenTankFillerBlock>(block);

	ensure(CurrentBlock);

}