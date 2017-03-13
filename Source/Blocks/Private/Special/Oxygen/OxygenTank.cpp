#include "Blocks.h"
#include "OxygenTank.h"

AOxygenTank::AOxygenTank()
	: Super(), ListeningHandle()
{
	PrimaryActorTick.bCanEverTick = false;

	OxygenTankMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OxygenTankMesh"));
	OxygenTankMesh->SetupAttachment(GetRootComponent());

	OxygenComponent = CreateDefaultSubobject<UOxygenComponent>(TEXT("OxygenComponent"));
	AddOwnedComponent(OxygenComponent);
}

UStaticMeshComponent* AOxygenTank::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	if (BlockMeshStructureDefIndex == 0)
		return OxygenTankMesh;

	return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
}

UPrimitiveComponent* AOxygenTank::GetComponentForObjectOutline_Implementation() {
	return OxygenTankMesh;
}

void  AOxygenTank::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	SelectTargetComponent->EnableUse(500);
	SelectTargetComponent->CustomUsingMessage = NSLOCTEXT("TCF2LocSpace", "LC.PickupItem", "Vzít");

	FUseDelegate Subscriber;
	Subscriber.BindUObject(this, &AOxygenTank::ListeningOnUse);
	ListeningHandle = SelectTargetComponent->AddEventListener(Subscriber);
}

void AOxygenTank::ListeningOnUse(AActor* actor, bool isSpecial)
{
	print(TEXT("Getting Oxy tank!"));
	OnPickup(this);
}

void AOxygenTank::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ListeningHandle.IsValid() && SelectTargetComponent)
		SelectTargetComponent->RemoveEventListener(ListeningHandle);

	Super::EndPlay(EndPlayReason);
}