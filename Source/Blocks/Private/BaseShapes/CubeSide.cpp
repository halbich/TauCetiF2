#include "Blocks.h"
#include "CubeSide.h"

ACubeSide::ACubeSide()
	: Super()
{
	PrimaryActorTick.bCanEverTick = false;

	CubeSideMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeSideMesh"));

	CubeSideMesh->SetupAttachment(GetRootComponent());

	ElectricityComponent = CreateDefaultSubobject<UElectricityComponent>(TEXT("ElectricityComponent"));
	AddOwnedComponent(ElectricityComponent);
}

UStaticMeshComponent* ACubeSide::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	if (BlockMeshStructureDefIndex == 0)
		return CubeSideMesh;

	return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
}

UPrimitiveComponent* ACubeSide::GetComponentForObjectOutline_Implementation() {
	return CubeSideMesh;
}