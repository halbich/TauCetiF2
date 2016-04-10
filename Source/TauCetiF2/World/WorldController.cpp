

#include "TauCetiF2.h"
#include "WorldController.h"


// Sets default values
AWorldController::AWorldController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWorldController::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWorldController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
