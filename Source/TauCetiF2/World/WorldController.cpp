

#include "TauCetiF2.h"
#include "WorldController.h"

AWorldController::AWorldController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	auto minCube = UHelpers::BorderToWorld(FVector(0, 0, 0));
	print(minCube.ToString());

	auto maxCube = UHelpers::BorderToWorld(UHelpers::WorldBorders);
	print(maxCube.ToString());

	FVector min((minCube.X - 0.5) * UHelpers::CubeMinSize, (minCube.Y - 0.5) * UHelpers::CubeMinSize, (minCube.Z-0.5)*UHelpers::CubeMinSize);
	FVector max((maxCube.X + 0.5) * UHelpers::CubeMinSize, (maxCube.Y + 0.5) * UHelpers::CubeMinSize, (maxCube.Z + 0.5)*UHelpers::CubeMinSize);

	RootBox = new FMinMaxBox(min, max);

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

				if (ensure(RootBox != nullptr))
					RootBox->AddToTree(FMinMaxBox::FromWorldObject(actor));


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
