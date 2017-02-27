#include "Blocks.h"
#include "CubeBody.h"

ACubeBody::ACubeBody()
	: Super()
{
	PrimaryActorTick.bCanEverTick = false;

	CubeBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeBodyMesh"));

	CubeBodyMesh->SetupAttachment(GetRootComponent());
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
