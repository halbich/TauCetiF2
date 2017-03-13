#pragma once

#include "Block.h"
#include "PickableBlock.generated.h"

DECLARE_DELEGATE_OneParam(FPickupItemDelegate, ABlock*);
DECLARE_EVENT_OneParam(IPickableBlock, FPickupItemEvent, ABlock*);

UINTERFACE(BlueprintType)
class BLOCKS_API UPickableBlock : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 *
 */
class BLOCKS_API IPickableBlock
{
	GENERATED_IINTERFACE_BODY()

public:

	IPickableBlock()
	{
		MyPickupItemEvent = FPickupItemEvent();
	};

	void OnPickup(ABlock* source);

	FDelegateHandle AddPickupItemEventListener(FPickupItemDelegate& PickupItemDelegate);
	void RemovePickupItemEventListener(FDelegateHandle DelegateHandle);
private:
	FPickupItemEvent MyPickupItemEvent;
};
