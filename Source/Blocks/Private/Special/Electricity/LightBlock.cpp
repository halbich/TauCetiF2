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

void ALightBlock::BeginPlay() {
	Super::BeginPlay();
	
	auto inst = Cast<UTCF2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ensure(inst);
	inst->OnDaytimeChangedEvent.AddDynamic(this, &ALightBlock::OnNightChanged);
	OnNightChanged(inst->IsNightInGame);
}

void ALightBlock::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	auto inst = Cast<UTCF2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ensure(inst);
	inst->OnDaytimeChangedEvent.RemoveDynamic(this, &ALightBlock::OnNightChanged);

	Super::EndPlay(EndPlayReason);
}

void ALightBlock::Tick(float DeltaSeconds)
{
	auto elapsedSeconds = DeltaSeconds * dayMultiplier;

	auto max = ElectricityComponent->GetDefinition()->MaxConsumedEnergyPerGameSecond;
	auto i = ElectricityComponent->GetInfo();

	auto powerConsumption = AutoregulatePowerOutput ? getAutoregulatedPower(i->GetRemainingPercentage(), max) : i->CurrentPowerConsumptionPerSec;

	auto toObtain = elapsedSeconds * powerConsumption;

	float actuallyObtained = 0;
	if (ElectricityComponent->ObtainAmount(toObtain, actuallyObtained))
		toObtain -= actuallyObtained;

	ElectricityComponent->EnergyConsumed += actuallyObtained;

	updateLightByConsumption(actuallyObtained / elapsedSeconds, max);
}

void ALightBlock::OnNightChanged(bool isNight)
{
	isDaytime = !isNight;
	print(TEXT("Night changed"));
}


void ALightBlock::SetControlState_Implementation(bool isOn) {}
void ALightBlock::SetOutputPowerPercentage_Implementation(float percentage) {}
void ALightBlock::SetController_Implementation(ABlock* controller) {}
ABlock* ALightBlock::GetController_Implementation() { return NULL; }