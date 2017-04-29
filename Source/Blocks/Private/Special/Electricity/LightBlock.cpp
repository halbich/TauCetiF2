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

	ensure(BlockInfo->ID == LightSmallID);

	auto state = BlockInfo->BlockSpecificData.FindOrAdd(LightBlockConstants::IsAutoregulated);
	if (state.IsEmpty())
		BlockInfo->BlockSpecificData[LightBlockConstants::IsAutoregulated] = FString::FromInt((uint8)AutoregulatePowerOutput);
	else
		AutoregulatePowerOutput = FCString::Atoi(*state) > 0 ? true : false;

	auto state1 = BlockInfo->BlockSpecificData.FindOrAdd(LightBlockConstants::IsOn);
	if (state1.IsEmpty())
		BlockInfo->BlockSpecificData[LightBlockConstants::IsOn] = FString::FromInt((uint8)IsOn);
	else
		IsOn = FCString::Atoi(*state1) > 0 ? true : false;
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

	if (!usedController)
		this->Execute_SetControlState(this, !IsOn);
}

void  ALightBlock::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	SelectTargetComponent->EnableUse(500);
	updateUsingMessage();

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

	this->Execute_SetController(this, NULL);

	Super::EndPlay(EndPlayReason);
}

void ALightBlock::Tick(float DeltaSeconds)
{
	auto elapsedSeconds = DeltaSeconds * GameDefinitions::GameDayMultiplier;

	auto max = ElectricityComponent->GetDefinition()->MaxConsumedEnergyPerGameSecond;
	auto i = ElectricityComponent->GetInfo();

	auto powerConsumption = IsOn
		? (AutoregulatePowerOutput
			? getAutoregulatedPower(i->GetRemainingPercentage())
			: i->PowerConsumptionPercent)
		: 0;

	auto toObtain = elapsedSeconds * powerConsumption  * max;

	float actuallyObtained = 0;
	if (ElectricityComponent->ObtainAmount(toObtain, actuallyObtained))
		toObtain -= actuallyObtained;

	ElectricityComponent->EnergyConsumed += actuallyObtained;

	updateLightByConsumption(actuallyObtained / elapsedSeconds, max);
}

void ALightBlock::OnNightChanged(bool isNight) {
	isDaytime = !isNight;
	if (usedController == NULL) {
		this->Execute_SetControlState(this, isNight);
	}
}

void ALightBlock::SetControlState_Implementation(bool isOn) {
	IsOn = isOn;

	ensure(BlockInfo->ID == LightSmallID);
	BlockInfo->BlockSpecificData[LightBlockConstants::IsOn] = FString::FromInt((uint8)IsOn);
	updateUsingMessage();
}
void ALightBlock::SetOutputPowerPercentage_Implementation(float percentage) { BlockInfo->ElectricityInfo->PowerConsumptionPercent = percentage; }

void ALightBlock::SetController_Implementation(ABlock* controller) {
	if (usedController && usedController->IsValidLowLevel())
	{
		auto usedContTemp = usedController;
		usedController = NULL;
		auto controllable = Cast<IControllerBlock>(usedContTemp);
		ensure(controllable);

		controllable->Execute_UnbindControl(usedContTemp, this);
	}

	usedController = controller;

	if (usedController)
	{
		auto interf = Cast<IControllerBlock>(usedController);
		if (interf)
			this->Execute_SetControlState(this, interf->Execute_GetControlState(usedController));
	}
	else
		IsOn = true;

	updateUsingMessage();
}
ABlock* ALightBlock::GetController_Implementation() { return usedController; }

void ALightBlock::SetDisplayedWidget(UUserWidget* widget) { shownWidget = widget; }
UUserWidget* ALightBlock::GetShownWidget() { return shownWidget; }

void ALightBlock::ShowWidget_Implementation()
{
	auto def = Definition->GetDefaultObject<UBlockDefinition>();
	check(def);
	IBlockWithShowableWidget::CallShowWidget(this, def->UsableDef.ShowWidgetOnUse);
}

void ALightBlock::UpdateAutoregulate(bool newAutoregulate)
{
	AutoregulatePowerOutput = newAutoregulate;


	ensure(BlockInfo->ID == LightSmallID);
	BlockInfo->BlockSpecificData[LightBlockConstants::IsAutoregulated] = FString::FromInt((uint8)AutoregulatePowerOutput);
}