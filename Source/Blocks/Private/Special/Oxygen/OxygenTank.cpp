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

void AOxygenTank::SetBlockInfo(UBlockInfo* info)
{
	Super::SetBlockInfo(info);

	ensure(BlockInfo->ID == OxygenTankID);

	BlockInfo->BlockSpecificData.FindOrAdd(OxygenTankConstants::ItemTags);

}

TArray<FString> AOxygenTank::GetSupportedAdditionals()
{
	TArray<FString> result;
	result.Add(OxygenTankConstants::ItemTags);
	return result;
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

	if (FMath::IsNearlyZero(OxygenComponent->OxygenInfo->CurrentObjectOxygen))		// nemáme z čeho bychom brali
		return;

	float actuallyPutted = 0.0f;
	float actuallyObtained = 0.0f;
	bool obtainResult = false;
	if (actorOxygen->PutAmount(OxygenComponent->OxygenInfo->CurrentObjectOxygen, actuallyPutted))
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

	mat->SetScalarParameterValue(TEXT("Filling"), source->GetRemainingPercentageUnit());
}

void AOxygenTank::BeginPlay()
{
	Super::BeginPlay();

	SelectTargetComponent->EnableUse(500);
	SelectTargetComponent->CustomUsingMessage = NSLOCTEXT("TCF2LocSpace", "LC.OxygenTank.Pickup", "Doplnit kyslík / Sebrat");

	FUseDelegate Subscriber;
	Subscriber.BindUObject(this, &AOxygenTank::ListeningOnUse);
	ListeningHandle = SelectTargetComponent->AddEventListener(Subscriber);

	OxygenComponent->OnComponentDataChangedEvent.AddDynamic(this, &AOxygenTank::ListeningOnOxygenCompChanged);
	OxygenComponent->onComponentDataChanged();
}

void AOxygenTank::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ListeningHandle.IsValid() && SelectTargetComponent)
		SelectTargetComponent->RemoveEventListener(ListeningHandle);

	if (OxygenComponent)
		OxygenComponent->OnComponentDataChangedEvent.RemoveDynamic(this, &AOxygenTank::ListeningOnOxygenCompChanged);

	Super::EndPlay(EndPlayReason);
}

void AOxygenTank::PropagateInventoryBuildableTags(FString tags)
{
	BlockInfo->BlockSpecificData[OxygenTankConstants::ItemTags] = tags;
}

FString AOxygenTank::GetInventoryBuildableTags()
{
	return BlockInfo->BlockSpecificData.FindOrAdd(OxygenTankConstants::ItemTags);
}