#include "Blocks.h"
#include "Block.h"

ABlock::ABlock()
	: Super()
{
	PrimaryActorTick.bCanEverTick = false;

	auto root = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	SetRootComponent(root);

	SelectTargetComponent = CreateDefaultSubobject<USelectTargetComponent>(TEXT("SelectTarget"));
}

#pragma optimize("", off)

void  ABlock::OnConstruction(const FTransform& Transform)
{
	if (!Definition)
	{
		// TODO Localization!
		auto errorText = TEXT("No definition used for block ") + GetFullName();
		error(*errorText);
		return;
	}

	if (!BlockInfo) {
		Super::OnConstruction(Transform);
		return;
	}

	auto genBlock = Cast<IGenericBlock>(this);
	auto def = Definition->GetDefaultObject<UBlockDefinition>();

	auto currentScale = GetBlockScale();

	int32 index = 0;

	for (auto structureDef : def->MeshStructure)
	{
		// TODO Localization!
		check(structureDef.Mesh && "Structure element has no Mesh assigned!");

		auto meshComp = genBlock->Execute_GetMeshStructureComponent(this, index++);

		// TODO validaèní error
		if (!meshComp)
			continue;

		// TODO Localization!
		check(meshComp && "Function GetMeshStructureComponent returned NULL!");
		meshComp->SetStaticMesh(structureDef.Mesh);

		for (size_t i = 0; i < structureDef.Materials.Num(); i++)
		{
			auto matDef = structureDef.Materials[i];

			setMaterial(meshComp, BlockInfo->UnderConstruction ? matDef.TranslucentMat : matDef.DefaultMat, i, matDef.GetParams(currentScale));
		}
	}

	auto targetCompo = genBlock->Execute_GetComponentForObjectOutline(this);
	if (targetCompo)
		SelectTargetComponent->EnableSelect(targetCompo);

	genBlock->Execute_UpdateBlockOnConstruction(this, def);

	Super::OnConstruction(Transform);
}

void ABlock::BeginPlay() {
	Super::BeginPlay();
}

void ABlock::EndPlay(const EEndPlayReason::Type EndPlayReasonType) {
	Super::EndPlay(EndPlayReasonType);
}

UStaticMeshComponent* ABlock::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) {
	return NULL;
}

UPrimitiveComponent* ABlock::GetComponentForObjectOutline_Implementation() {
	return NULL;
}

void ABlock::UpdateBlockOnConstruction_Implementation(UBlockDefinition* BlockDef)
{
	auto oxygenBlock = Cast<UOxygenComponent>(GetComponentByClass(UOxygenComponent::StaticClass()));
	if (oxygenBlock)
	{
		check(BlockDef->HasOxygenComponent);	// musíme mít definici
		oxygenBlock->SetDefinition(BlockDef->OxygenComponentDef);
	}
	else
	{
		check(!BlockDef->HasOxygenComponent);	// nemáme komponentu -> nesmíme mít definici
	}

	auto electricityBlock = Cast<UElectricityComponent>(GetComponentByClass(UElectricityComponent::StaticClass()));
	if (electricityBlock)
	{
		check(BlockDef->HasElectricityComponent);	// musíme mít definici
		electricityBlock->SetDefinition(BlockDef->ElectricityComponentDef);
	}
	else
	{
		check(!BlockDef->HasElectricityComponent);	// nemáme komponentu -> nesmíme mít definici
	}
}

FVector ABlock::GetBlockScale()
{
	return GetActorScale3D();
}

void ABlock::SetBlockInfo(UBlockInfo* info)
{
	BlockInfo = info;

	if (BlockInfo->UnderConstruction)
		return;

	auto oxygenBlock = Cast<UOxygenComponent>(GetComponentByClass(UOxygenComponent::StaticClass()));
	if (oxygenBlock)
		info->OxygenInfo = oxygenBlock->SetInfo(info->OxygenInfo);

	auto electricityBlock = Cast<UElectricityComponent>(GetComponentByClass(UElectricityComponent::StaticClass()));
	if (electricityBlock)
		info->ElectricityInfo = electricityBlock->SetInfo(info->ElectricityInfo);
}

#pragma optimize("",on)