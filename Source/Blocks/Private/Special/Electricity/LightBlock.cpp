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

	AutoregulatePowerOutput = true;
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



void ALightBlock::SetBlockInfo(UBlockInfo* info)
{
	Super::SetBlockInfo(info);



	/*auto state = info->BlockSpecificData.FindOrAdd(DoorBlockConstants::DoorState);
	if (state.IsEmpty())
	{
		doorState = EDoorState::Opened;
		BlockInfo->BlockSpecificData[DoorBlockConstants::DoorState] = FString::FromInt((uint8)doorState);
	}
	else {
		doorState = (EDoorState)FCString::Atoi(*state);
	}

	auto setYaw = 90 * openingConstant;

	auto yaw = BlockInfo->BlockSpecificData.FindOrAdd(DoorBlockConstants::DoorYaw);
	if (yaw.IsEmpty())
		BlockInfo->BlockSpecificData[DoorBlockConstants::DoorYaw] = FString::SanitizeFloat(setYaw);
	else
		setYaw = FCString::Atof(*yaw);

	auto transf = DoorBlockMesh->GetRelativeTransform();
	transf.SetRotation(transf.Rotator().Add(0, setYaw, 0).Quaternion());
	updateDoorState(transf, openingConstant);*/
}

void ALightBlock::ListeningOnUse(AActor* actor, bool isSpecial)
{
	/*if (doorState == EDoorState::Closed)
	{
		doorState = EDoorState::Opening;
		return;
	}
	else if (doorState == EDoorState::Opened)
		doorState = EDoorState::Closing;*/
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
	auto elapsedSeconds = DeltaSeconds * GameDefinitions::GameDayMultiplier;

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

void ALightBlock::OnNightChanged(bool isNight) { isDaytime = !isNight; }

void ALightBlock::SetControlState_Implementation(bool isOn) {}
void ALightBlock::SetOutputPowerPercentage_Implementation(float percentage) {}
void ALightBlock::SetController_Implementation(ABlock* controller) {}
ABlock* ALightBlock::GetController_Implementation() { return NULL; }

void ALightBlock::SetDisplayedWidget(UUserWidget* widget) { shownWidget = widget; }

UUserWidget* ALightBlock::GetShownWidget() { return shownWidget; }