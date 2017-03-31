#include "Blocks.h"
#include "Block.h"

ABlock::ABlock()
	: Super()
{
	PrimaryActorTick.bCanEverTick = false;

	auto root = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	SetRootComponent(root);

	SelectTargetComponent = CreateDefaultSubobject<USelectTargetComponent>(TEXT("SelectTarget"));

	WorldObjectComponent = CreateDefaultSubobject<UWorldObjectComponent>(TEXT("WorldObjectComponent"));
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

		// TODO validační error
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
		auto scaleVect = BlockDef->HasCustomScaling ? FVector(1, 1, 1) : BlockInfo->Scale;
		electricityBlock->SetDefinition(BlockDef->ElectricityComponentDef, scaleVect, BlockInfo->Rotation);
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

	auto oxygenBlock = Cast<UOxygenComponent>(GetComponentByClass(UOxygenComponent::StaticClass()));
	if (oxygenBlock)
		info->OxygenInfo = oxygenBlock->SetInfo(info->OxygenInfo);

	auto electricityBlock = Cast<UElectricityComponent>(GetComponentByClass(UElectricityComponent::StaticClass()));
	if (electricityBlock)
		info->ElectricityInfo = electricityBlock->SetInfo(info->ElectricityInfo);
}

void ABlock::InitWorldObjectComponent()
{
	auto woc = WorldObjectComponent;

	ensure(woc->BuildingTree);

	FlushPersistentDebugLines(GetWorld());

	ensure(woc->DefiningBox);
	ensure(woc->TreeElements.Num() > 0);

	woc->RootBox = woc->TreeElements[0]->GetRootNode<UKDTree>(true);
	ensure(woc->RootBox != nullptr);

	auto surroundings = NewObject<UKDTree>()->Init(woc->DefiningBox, woc->RootBox);

	surroundings->DEBUGDrawSurrondings(GetWorld());

	// TODO

	woc->TreeElements[0]->DEBUGDrawSurrondings(GetWorld(), FColor::Black);

	auto def = Definition->GetDefaultObject<UBlockDefinition>();

	// potřebujeme okolí pro propojení elektriky nebo zapojení do patternů
	if (!def->HasElectricityComponent && !def->UsingInPatterns)
		return;

	TArray<UObject*> items;
	woc->TreeElements[0]->GetContainingObjectsFromBottom(surroundings, items, this);

	TArray<UMinMaxTree*> usedTrees;

	for (auto obj : items)
	{
		auto object = Cast<ABlock>(obj);

		check(object && object->IsValidLowLevelFast() && object->WorldObjectComponent && object->WorldObjectComponent->IsValidLowLevelFast());

		if (def->HasElectricityComponent && object->Definition.GetDefaultObject()->HasElectricityComponent)
		{
			auto ec = Cast<UElectricityComponent>(object->GetComponentByClass(UElectricityComponent::StaticClass()));
			check(ec);
			woc->SurroundingElectricityComponent.Add(ec);
		}

		if (def->UsingInPatterns && object->BlockInfo->ID == BlockInfo->ID) {

			ensure(object->WorldObjectComponent->BuildingTree);
			usedTrees.AddUnique(object->WorldObjectComponent->BuildingTree->GetRoot());
			print(*object->WorldObjectComponent->DefiningBox->ContainingObject->GetName());
		}
	}
	for (auto rootObj : usedTrees)
	{
		rootObj->Insert(woc->BuildingTree);
	}

	woc->BuildingTree->GetRoot()->DEBUGDrawBorder(GetWorld());
}

// *********** friend methods ********

bool CheckCommonBoundaries(UObject* o1, const UObject* o2)
{
	auto b1 = Cast<ABlock>(o1);
	ensure(b1);

	auto b2 = Cast<ABlock>(o2);
	ensure(b2);

	return UMinMaxBox::HasCommonBoundaries(b1->WorldObjectComponent->DefiningBox, b2->WorldObjectComponent->DefiningBox);
}

void AddToTreeElements(UObject* obj, UKDTree* box)
{
	auto b = Cast<ABlock>(obj);
	ensure(b);

	b->WorldObjectComponent->TreeElements.Add(box);
	b->WorldObjectComponent->OnTreeElementsChanged();
}

void RemoveFromTreeElements(UObject* obj, UKDTree* box)
{
	auto b = Cast<ABlock>(obj);
	ensure(b);

	b->WorldObjectComponent->TreeElements.Remove(box);
	b->WorldObjectComponent->OnTreeElementsChanged();
}

void AddToWeatherTreeElements(UObject* obj, UWeatherTargetsKDTree* box)
{
	auto b = Cast<ABlock>(obj);
	ensure(b);

	b->WorldObjectComponent->WeatherTreeElements.Add(box);
	b->WorldObjectComponent->OnWeatherTreeElementsChanged();
}

//bool CheckCommonBoundaries1(UObject* o1, const UObject* o2)
//{
//	auto b1 = Cast<ABlock>(o1);
//	ensure(b1);
//
//	auto b2 = Cast<ABlock>(o2);
//	ensure(b2);
//
//	return UMinMaxBox::HasCommonBoundaries(b1->WorldObjectComponent->DefiningBox, b2->WorldObjectComponent->DefiningBox);
//}
//
//
//void RemoveFromTreeElements(UObject* obj, UWeatherTargetsKDTree* box)
//{
//	auto b = Cast<ABlock>(obj);
//	ensure(b);
//
//	b->WorldObjectComponent->TreeElements.Remove(box);
//	b->WorldObjectComponent->OnTreeElementsChanged();
//}

void ABlock::WasHitByStorm(const FVector& blockLocation)
{
	
}

TArray<UElectricityComponent*> GetSurroundingComponents(UElectricityComponent* source)
{
	check(source);
	auto bl = Cast<ABlock>(source->GetOwner());
	check(bl && bl->WorldObjectComponent);

	auto result = TArray<UElectricityComponent*>( bl->WorldObjectComponent->SurroundingElectricityComponent);
	bl->WorldObjectComponent->SurroundingElectricityComponent.Empty();
	return result;
}

#pragma optimize("",on)