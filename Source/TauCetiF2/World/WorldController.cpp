

#include "TauCetiF2.h"
#include "EngineUtils.h"
#include "Blocks/Public/Block.h"
#include "AssetRegistryModule.h"
#include "WorldController.h"


#pragma optimize("", off)

AWorldController::AWorldController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	BaseControl = ObjectInitializer.CreateDefaultSubobject<UBaseControlComponent>(this, TEXT("Base Control"));



}


void AWorldController::LoadBlocksArray(UPARAM(ref)TArray<UBlockInfo*>& blocks) {

	auto world = GetWorld();
	if (!world)
		return;


	auto children = ABlock::StaticClass()->Children;


	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);
	TArray<FAssetData> AssetData;
	FARFilter Filter;
	//Filter.ClassNames.Add(ABlock::StaticClass()->GetFName());
	Filter.PackagePaths.Add("/Game/Blocks/");
	Filter.bRecursiveClasses = true;
	Filter.bRecursivePaths = true;
	AssetRegistryModule.Get().GetAssets(Filter, AssetData);

	for (auto a : AssetData)
	{
		print(TEXT("Found block"));
		print(*a.ObjectPath.ToString());
	}

	for (TObjectIterator<ABlock> Itr; Itr; ++Itr)
	{
		auto o = Itr;

		auto _world = o->GetWorld();

		if (_world == world)
		{
			print(TEXT("Found block"));
			print(*Itr->GetName());
		}
	}

	UsedBlocks.Empty();
	UsedBlocks.Reserve(blocks.Num());
	for (auto block : blocks)
	{
		SpawnWorldObject(world, block, true);
	}

}

bool AWorldController::DestroyWorldObject(AWorldObject* object)
{
	if (!object || !object->IsValidLowLevel() || object->IsPendingKill())
		return false;

	auto count = UsedBlocks.Remove(object->WorldObjectComponent->BlockInfo);
	check(count == 1 && "Failed to remove block info.");
	object->Destroy();

	if (debugBoxesShown) {
		DEBUGHideMinMaxBoxes();
		DEBUGShowMinMaxBoxes();
	}

	check(RootBox && RootBox->IsValidLowLevel() && !RootBox->IsPendingKill());

	return true;
}

