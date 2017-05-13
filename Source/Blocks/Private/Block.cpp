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

void ABlock::OnConstruction(const FTransform& Transform)
{
	if (!Definition)
	{
		error(FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.Block.NoDefinition", "Blok {0} nemá zadanou žádnou definici!."), FText::FromString(GetFullName())).ToString());
		return;
	}

	if (!BlockInfo) {
		Super::OnConstruction(Transform);
		return;
	}

	auto def = Definition->GetDefaultObject<UBlockDefinition>();

	auto currentScale = GetBlockScale();

	if (!BlockInfo->UnderConstruction) {
		auto baseHealth = def->HealthSize * buildingCoeficient(def);
		auto dimensions = def->GetMeshScale(currentScale);
		BlockInfo->MaxHealth = baseHealth * dimensions.X * dimensions.Y * dimensions.Z;

		if (BlockInfo->Health <= 0)
			BlockInfo->Health = BlockInfo->MaxHealth;

		BlockInfo->Health = FMath::Clamp(BlockInfo->Health, 0.0f, BlockInfo->MaxHealth);
		HealthUpdated();
	}

	auto genBlock = Cast<IGenericBlock>(this);
	int32 index = 0;

	for (auto structureDef : def->MeshStructure)
	{
		check(structureDef.Mesh && "Structure element has no Mesh assigned!");

		auto meshComp = genBlock->Execute_GetMeshStructureComponent(this, index++);

		if (!meshComp)
			continue;

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

	if (!BlockInfo->UnderConstruction) {
		if (def->ElectricityComponentDef.IsControlBlock)
		{
			if (!BlockInfo->RelationsInfo || !BlockInfo->RelationsInfo->IsValidLowLevel())
				BlockInfo->RelationsInfo = NewObject<UBlockWithRelationsInfo>();
		}
		else {
			ensure(!BlockInfo->RelationsInfo);
		}
	}

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
		auto scaleVect = BlockDef->HasCustomScaling ? FVector(1, 1, 1) : BlockInfo->Scale;
		oxygenBlock->SetDefinition(BlockDef->OxygenComponentDef, scaleVect, BlockInfo->Rotation);
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
	ensure(BlockInfo == NULL);
	BlockInfo = info;

	auto oxygenBlock = Cast<UOxygenComponent>(GetComponentByClass(UOxygenComponent::StaticClass()));
	if (oxygenBlock)
		info->OxygenInfo = oxygenBlock->SetInfo(info->OxygenInfo);

	auto electricityBlock = Cast<UElectricityComponent>(GetComponentByClass(UElectricityComponent::StaticClass()));
	if (electricityBlock)
	{
		ensure(Definition);
		auto def = Definition->GetDefaultObject<UBlockDefinition>();
		if (def->ElectricityComponentDef.IsControlBlock && !info->ElectricityInfo->PoweredBlockInfo)
		{
			info->ElectricityInfo->PoweredBlockInfo = NewObject<UPoweredBlockInfo>();
		}

		info->ElectricityInfo = electricityBlock->SetInfo(info->ElectricityInfo);

	}
	auto allowedAdditionals = GetSupportedAdditionals();

	TArray<FString> toDelete;
	for (auto kvp : BlockInfo->BlockSpecificData)
	{
		if (!allowedAdditionals.Contains(kvp.Key))
			toDelete.Add(kvp.Key);
	}

	for (auto k : toDelete)
		BlockInfo->BlockSpecificData.Remove(k);
}

void ABlock::InitWorldObjectComponent()
{
	auto woc = WorldObjectComponent;

	ensure(woc->DefiningBox);
	ensure(woc->TreeElements.Num() > 0);

	if (!woc->RootBox || !woc->RootBox->IsValidLowLevel())
		woc->RootBox = woc->TreeElements[0]->GetRootNode<UKDTree>(true);

	ensure(woc->RootBox != nullptr);

	auto def = Definition->GetDefaultObject<UBlockDefinition>();

	// potřebujeme okolí pro propojení elektriky nebo zapojení do patternů
	if (!def->HasElectricityComponent && !def->UsingInPatterns)
		return;

	auto surroundings = NewObject<UKDTree>()->Init(woc->DefiningBox, woc->RootBox);

	TArray<UObject*> items;
	woc->TreeElements[0]->GetContainingObjectsFromBottom(surroundings, items, this);

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
			auto pi = object->WorldObjectComponent->PatternGroupInfo;
			ensure(pi && pi->IsValidLowLevel());

			if (!woc->PatternGroupInfo) {
				// we don't have group yet, we can join it
				pi->RegisterBlock(this);
				woc->PatternGroupInfo = pi;
			}
			else
			{
				if (pi != woc->PatternGroupInfo) {
					// we need to merge groups
					for (auto i : pi->BlocksInGroup)
					{
						i->WorldObjectComponent->PatternGroupInfo = woc->PatternGroupInfo;

						woc->PatternGroupInfo->RegisterBlock(i);
					}

					pi->BlocksInGroup.Empty();
					woc->RootBox->TryUnregisterWatchingGroup(pi);
					pi->MarkPendingKill();
				}
			}
		}
	}

	if (def->UsingInPatterns && !woc->PatternGroupInfo)
	{
		auto pi = woc->PatternGroupInfo = GetPatternGroupImpl();
		pi->RegisterBlock(this);
	}
}

