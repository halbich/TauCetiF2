

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

	UsedBlocks.Empty();
	UsedBlocks.Reserve(blocks.Num());
	for (auto block : blocks)
	{
		if (!block)
			continue;

		print(TEXT("block"));

		UsedBlocks.Add(block);
	}

}