// TODO inline!
AWorldObject* AWorldController::SpawnWorldObject(UWorld* world, UBlockInfo* block, bool addToRoot)
{
	if (!block)
		return nullptr;

	check(RootBox != nullptr);

	auto definition = FBlockDefinitionHolder::Instance().GetDefinition(block->ID, false);

	if (!definition)
	{
		UE_LOG(LogTemp, Error, TEXT("Neznámé ID (%d) objektu. Vynechávám"), block->ID);
		return nullptr;
	}


	UMinMaxBox* box = BlockHelpers::GetSpawnBox(definition, block);
	ensure(box != nullptr);
	if (!IsValidSpawnPoint(box))
	{
		UE_LOG(LogTemp, Error, TEXT("Objekt nelze korektnì pøidat do stromu. Vynechávám."));
		return nullptr;
	}

	FString invalidReason;
	bool isValid = BlockHelpers::CheckBlockValidity(definition, block, invalidReason);
	if (!isValid)
	{
		UE_LOG(LogTemp, Error, TEXT("Blok není validní. Dùvod: %s. nechávám"), *invalidReason);
		return nullptr;
	}

	auto classBP = BlockHelpers::GetClassByShape(definition);

	ensure(classBP != nullptr);

	auto trans = BlockHelpers::GetSpawnTransform(definition, block);
	auto actor = world->SpawnActorDeferred<AWorldObject>(classBP, trans, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	if (!actor)
	{
		UE_LOG(LogTemp, Error, TEXT("Neznámé ID (%d) objektu. Vynechávám"), block->ID);
		ensure(actor != nullptr);			// chceme vyhodit chybu
		return nullptr;
	}

	actor->SetBlockInfo(block, definition);

	if (addToRoot) {

		auto MinMax = NewObject<UKDTree>()->Init(box);
		MinMax->ContainingObject = actor;
		UE_LOG(LogTemp, Log, TEXT("---   Pøidávám do svìta objekt  %s"), *actor->GetName());


		actor->WorldObjectComponent->UpdateDefiningBox(MinMax);
		RootBox->AddToTree(MinMax);
		for (auto usedBox : actor->WorldObjectComponent->TreeElements)
		{
			check(usedBox->GetRootNode<UKDTree>() == RootBox && TEXT("Used box don't have RootBox as ROOT !"));
			check(usedBox->ContainingObject == actor && TEXT("Used box has another ContainingObject than it should have!"));
		}

		//MinMax->DEBUGDrawContainingBox(GetWorld());


	}

	UGameplayStatics::FinishSpawningActor(actor, trans);


	if (addToRoot)
	{
		UsedBlocks.Add(block);
		if (debugBoxesShown) {
			DEBUGHideMinMaxBoxes();
			DEBUGShowMinMaxBoxes();
		}
	}

	return actor;


}


void AWorldController::DEBUGShowMinMaxBoxes() {

	if (RootBox)
	{
		debugBoxesShown = true;
		RootBox->DEBUGDrawContainingBox(GetWorld());
	}
	else
		print(TEXT("NO Root!"));
}


void AWorldController::DEBUGHideMinMaxBoxes() {

	if (RootBox)
	{
		FlushPersistentDebugLines(GetWorld());
		debugBoxesShown = false;
		//
	}
	else
		print(TEXT("NO Root!"));
}


void AWorldController::DEBUGSpawnPatterns(const FVector & startingPoint)
{

	print(TEXT("DEBUGSpawnPatterns"));

	auto blocks = UPatternDefinitionsHolder::Instance()->DEBUGSpawnPatterns(startingPoint);
	for (auto block : blocks)
	{
		SpawnWorldObject(GetWorld(), block, true);
	}

}

void AWorldController::DEBUGUsedPatternElements(const FVector & startingPoint)
{
	print(TEXT("DEBUGUsedPatternElements"));

	auto blocks = UPatternDefinitionsHolder::Instance()->DEBUGUsedPatternElements(startingPoint);
	for (auto block : blocks)
	{
		SpawnWorldObject(GetWorld(), block, true);
	}

}

void AWorldController::BeginPlay() {
	print(TEXT("Cpp WC BP"));

	RootBox = NewObject<UKDTree>(GetTransientPackage(), TEXT("RootBox"));


	auto minCube = WorldHelpers::BorderToWorld(FVector(0, 0, 0));
	auto maxCube = WorldHelpers::BorderToWorld(GameDefinitions::WorldBorders);


	FVector min((minCube - 0.5 * FVector(1, 1, 1))* GameDefinitions::CubeMinSize);
	FVector max((maxCube + 0.5 * FVector(1, 1, 1))* GameDefinitions::CubeMinSize);

	RootBox->Init(min, max, 0);

	UPatternDefinitionsHolder::Instance();



	TArray<UObject*> MeshAssets;
	EngineUtils::FindOrLoadAssetsByPath(TEXT("/Game/BuildingObjects/Meshes/"), MeshAssets, EngineUtils::ATL_Class);
	for (auto asset : MeshAssets) {
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, *asset->GetFullName());
		UStaticMesh* mesh = Cast<UStaticMesh>(asset);
		if (mesh != nullptr) {
			// do something with the mesh, create a component with it, etc.
		}
	}


	Super::BeginPlay();
}


void AWorldController::EndPlay(const EEndPlayReason::Type EndPlayReasonType)
{
	for (auto block : UsedBlocks)
	{

	}

	Super::EndPlay(EndPlayReasonType);
}

#pragma optimize("", on)