UPatternGroupInfo* ABlock::GetPatternGroupImpl() {
	return NewObject<UPatternGroupInfo>();
}

void ABlock::RenewPatternInfo()
{
	InitWorldObjectComponent();
}

void ABlock::WasHitByStorm(const FVector& blockHitLocation, const float amount)
{
	auto damage = amount;

	if (FMath::IsNearlyZero(damage))
		return;

	auto electricity = Cast<IBlockWithElectricity>(this);
	if (electricity)
		damage = electricity->WasHitByStorm(damage);

	if (FMath::IsNearlyZero(damage))
		return;

	auto healthDamage = damage * GameDefinitions::RainHitpointToEnergy * GameDefinitions::EnergyToHealth;

	BlockInfo->HealthDamageTaken += healthDamage;
	BlockInfo->Health = FMath::Clamp(BlockInfo->Health - healthDamage, 0.0f, BlockInfo->MaxHealth);

	HealthUpdated();

	if (BlockInfo->Health <= 0)
		OnDestroyRequestedEvent.Broadcast(this);
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

	b->WorldObjectComponent->TreeElements.AddUnique(box);
}

void RemoveFromTreeElements(UObject* obj, UKDTree* box)
{
	auto b = Cast<ABlock>(obj);
	ensure(b);

	auto rem = b->WorldObjectComponent->TreeElements.Remove(box);
	ensure(rem > 0);
}

void AddToWeatherTreeElements(UObject* obj, UWeatherTargetsKDTree* box)
{
	auto b = Cast<ABlock>(obj);
	ensure(b);

	b->WorldObjectComponent->WeatherTreeElements.AddUnique(box);
}

TArray<UElectricityComponent*> GetSurroundingComponents(UElectricityComponent* source)
{
	check(source);
	auto bl = Cast<ABlock>(source->GetOwner());
	check(bl && bl->WorldObjectComponent);

	auto result = TArray<UElectricityComponent*>(bl->WorldObjectComponent->SurroundingElectricityComponent);
	bl->WorldObjectComponent->SurroundingElectricityComponent.Empty();
	return result;
}

UBlockInfo* GetBlockInfoFromParent(UActorComponent* source)
{
	check(source);
	auto bl = Cast<ABlock>(source->GetOwner());
	check(bl && bl->BlockInfo);

	return bl->BlockInfo;
}

UMinMaxBox* GetDefiningBox(ABlock* block)
{
	return block->WorldObjectComponent->DefiningBox;
}

UTexture2D* ABlock::GetDefaultTexture()
{
	return Definition->GetDefaultObject<UBlockDefinition>()->BlockImage;
}

bool ABlock::GetIsController()
{
	auto d = Definition->GetDefaultObject<UBlockDefinition>();

	return d->HasElectricityComponent && d->ElectricityComponentDef.IsControlBlock && d->ElectricityComponentDef.IsController;
}

TArray<FString> ABlock::GetSupportedAdditionals()
{
	TArray<FString> result;
	return result;
}