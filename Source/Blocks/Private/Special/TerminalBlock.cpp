

#include "Blocks.h"
#include "TerminalBlock.h"




ATerminalBlock::ATerminalBlock()
	: Super()
{
	PrimaryActorTick.bCanEverTick = false;

	TerminalBlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TerminalBlockMesh"));

	TerminalBlockMesh->SetupAttachment(GetRootComponent());

}


UStaticMeshComponent* ATerminalBlock::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	if (BlockMeshStructureDefIndex == 0)
		return TerminalBlockMesh;

	return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
}