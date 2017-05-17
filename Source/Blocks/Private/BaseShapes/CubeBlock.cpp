#include "Blocks.h"
#include "CubeBlock.h"

ACubeBlock::ACubeBlock()
	: Super()
{

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	CubeMesh->SetupAttachment(GetRootComponent());


	ElectricityComponent = CreateDefaultSubobject<UElectricityComponent>(TEXT("ElectricityComponent"));
	AddOwnedComponent(ElectricityComponent);
}

UStaticMeshComponent* ACubeBlock::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	if (BlockMeshStructureDefIndex == 0)
		return CubeMesh;

	return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
}

UPrimitiveComponent* ACubeBlock::GetComponentForObjectOutline_Implementation() {
	return CubeMesh;
}