#include "Blocks.h"
#include "PickableBlock.h"

UPickableBlock::UPickableBlock(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void IPickableBlock::OnPickup(ABlock* source)
{
	MyPickupItemEvent.Broadcast(source);
}

FDelegateHandle IPickableBlock::AddPickupItemEventListener(FPickupItemDelegate& PickupItemDelegate)
{
	return MyPickupItemEvent.Add(PickupItemDelegate);
}

void IPickableBlock::RemovePickupItemEventListener(FDelegateHandle DelegateHandle)
{
	MyPickupItemEvent.Remove(DelegateHandle);
}

