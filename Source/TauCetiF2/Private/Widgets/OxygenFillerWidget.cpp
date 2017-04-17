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

ABlock* UOxygenFillerWidget::GetControllables(TArray<ABlock*>& aviables)
{
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

UInventoryBuildableBlockInfo* UOxygenFillerWidget::GetItemsToBeFilled(TArray<UInventoryBuildableBlockInfo*>& aviables)
{

	for (auto con : InventoryComponent->InventoryItems)
	{
		aviables.Add(con);
	}


	aviables.Sort([](const UInventoryBuildableBlockInfo& A, const UInventoryBuildableBlockInfo& B) {
		return A.OxygenInfo->CurrentObjectOxygen < B.OxygenInfo->CurrentObjectOxygen;
	});

	return CurrentBlock->currentFillingItem;
}