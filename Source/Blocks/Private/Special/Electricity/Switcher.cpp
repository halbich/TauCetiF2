

#include "Blocks.h"
#include "Switcher.h"



ASwitcher::ASwitcher()
	: Super()
{

	SwitcherMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitcherMesh"));
	SwitcherMesh->SetupAttachment(GetRootComponent());

	ElectricityComponent = CreateDefaultSubobject<UElectricityComponent>(TEXT("ElectricityComponent"));
	AddOwnedComponent(ElectricityComponent);
}


UStaticMeshComponent* ASwitcher::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	switch (BlockMeshStructureDefIndex)
	{
	case 0:
		return SwitcherMesh;
	default:
		return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
	}
}

UPrimitiveComponent* ASwitcher::GetComponentForObjectOutline_Implementation() {
	return SwitcherMesh;
}

void  ASwitcher::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	SelectTargetComponent->EnableUse(500);
	SelectTargetComponent->CustomUsingMessage = NSLOCTEXT("TCF2LocSpace", "LC.Switcher.Use", "Pøepnout / Nastavit");

	FUseDelegate Subscriber;
	Subscriber.BindUObject(this, &ASwitcher::ListeningOnUse);
	ListeningHandle = SelectTargetComponent->AddEventListener(Subscriber);


	updateDynamicColor();
}

void ASwitcher::ListeningOnUse(AActor* actor, bool isSpecial)
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

	if (controlledBlocks.Num() == 0)
		return;

	IsOn = !IsOn;

	updateDynamicColor();

}


void ASwitcher::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ListeningHandle.IsValid() && SelectTargetComponent)
		SelectTargetComponent->RemoveEventListener(ListeningHandle);

	Super::EndPlay(EndPlayReason);
}

void ASwitcher::SetDisplayedWidget(UUserWidget* widget)
{
	shownWidget = widget;
}

UUserWidget* ASwitcher::GetShownWidget() { return shownWidget; }


bool ASwitcher::BindControl_Implementation(ABlock* controllableBlock)
{
	auto interf = Cast<IControllableBlock>(controllableBlock);
	if (!interf)
		return false;

	controlledBlocks.AddUnique(controllableBlock);
	updateDynamicColor();
	return true;
}

bool ASwitcher::UnbindControl_Implementation(ABlock* controllableBlock)
{
	auto interf = Cast<IControllableBlock>(controllableBlock);
	if (!interf)
		return false;

	auto res = controlledBlocks.Remove(controllableBlock) != 0;

	updateDynamicColor();

	return res;
}