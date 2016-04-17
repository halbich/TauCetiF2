

#include "TauCetiF2.h"
#include "WorldController.h"


AWorldController::AWorldController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	auto minCube = WorldHelpers::BorderToWorld(FVector(0, 0, 0));
	auto maxCube = WorldHelpers::BorderToWorld(GameDefinitions::WorldBorders);


	FVector min((minCube - 0.5 * FVector(1, 1, 1))* GameDefinitions::CubeMinSize);
	FVector max((maxCube + 0.5 * FVector(1, 1, 1))* GameDefinitions::CubeMinSize);

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

	actor->WorldObjectComponent->BlockInfo = block;

	if (addToRoot) {

		auto MinMax = NewObject<UKDTree>()->Init(box);
		MinMax->containingObject = actor;
		UE_LOG(LogTemp, Log, TEXT("---   Pøidávám do svìta objekt  %s"), *actor->GetName());
		RootBox->AddToTree(MinMax);
		MinMax->DEBUGDrawContainingBox(GetWorld());
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

void AWorldController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (RootBox && RootBox->IsValidLowLevel())
	{
		if (RootBox->SingleChild)
		{
			RootBox->SingleChild->ConditionalBeginDestroy();
			RootBox->SingleChild = nullptr;
		}

		if (RootBox->B1)
		{
			RootBox->B1->ConditionalBeginDestroy();
			RootBox->B1 = nullptr;
		}

		if (RootBox->B2)
		{
			RootBox->B2->ConditionalBeginDestroy();
			RootBox->B2 = nullptr;
		}
	}
}