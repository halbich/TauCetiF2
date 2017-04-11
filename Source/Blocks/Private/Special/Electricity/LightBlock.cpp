#include "Blocks.h"
#include "LightBlock.h"

ALightBlock::ALightBlock()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;

	LightBlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightBlockMesh"));

	LightBlockMesh->SetupAttachment(GetRootComponent());
	LightBlockMesh->AddLocalOffset(FVector(0, 0, -1));

	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	Light->SetupAttachment(GetRootComponent());
	Light->AddLocalOffset(FVector(0, 0, -4));

	ElectricityComponent = CreateDefaultSubobject<UElectricityComponent>(TEXT("ElectricityComponent"));
	AddOwnedComponent(ElectricityComponent);
}

UStaticMeshComponent* ALightBlock::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	if (BlockMeshStructureDefIndex == 0)
		return LightBlockMesh;

	return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
}

UPrimitiveComponent* ALightBlock::GetComponentForObjectOutline_Implementation() {
	return LightBlockMesh;
}