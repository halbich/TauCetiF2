

#include "TauCetiF2.h"
#include "WorldController.h"


AWorldController::AWorldController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	auto minCube = UHelpers::BorderToWorld(FVector(0, 0, 0));
	auto maxCube = UHelpers::BorderToWorld(UHelpers::WorldBorders);

	FVector min((minCube.X - 0.5) * UHelpers::CubeMinSize, (minCube.Y - 0.5) * UHelpers::CubeMinSize, (minCube.Z-0.5)*UHelpers::CubeMinSize);
	FVector max((maxCube.X + 0.5) * UHelpers::CubeMinSize, (maxCube.Y + 0.5) * UHelpers::CubeMinSize, (maxCube.Z + 0.5)*UHelpers::CubeMinSize);

	RootBox = new FMinMaxBox(min, max, 0);
	RootBox->name = TEXT("ROOT");

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

		auto definition = FBlockDefinitionHolder::Instance().GetDefinition(block->ID, false);
		if (!definition)
		{
			UE_LOG(LogTemp, Error, TEXT("Neznámé ID (%d) objektu. Vynechávám"), block->ID);
			continue;
		}

		auto classBP = GetClassByShape(*definition);
		if (classBP) {

			auto trans = UHelpers::GetSpawnTransform(block->Location, block->Scale);

			trans.SetRotation(FQuat::FQuat(block->Rotation));

			auto actor = world->SpawnActorDeferred<AWorldObject>(classBP, trans);

			if (actor)
			{
				actor->WorldObjectComponent->BlockInfo = block;

				UE_LOG(LogTemp, Log, TEXT("---   Pøidávám do svìta objekt  %s"), *actor->GetName());


				if (ensure(RootBox != nullptr))
					RootBox->AddToTree(&FMinMaxBox::FromWorldObject(actor));

				UE_LOG(LogTemp, Log, TEXT("---   Pøidávám do svìta objekt  %s"), *actor->GetName());

				UGameplayStatics::FinishSpawningActor(actor, trans);
			}


		}

		UsedBlocks.Add(block);
	}

}

void AWorldController::DEBUGShowMinMaxBoxes() {

	if (RootBox)
		RootBox->DEBUGDrawContainingBox(GetWorld());
	else
		print(TEXT("NO Root!"));
}
