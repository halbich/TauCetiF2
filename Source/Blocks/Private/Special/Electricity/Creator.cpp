

#include "Blocks.h"
#include "Creator.h"


ACreator::ACreator()
	: Super()
{

	CreatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CreatorMesh"));
	CreatorMesh->SetupAttachment(GetRootComponent());

	ElectricityComponent = CreateDefaultSubobject<UElectricityComponent>(TEXT("ElectricityComponent"));
	AddOwnedComponent(ElectricityComponent);
}


UStaticMeshComponent* ACreator::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	switch (BlockMeshStructureDefIndex)
	{
	case 0:
		return CreatorMesh;
	default:
		return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
	}
}

UPrimitiveComponent* ACreator::GetComponentForObjectOutline_Implementation() {
	return CreatorMesh;
}