

#include "Blocks.h"
#include "OxygenTank.h"


AOxygenTank::AOxygenTank()
	: Super()
{
	PrimaryActorTick.bCanEverTick = false;

	OxygenTankMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OxygenTankMesh"));
	OxygenTankMesh->SetupAttachment(GetRootComponent());


}

UStaticMeshComponent* AOxygenTank::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	if (BlockMeshStructureDefIndex == 0)
		return OxygenTankMesh;

	return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
}

UPrimitiveComponent* AOxygenTank::GetComponentForObjectOutline_Implementation() {
	return OxygenTankMesh;
}

