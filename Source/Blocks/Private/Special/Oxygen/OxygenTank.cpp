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
	SelectTargetComponent->CustomUsingMessage = NSLOCTEXT("TCF2LocSpace", "LC.PickupItem", "Použít / Sebrat");

	FUseDelegate Subscriber;
	Subscriber.BindUObject(this, &AOxygenTank::ListeningOnUse);
	ListeningHandle = SelectTargetComponent->AddEventListener(Subscriber);

	FOxygenComponentDataChangedDelegate OxygenSubscriber;
	OxygenSubscriber.BindUObject(this, &AOxygenTank::ListeningOnOxygenCompChanged);
	OxygenDataChangedHandle = OxygenComponent->AddEventListener(OxygenSubscriber);
}

void AOxygenTank::ListeningOnUse(AActor* actor, bool isSpecial)
{
	print(TEXT("using Oxy tank!"));

	if (!isSpecial)
		IPickableBlock::OnPickup(this);

	if (isSpecial)
		print(TEXT("Deplenishing Oxy tank!"));
	else
		print(TEXT("Getting Oxy tank!"));
}

void AOxygenTank::ListeningOnOxygenCompChanged(UBlockWithOxygenInfo* source)
{
	print(TEXT("data changed"));

	auto mat = Cast<UMaterialInstanceDynamic>(OxygenTankMesh->GetMaterial(0));
	if (!mat)
		return;

	auto def = OxygenComponent->GetDefinition();
	if (!def)
		return;



	mat->SetScalarParameterValue(TEXT("Filling"), def->TotalObjectVolume > 0 ? source->CurrentFillingValue / def->TotalObjectVolume : 0.0f);
}


void AOxygenTank::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ListeningHandle.IsValid() && SelectTargetComponent)
		SelectTargetComponent->RemoveEventListener(ListeningHandle);

	if (OxygenDataChangedHandle.IsValid() && OxygenComponent)
		OxygenComponent->RemoveEventListener(OxygenDataChangedHandle);

	Super::EndPlay(EndPlayReason);
}