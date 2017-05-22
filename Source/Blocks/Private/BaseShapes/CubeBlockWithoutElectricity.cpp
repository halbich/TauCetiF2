﻿#include "Blocks.h"
#include "CubeBlockWithoutElectricity.h"

ACubeBlockWithoutElectricity::ACubeBlockWithoutElectricity()
	: Super()
{
	PrimaryActorTick.bCanEverTick = false;

	GetRootComponent()->SetMobility(EComponentMobility::Static);

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	CubeMesh->SetupAttachment(GetRootComponent());
	CubeMesh->SetMobility(EComponentMobility::Static);

	CubeMeshOutline = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMeshOutline"));
	CubeMeshOutline->SetupAttachment(CubeMesh);
	CubeMeshOutline->SetRenderInMainPass(false);
	CubeMeshOutline->SetMobility(EComponentMobility::Static);
	CubeMeshOutline->SetCastShadow(false);
}

UStaticMeshComponent* ACubeBlockWithoutElectricity::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	if (BlockMeshStructureDefIndex == 0)
		return CubeMesh;

	return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
}

UPrimitiveComponent* ACubeBlockWithoutElectricity::GetComponentForObjectOutline_Implementation() {
	CubeMeshOutline->SetStaticMesh(CubeMesh->GetStaticMesh());
	return CubeMeshOutline;
}