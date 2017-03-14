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

	OxygenComponent->OnComponentDataChangedEvent.AddDynamic(this, &AOxygenTank::ListeningOnOxygenCompChanged);
	OxygenComponent->onComponentDataChanged();
}

void AOxygenTank::ListeningOnUse(AActor* actor, bool isSpecial)
{
	if (!actor || !actor->IsValidLowLevel())
		return;

	if (!isSpecial)
	{
		IPickableBlock::OnPickup(this);
		return;
	}

	auto actorOxygen = Cast<UOxygenComponent>(actor->GetComponentByClass(UOxygenComponent::StaticClass()));
	if (!actorOxygen)
		return;

	if (FMath::IsNearlyZero(OxygenComponent->OxygenInfo->CurrentFillingValue))		// nemáme z èeho bychom brali
		return;

	float actuallyPutted = 0.0f;
	float actuallyObtained = 0.0f;
	bool obtainResult = false;
	if (actorOxygen->PutAmount(OxygenComponent->OxygenInfo->CurrentFillingValue, actuallyPutted))
	{
		obtainResult = OxygenComponent->ObtainAmount(actuallyPutted, actuallyObtained);
		check(obtainResult && FMath::IsNearlyZero(actuallyObtained - actuallyPutted));
	}
}

void AOxygenTank::ListeningOnOxygenCompChanged(UBlockWithOxygenInfo* source)
{
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

	if (OxygenComponent)
		OxygenComponent->OnComponentDataChangedEvent.RemoveDynamic(this, &AOxygenTank::ListeningOnOxygenCompChanged);

	Super::EndPlay(EndPlayReason);
}