﻿#include "Blocks.h"
#include "TerminalBlock.h"

ATerminalBlock::ATerminalBlock()
	: Super(), ListeningHandle()
{
	PrimaryActorTick.bCanEverTick = false;

	GetRootComponent()->SetMobility(EComponentMobility::Stationary);

	TerminalBlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TerminalBlockMesh"));
	TerminalBlockMesh->SetupAttachment(GetRootComponent());
	TerminalBlockMesh->SetMobility(EComponentMobility::Stationary);

	TerminalBlockMeshOutline = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TerminalBlockMeshOutline"));
	TerminalBlockMeshOutline->SetupAttachment(TerminalBlockMesh);
	TerminalBlockMeshOutline->SetRenderInMainPass(false);
	TerminalBlockMeshOutline->SetMobility(EComponentMobility::Stationary);
	TerminalBlockMeshOutline->SetCastShadow(false);

	ElectricityComponent = CreateDefaultSubobject<UElectricityComponent>(TEXT("ElectricityComponent"));
	AddOwnedComponent(ElectricityComponent);
}

UStaticMeshComponent* ATerminalBlock::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	if (BlockMeshStructureDefIndex == 0)
		return TerminalBlockMesh;

	return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
}

void  ATerminalBlock::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);


}

void ATerminalBlock::ListeningOnUse(AActor* actor, bool isSpecial)
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

	auto actorElectricity = Cast<UElectricityComponent>(actor->GetComponentByClass(UElectricityComponent::StaticClass()));
	if (!actorElectricity)
		return;

	auto i = ElectricityComponent->GetInfo();

	if (FMath::IsNearlyZero(i->CurrentObjectEnergy))		// nemáme z čeho bychom brali
		return;

	float actuallyPutted = 0.0f;
	float actuallyObtained = 0.0f;
	bool obtainResult = false;
	if (actorElectricity->PutAmount(i->CurrentObjectEnergy, actuallyPutted))
	{
		obtainResult = ElectricityComponent->ObtainAmount(actuallyPutted, actuallyObtained);
		check(obtainResult && FMath::IsNearlyZero(actuallyObtained - actuallyPutted));
	}
}

void ATerminalBlock::BeginPlay()
{
	Super::BeginPlay();

	SelectTargetComponent->EnableUse(400);
	SelectTargetComponent->CustomUsingMessage = NSLOCTEXT("TCF2LocSpace", "LC.UseTerminal", "Doplnit energii / Otevřít");

	FUseDelegate Subscriber;
	Subscriber.BindUObject(this, &ATerminalBlock::ListeningOnUse);
	ListeningHandle = SelectTargetComponent->AddEventListener(Subscriber);
}

void ATerminalBlock::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ListeningHandle.IsValid() && SelectTargetComponent)
		SelectTargetComponent->RemoveEventListener(ListeningHandle);

	Super::EndPlay(EndPlayReason);
}

UPrimitiveComponent* ATerminalBlock::GetComponentForObjectOutline_Implementation() {
	TerminalBlockMeshOutline->SetStaticMesh(TerminalBlockMesh->GetStaticMesh());
	TerminalBlockMeshOutline->SetMaterial(1, TerminalBlockMeshOutline->GetMaterial(0));

	return TerminalBlockMeshOutline;
}

void ATerminalBlock::SetDisplayedWidget(UUserWidget* widget)
{
	shownWidget = widget;
}

UUserWidget* ATerminalBlock::GetShownWidget() { return shownWidget; }
void ATerminalBlock::ShowWidget_Implementation() {
	auto def = Definition->GetDefaultObject<UBlockDefinition>();
	check(def);
	IBlockWithShowableWidget::CallShowWidget(this, def->UsableDef.ShowWidgetOnUse);
}

void ATerminalBlock::NetworkChanged()
{
	if (!shownWidget || !shownWidget->IsValidLowLevel())
		return;
}