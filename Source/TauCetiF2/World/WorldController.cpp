

#include "TauCetiF2.h"
#include "WorldController.h"

AWorldController::AWorldController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	RootBox = new FMinMaxBox(FVector(-500, -500, -10), FVector(500, 500, 150));
	
}

AWorldController::~AWorldController() {
	
	if (RootBox)
	{
		delete RootBox;
		RootBox = nullptr;
	}
}


void AWorldController::LoadBlocksArray(TArray<UBlockInfo*>& blocks) {

	auto world = GetWorld();
	if (!world)
		return;

	UsedBlocks.Empty();
	UsedBlocks.Reserve(blocks.Num());
	for (auto block : blocks)
	{
		if (!block)
			continue;


		auto classBP = GetClassByShape(block->ShapeType);
		if (classBP) {

			auto trans = UHelpers::GetSpawnTransform(block->Location, block->BlockScale);

			trans.SetRotation(FQuat::FQuat(block->BlockRotation));

			auto actor = world->SpawnActorDeferred<AWorldObject>(classBP, trans);

			if (actor)
			{
				actor->WorldObjectComponent->BlockInfo = block;
				UGameplayStatics::FinishSpawningActor(actor, trans);
			}


		}

		UsedBlocks.Add(block);
	}

}

void AWorldController::DEBUGShowMinMaxBoxes() {
	print(TEXT("DEBUG Boxes!"));

	if (RootBox)
		RootBox->DEBUGDrawContainingBox(GetWorld());
	else
		print(TEXT("NO Root!"));
}
