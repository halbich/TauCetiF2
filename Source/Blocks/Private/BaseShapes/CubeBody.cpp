#include "Blocks.h"
#include "CubeBody.h"

ACubeBody::ACubeBody()
	: Super()
{
	PrimaryActorTick.bCanEverTick = false;

	GetRootComponent()->SetMobility(EComponentMobility::Static);

	CubeBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeBodyMesh"));
	CubeBodyMesh->SetupAttachment(GetRootComponent());
	CubeBodyMesh->SetMobility(EComponentMobility::Static);

	ElectricityComponent = CreateDefaultSubobject<UElectricityComponent>(TEXT("ElectricityComponent"));
	AddOwnedComponent(ElectricityComponent);
}

UStaticMeshComponent* ACubeBody::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	if (BlockMeshStructureDefIndex == 0)
		return CubeBodyMesh;

	return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
}

UPrimitiveComponent* ACubeBody::GetComponentForObjectOutline_Implementation() {
	return CubeBodyMesh;
}