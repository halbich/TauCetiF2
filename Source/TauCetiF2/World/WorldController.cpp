

#include "TauCetiF2.h"
#include "WorldController.h"


AWorldController::AWorldController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	auto minCube = WorldHelpers::BorderToWorld(FVector(0, 0, 0));
	auto maxCube = WorldHelpers::BorderToWorld(GameDefinitions::WorldBorders);


	FVector min((minCube - 0.5 * FVector(1, 1, 1))* GameDefinitions::CubeMinSize);
	FVector max((maxCube + 0.5 * FVector(1, 1, 1))* GameDefinitions::CubeMinSize);

	RootBox = ObjectInitializer.CreateDefaultSubobject<UKDTree>(this, TEXT("RootBox"));
	RootBox->Init(min, max, 0);

}

void AWorldController::preLoadInit(bool ctor)
{
	if (RootBox && RootBox->IsValidLowLevel())
		return;

	auto minCube = WorldHelpers::BorderToWorld(FVector(0, 0, 0));
	auto maxCube = WorldHelpers::BorderToWorld(GameDefinitions::WorldBorders);


	FVector min((minCube - 0.5 * FVector(1, 1, 1))* GameDefinitions::CubeMinSize);
	FVector max((maxCube + 0.5 * FVector(1, 1, 1))* GameDefinitions::CubeMinSize);

	RootBox = ctor ? CreateDefaultSubobject<UKDTree>(TEXT("RootBox")) : NewObject<UKDTree>();;
	RootBox->Init(min, max, 0);
}

void AWorldController::PreLoadInit()
{
	preLoadInit(false);

	UPatternDefinitionsHolder::Instance();
}

void AWorldController::LoadBlocksArray(UPARAM(ref)TArray<UBlockInfo*>& blocks) {

	auto world = GetWorld();
	if (!world)
		return;

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

	FString invalidReason;
	bool isValid = BlockHelpers::CheckBlockValidity(definition, block, invalidReason);
	if (!isValid)
	{
		UE_LOG(LogTemp, Error, TEXT("Blok není validní. Dùvod: %s. nechávám"), *invalidReason);
		return nullptr;
	}



	UMinMaxBox* box = BlockHelpers::GetSpawnBox(definition, block);
	ensure(box != nullptr);
	if (!IsValidSpawnPoint(box))
	{
		UE_LOG(LogTemp, Error, TEXT("Objekt nelze korektnì pøidat do stromu. Vynechávám."));
		return nullptr;
	}

	auto classBP = BlockHelpers::GetClassByShape(definition);

	ensure(classBP != nullptr);

	auto trans = BlockHelpers::GetSpawnTransform(definition, block);
	auto actor = world->SpawnActorDeferred<AWorldObject>(classBP, trans);

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
