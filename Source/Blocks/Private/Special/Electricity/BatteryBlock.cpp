#include "Blocks.h"
#include "BatteryBlock.h"

#pragma optimize("", off)

ABatteryBlock::ABatteryBlock()
	: Super()
{
	PrimaryActorTick.bCanEverTick = false;

	BatteryBlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BatteryBlockMesh"));
	BatteryBlockMesh->SetupAttachment(GetRootComponent());

	ElectricityComponent = CreateDefaultSubobject<UElectricityComponent>(TEXT("ElectricityComponent"));
	AddOwnedComponent(ElectricityComponent);
}

UStaticMeshComponent* ABatteryBlock::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	switch (BlockMeshStructureDefIndex)
	{
	case 0:
		return BatteryBlockMesh;
	default:
		return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
	}
}

UPrimitiveComponent* ABatteryBlock::GetComponentForObjectOutline_Implementation() {
	return BatteryBlockMesh;
}

void  ABatteryBlock::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	ElectricityComponent->OnComponentDataChangedEvent.AddDynamic(this, &ABatteryBlock::ListeningOnElectricityCompChanged);
	ElectricityComponent->onComponentDataChanged();
}

void ABatteryBlock::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ElectricityComponent)
		ElectricityComponent->OnComponentDataChangedEvent.RemoveDynamic(this, &ABatteryBlock::ListeningOnElectricityCompChanged);

	Super::EndPlay(EndPlayReason);
}

void ABatteryBlock::ListeningOnElectricityCompChanged(UBlockWithElectricityInfo* source)
{
	auto mat = Cast<UMaterialInstanceDynamic>(BatteryBlockMesh->GetMaterial(0));
	if (!mat)
		return;

	mat->SetScalarParameterValue(TEXT("Filling"), source->GetRemainingPercentageUnit());
}

#pragma optimize("", on)