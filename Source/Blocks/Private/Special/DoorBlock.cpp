#include "Blocks.h"
#include "DoorBlock.h"

ADoorBlock::ADoorBlock()
	: Super()
{
	PrimaryActorTick.bCanEverTick = false;

	DoorBlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorBlockMesh"));

	DoorBlockMesh->SetupAttachment(GetRootComponent());
}

UStaticMeshComponent* ADoorBlock::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	if (BlockMeshStructureDefIndex == 0)
		return DoorBlockMesh;

	return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
}