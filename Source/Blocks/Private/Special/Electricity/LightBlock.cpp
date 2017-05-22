#include "Blocks.h"
#include "LightBlock.h"

ALightBlock::ALightBlock()
	: Super(), ListeningHandle()
{
	PrimaryActorTick.bCanEverTick = true;

	GetRootComponent()->SetMobility(EComponentMobility::Static);

	LightBlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightBlockMesh"));
	LightBlockMesh->SetupAttachment(GetRootComponent());
	LightBlockMesh->AddLocalOffset(FVector(0, 0, -1));
	LightBlockMesh->SetMobility(EComponentMobility::Static);

	LightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light Component"));
	LightComp->SetupAttachment(GetRootComponent());
	LightComp->AddLocalOffset(FVector(0, 0, -4));
	LightComp->SetMobility(EComponentMobility::Movable);

	ElectricityComponent = CreateDefaultSubobject<UElectricityComponent>(TEXT("ElectricityComponent"));
	AddOwnedComponent(ElectricityComponent);
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

	PoweredBlockInfo = ElectricityComponent->ElectricityInfo->PoweredBlockInfo;
	ensure(PoweredBlockInfo);

	updateUsingMessage();
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
		Cast<IControllableBlock>(this)->Execute_SetControlState(this, !PoweredBlockInfo->IsOn);
}

void  ALightBlock::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);


}

void ALightBlock::BeginPlay()
{
	Super::BeginPlay();

	SelectTargetComponent->EnableUse(500);
	updateUsingMessage();

	FUseDelegate Subscriber;
	Subscriber.BindUObject(this, &ALightBlock::ListeningOnUse);
	ListeningHandle = SelectTargetComponent->AddEventListener(Subscriber);

}

void ALightBlock::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ListeningHandle.IsValid() && SelectTargetComponent)
		SelectTargetComponent->RemoveEventListener(ListeningHandle);

	Cast<IControllableBlock>(this)->Execute_SetController(this, NULL);

	Super::EndPlay(EndPlayReason);
}

void ALightBlock::Tick(float DeltaSeconds)
{
	auto elapsedSeconds = DeltaSeconds * GameDefinitions::GameDayMultiplier;

	auto max = ElectricityComponent->GetDefinition()->MaxConsumedEnergyPerGameSecond;
	auto i = ElectricityComponent->GetInfo();

	auto toObtain = elapsedSeconds * i->GetPowerOutput()  * max;

	float actuallyObtained = 0;
	if (ElectricityComponent->ObtainAmount(toObtain, actuallyObtained))
		toObtain -= actuallyObtained;

	ElectricityComponent->EnergyConsumed += actuallyObtained;

	updateLightByConsumption(actuallyObtained / elapsedSeconds, max);
}

void ALightBlock::SetControlState_Implementation(bool isOn) {
	ensure(BlockInfo->ID == LightSmallID);
	PoweredBlockInfo->IsOn = isOn;
	updateUsingMessage();
}
void ALightBlock::SetOutputPowerPercentage_Implementation(float percentage) {
	ensure(BlockInfo->ID == LightSmallID);
	PoweredBlockInfo->PowerConsumptionPercent = percentage;
}

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
			Cast<IControllableBlock>(this)->Execute_SetControlState(this, interf->Execute_GetControlState(usedController));
	}

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

void ALightBlock::SetAutoregulatePower(bool newAutoregulatePower)
{
	ensure(BlockInfo->ID == LightSmallID);
	PoweredBlockInfo->AutoregulatePower = newAutoregulatePower;
}