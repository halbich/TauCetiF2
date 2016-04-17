

#include "TauCetiF2.h"
#include "WorldController.h"


AWorldController::AWorldController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	auto minCube = UHelpers::BorderToWorld(FVector(0, 0, 0));
	auto maxCube = UHelpers::BorderToWorld(UHelpers::WorldBorders);


	FVector min((minCube - 0.5 * FVector(1, 1, 1))* UHelpers::CubeMinSize);
	FVector max((maxCube + 0.5 * FVector(1, 1, 1))* UHelpers::CubeMinSize);

	RootBox = CreateDefaultSubobject<UKDTree>(TEXT("RootBox"));
	RootBox->Init(min, max, 0);

}



void AWorldController::LoadBlocksArray(TArray<UBlockInfo*>& blocks) {

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

	auto classBP = GetClassByShape(*definition);

	ensure(classBP != nullptr);


	auto trans = UHelpers::GetSpawnTransform(block->Location, block->Scale, block->Rotation);
	if (!IsValidSpawnPoint(trans))
	{
		UE_LOG(LogTemp, Error, TEXT("Objekt nelze korektnì pøidat do stromu. Vynechávám."));
		IsValidSpawnPoint(trans);
		return nullptr;
	}

	auto actor = world->SpawnActorDeferred<AWorldObject>(classBP, trans);

	if (!actor)
	{
		UE_LOG(LogTemp, Error, TEXT("Neznámé ID (%d) objektu. Vynechávám"), block->ID);
		ensure(actor != nullptr);			// chceme vyhodit chybu
		return nullptr;
	}

	actor->WorldObjectComponent->BlockInfo = block;

	if (addToRoot) {

		auto MinMax = UKDTree::FromWorldObject(actor);
		MinMax->DEBUGDrawContainingBox(GetWorld());
		UE_LOG(LogTemp, Log, TEXT("---   Pøidávám do svìta objekt  %s"), *actor->GetName());
		RootBox->AddToTree(MinMax);
		//UE_LOG(LogTemp, Log, TEXT("---   Pøidávám do svìta objekt  %s"), *actor->GetName());
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