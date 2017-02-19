

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
		auto errorText = TEXT("No definition used for block ") + GetFullName();
		error(*errorText);
		return;
	}

	auto genBlock = Cast<IGenericBlock>(this);
	auto def = Definition->GetDefaultObject<UBlockDefinition>();

	auto currentScale = GetBlockScale();

	int32 index = 0;

	for (auto structureDef : def->MeshStructure)
	{

		check(structureDef.Mesh && "Structure element has no Mesh assigned!");

		auto meshComp = genBlock->Execute_GetMeshStructureComponent(this, index++);


		// TODO
		if (!meshComp)
			continue;

		check(meshComp && "Function GetMeshStructureComponent returned NULL!");
		meshComp->SetStaticMesh(structureDef.Mesh);

		for (size_t i = 0; i < structureDef.Materials.Num(); i++)
		{
			auto matDef = structureDef.Materials[i];

			setMaterial(meshComp, matDef.DefaultMat, i, matDef.GetParams(currentScale));
		}

	}

	this->UpdateBlockOnConstruction(def);

	Super::OnConstruction(Transform);
}

void ABlock::BeginPlay() {


	//IGenericBlock* genBlock = Cast<IGenericBlock>(this);
	//if (genBlock)
	//{
	//	//Don't call your functions directly, use the 'Execute_' prefix
	//	//the Execute_ReactToHighNoon and Execute_ReactToMidnight are generated on compile
	//	//you may need to compile before these functions will appear
	//	genBlock->Execute_GetMeshStructureComponent(this, 0);
	//}


	Super::BeginPlay();
}

void ABlock::EndPlay(const EEndPlayReason::Type EndPlayReasonType) {

	Super::EndPlay(EndPlayReasonType);
}


UStaticMeshComponent* ABlock::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	print(TEXT("base impl"));

	return NULL;

}


FVector ABlock::GetBlockScale()
{
	return GetActorScale3D();
}

void ABlock::SetBlockInfo(UBlockInfo* info)
{
	BlockInfo = info;
}

#pragma optimize("",on)