#include "Blocks.h"
#include "CubeBlock.h"

ACubeBlockWithoutElectricity::ACubeBlockWithoutElectricity()
	: Super()
{
	PrimaryActorTick.bCanEverTick = false;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));

	CubeMesh->SetupAttachment(GetRootComponent());
}

UStaticMeshComponent* ACubeBlockWithoutElectricity::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	if (BlockMeshStructureDefIndex == 0)
		return CubeMesh;

	return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
}

UPrimitiveComponent* ACubeBlockWithoutElectricity::GetComponentForObjectOutline_Implementation() {
	return CubeMesh;
}