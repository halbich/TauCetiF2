#include "Blocks.h"
#include "LightBlock.h"

ALightBlock::ALightBlock()
	: Super(), ListeningHandle()
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


}

void ALightBlock::ListeningOnUse(AActor* actor, bool isSpecial)
{
	if (!actor || !actor->IsValidLowLevel())
		return;

	if (!isSpecial)
	{
		auto def = Definition->GetDefaultObject<UBlockDefinition>();
		check(def);
		IBlockWithShowableWidget::CallShowWidget(this, def->UsableDef.ShowWidgetOnUse);
		return;
	}
}

void  ALightBlock::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	SelectTargetComponent->EnableUse(500);
	SelectTargetComponent->CustomUsingMessage = NSLOCTEXT("TCF2LocSpace", "LC.LightBlock.Use", "Ovládat / Nastavit");

	FUseDelegate Subscriber;
	Subscriber.BindUObject(this, &ALightBlock::ListeningOnUse);
	ListeningHandle = SelectTargetComponent->AddEventListener(Subscriber);

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

	if (ListeningHandle.IsValid() && SelectTargetComponent)
		SelectTargetComponent->RemoveEventListener(ListeningHandle);

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

void ALightBlock::SetController_Implementation(ABlock* controller) { usedController = controller; AutoregulatePowerOutput = !controller; }
ABlock* ALightBlock::GetController_Implementation() { return usedController; }

void ALightBlock::SetDisplayedWidget(UUserWidget* widget) { shownWidget = widget; }
UUserWidget* ALightBlock::GetShownWidget() { return shownWidget; }

void ALightBlock::ShowWidget_Implementation()
{
	auto def = Definition->GetDefaultObject<UBlockDefinition>();
	check(def);
	IBlockWithShowableWidget::CallShowWidget(this, def->UsableDef.ShowWidgetOnUse);
}