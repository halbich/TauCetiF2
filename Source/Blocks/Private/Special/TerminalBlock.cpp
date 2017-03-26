#include "Blocks.h"
#include "TerminalBlock.h"

ATerminalBlock::ATerminalBlock()
	: Super(), ListeningHandle()
{
	PrimaryActorTick.bCanEverTick = false;

	TerminalBlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TerminalBlockMesh"));
	TerminalBlockMesh->SetupAttachment(GetRootComponent());

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

	SelectTargetComponent->EnableUse(400);
	SelectTargetComponent->CustomUsingMessage = NSLOCTEXT("TCF2LocSpace", "LC.UseTerminal", "Použít / Otevřít");

	FUseDelegate Subscriber;
	Subscriber.BindUObject(this, &ATerminalBlock::ListeningOnUse);
	ListeningHandle = SelectTargetComponent->AddEventListener(Subscriber);
}

void ATerminalBlock::ListeningOnUse(AActor* actor, bool isSpecial)
{
	if (!actor || !actor->IsValidLowLevel())
		return;

	if (!isSpecial)
	{
		print(TEXT("not special use"));
		auto def = Definition->GetDefaultObject<UBlockDefinition>();
		check(def);
		IBlockWithShowableWidget::CallShowWidget(this, def->UsableDef.ShowWidgetOnUse);
		return;
	}

	auto actorElectricity = Cast<UElectricityComponent>(actor->GetComponentByClass(UElectricityComponent::StaticClass()));
	if (!actorElectricity)
		return;

	if (FMath::IsNearlyZero(ElectricityComponent->ElectricityInfo->CurrentObjectEnergy))		// nemáme z čeho bychom brali
		return;

	float actuallyPutted = 0.0f;
	float actuallyObtained = 0.0f;
	bool obtainResult = false;
	if (actorElectricity->PutAmount(ElectricityComponent->ElectricityInfo->CurrentObjectEnergy, actuallyPutted))
	{
		obtainResult = ElectricityComponent->ObtainAmount(actuallyPutted, actuallyObtained);
		check(obtainResult && FMath::IsNearlyZero(actuallyObtained - actuallyPutted));
	}
}

void ATerminalBlock::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ListeningHandle.IsValid() && SelectTargetComponent)
		SelectTargetComponent->RemoveEventListener(ListeningHandle);

	Super::EndPlay(EndPlayReason);
}

UPrimitiveComponent* ATerminalBlock::GetComponentForObjectOutline_Implementation() {
	return TerminalBlockMesh;
}