#include "Blocks.h"
#include "OxygenTankFillerBlock.h"

AOxygenTankFillerBlock::AOxygenTankFillerBlock()
	: Super()
{
	PrimaryActorTick.bCanEverTick = false;

	OxygenTankFillerBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OxygenTankFillerBodyMesh"));
	OxygenTankFillerBodyMesh->SetupAttachment(GetRootComponent());

	OxygenTankFillerHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OxygenTankFillerHeadMesh"));
	OxygenTankFillerHeadMesh->SetupAttachment(OxygenTankFillerBodyMesh);

	OxygenComponent = CreateDefaultSubobject<UOxygenComponent>(TEXT("OxygenComponent"));
	AddOwnedComponent(OxygenComponent);

	ElectricityComponent = CreateDefaultSubobject<UElectricityComponent>(TEXT("ElectricityComponent"));
	AddOwnedComponent(ElectricityComponent);
}

UStaticMeshComponent* AOxygenTankFillerBlock::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	switch (BlockMeshStructureDefIndex)
	{
	case 0:
		return OxygenTankFillerBodyMesh;
	case 1:
		return OxygenTankFillerHeadMesh;
	default:
		return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
	}
}

UPrimitiveComponent* AOxygenTankFillerBlock::GetComponentForObjectOutline_Implementation() {
	return OxygenTankFillerBodyMesh;
}

void  AOxygenTankFillerBlock::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	SelectTargetComponent->EnableUse(500);
	SelectTargetComponent->CustomUsingMessage = NSLOCTEXT("TCF2LocSpace", "LC.OxygenTankFillerBlock.Pickup", "Použít / Otevřít konzoli");

	FUseDelegate Subscriber;
	Subscriber.BindUObject(this, &AOxygenTankFillerBlock::ListeningOnUse);
	ListeningHandle = SelectTargetComponent->AddEventListener(Subscriber);

	OxygenComponent->OnComponentDataChangedEvent.AddDynamic(this, &AOxygenTankFillerBlock::ListeningOnOxygenCompChanged);
	OxygenComponent->onComponentDataChanged();
}

void AOxygenTankFillerBlock::ListeningOnUse(AActor* actor, bool isSpecial)
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

	auto actorOxygen = Cast<UOxygenComponent>(actor->GetComponentByClass(UOxygenComponent::StaticClass()));
	if (!actorOxygen)
		return;

	auto i = OxygenComponent->GetInfo();

	if (FMath::IsNearlyZero(i->CurrentObjectOxygen))		// nemáme z čeho bychom brali
		return;

	float actuallyPutted = 0.0f;
	float actuallyObtained = 0.0f;
	bool obtainResult = false;
	if (actorOxygen->PutAmount(i->CurrentObjectOxygen, actuallyPutted))
	{
		obtainResult = OxygenComponent->ObtainAmount(actuallyPutted, actuallyObtained);
		check(obtainResult && FMath::IsNearlyZero(actuallyObtained - actuallyPutted));
	}
}

void AOxygenTankFillerBlock::ListeningOnOxygenCompChanged(UBlockWithOxygenInfo* source)
{
}

void AOxygenTankFillerBlock::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ListeningHandle.IsValid() && SelectTargetComponent)
		SelectTargetComponent->RemoveEventListener(ListeningHandle);

	if (OxygenComponent)
		OxygenComponent->OnComponentDataChangedEvent.RemoveDynamic(this, &AOxygenTankFillerBlock::ListeningOnOxygenCompChanged);

	Super::EndPlay(EndPlayReason);
}

void AOxygenTankFillerBlock::SetDisplayedWidget(UUserWidget* widget)
{
	shownWidget = widget;
}

UUserWidget* AOxygenTankFillerBlock::GetShownWidget() { return shownWidget; }

void AOxygenTankFillerBlock::ShowWidget_Implementation()
{
	auto def = Definition->GetDefaultObject<UBlockDefinition>();
	check(def);
	IBlockWithShowableWidget::CallShowWidget(this, def->UsableDef.ShowWidgetOnUse);
}


void AOxygenTankFillerBlock::SetControlState_Implementation(bool isOn) {}
void AOxygenTankFillerBlock::SetOutputPowerPercentage_Implementation(float percentage) {}

void AOxygenTankFillerBlock::SetController_Implementation(ABlock* controller) { usedController = controller; }
ABlock* AOxygenTankFillerBlock::GetController_Implementation() { return usedController; }