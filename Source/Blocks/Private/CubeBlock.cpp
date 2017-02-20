#include "Blocks.h"
#include "CubeBlock.h"

ACubeBlock::ACubeBlock()
	: Super()
{
	PrimaryActorTick.bCanEverTick = false;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));

	CubeMesh->SetupAttachment(GetRootComponent());
}

UStaticMeshComponent* ACubeBlock::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	if (BlockMeshStructureDefIndex == 0)
		return CubeMesh;

	return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
}