#include "Blocks.h"
#include "BatteryBlock.h"

ABatteryBlock::ABatteryBlock()
	: Super()
{
	PrimaryActorTick.bCanEverTick = false;

	BatteryBlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BatteryBlockMesh"));
	BatteryBlockMesh->SetupAttachment(GetRootComponent());
}

UStaticMeshComponent* ABatteryBlock::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	switch (BlockMeshStructureDefIndex)
	{
	case 0:
		return BatteryBlockMesh;
	default:
		return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
	}
}

UPrimitiveComponent* ABatteryBlock::GetComponentForObjectOutline_Implementation() {
	return BatteryBlockMesh;
}