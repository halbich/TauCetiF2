#include "Blocks.h"
#include "LightBlock.h"

ALightBlock::ALightBlock()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;

	LightBlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightBlockMesh"));

	LightBlockMesh->SetupAttachment(GetRootComponent());
	LightBlockMesh->AddLocalOffset(FVector(0, 0, -1));

	LightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light Component"));
	LightComp->SetupAttachment(GetRootComponent());
	LightComp->AddLocalOffset(FVector(0, 0, -4));

	ElectricityComponent = CreateDefaultSubobject<UElectricityComponent>(TEXT("ElectricityComponent"));
	AddOwnedComponent(ElectricityComponent);

	dayMultiplier = 86400.0f / GameDefinitions::GameDayLength;
}

UStaticMeshComponent* ALightBlock::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	if (BlockMeshStructureDefIndex == 0)
		return LightBlockMesh;

	return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
}

UPrimitiveComponent* ALightBlock::GetComponentForObjectOutline_Implementation() {
	return LightBlockMesh;
}

void ALightBlock::Tick(float DeltaSeconds)
{

	auto elapsedSeconds = DeltaSeconds * dayMultiplier;

	auto max = ElectricityComponent->GetDefinition()->MaxConsumedEnergyPerGameSecond;
	auto i = ElectricityComponent->GetInfo();
	
	auto powerConsumption = AutoregulatePowerOutput ? getAutoregulatedPower(i->GetRemainingPercentage(), max) : i->CurrentPowerConsumptionPerSec;
	updateLightByConsumption(powerConsumption,max);

	auto consumed = elapsedSeconds * powerConsumption;
}