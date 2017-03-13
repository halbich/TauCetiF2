#include "Blocks.h"
#include "EnergyBlobBlock.h"

AEnergyBlobBlock::AEnergyBlobBlock()
	: Super(), ListeningHandle()
{
	PrimaryActorTick.bCanEverTick = false;

	BlobBlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlobBlockMesh"));
	BlobBlockMesh->SetupAttachment(GetRootComponent());
}

UStaticMeshComponent* AEnergyBlobBlock::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	switch (BlockMeshStructureDefIndex)
	{
	case 0:
		return BlobBlockMesh;
	default:
		return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
	}
}

UPrimitiveComponent* AEnergyBlobBlock::GetComponentForObjectOutline_Implementation() {
	return BlobBlockMesh;
}

void  AEnergyBlobBlock::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	SelectTargetComponent->EnableUse(500);
	SelectTargetComponent->CustomUsingMessage = NSLOCTEXT("TCF2LocSpace", "LC.PickupItem", "Vzít");

	FUseDelegate Subscriber;
	Subscriber.BindUObject(this, &AEnergyBlobBlock::ListeningOnUse);
	ListeningHandle = SelectTargetComponent->AddEventListener(Subscriber);
}

void AEnergyBlobBlock::ListeningOnUse(AActor* actor, bool isSpecial)
{
	print(TEXT("Getting Blob!"));
}

void AEnergyBlobBlock::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ListeningHandle.IsValid() && SelectTargetComponent)
		SelectTargetComponent->RemoveEventListener(ListeningHandle);

	Super::EndPlay(EndPlayReason);
}