#include "Blocks.h"
#include "OxygenTankFillerBlock.h"

AOxygenTankFillerBlock::AOxygenTankFillerBlock()
	: Super(), FillingItemCritical()
{
	PrimaryActorTick.bCanEverTick = true;

	GetRootComponent()->SetMobility(EComponentMobility::Stationary);

	OxygenTankFillerBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OxygenTankFillerBodyMesh"));
	OxygenTankFillerBodyMesh->SetupAttachment(GetRootComponent());
	OxygenTankFillerBodyMesh->SetMobility(EComponentMobility::Stationary);

	OxygenTankFillerHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OxygenTankFillerHeadMesh"));
	OxygenTankFillerHeadMesh->SetupAttachment(OxygenTankFillerBodyMesh);
	OxygenTankFillerHeadMesh->SetMobility(EComponentMobility::Movable);
	OxygenTankFillerHeadMesh->SetCastShadow(false);

	OxygenTankFillerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OxygenTankFillerMesh"));
	OxygenTankFillerMesh->SetupAttachment(OxygenTankFillerBodyMesh);
	OxygenTankFillerMesh->SetMobility(EComponentMobility::Stationary);
	OxygenTankFillerMesh->SetVisibility(false);

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

void AOxygenTankFillerBlock::SetBlockInfo(UBlockInfo* info)
{
	Super::SetBlockInfo(info);

	ensure(BlockInfo->ID == OxygenTankFillerID);
	auto currentF = 0.0f;
	auto filling = BlockInfo->BlockSpecificData.FindOrAdd(OxygenFillerBlockConstants::CurrentFilling);
	if (filling.IsEmpty())
		BlockInfo->BlockSpecificData[OxygenFillerBlockConstants::CurrentFilling] = FString::SanitizeFloat(currentF);
	else
		currentF = FCString::Atof(*filling);

	auto hasItem = false;
	auto item = BlockInfo->BlockSpecificData.FindOrAdd(OxygenFillerBlockConstants::HasItem);
	if (item.IsEmpty())
		BlockInfo->BlockSpecificData[OxygenFillerBlockConstants::HasItem] = FString::FromInt((uint8)hasItem);
	else
		hasItem = FCString::Atoi(*item) > 0 ? true : false;

	auto tags = BlockInfo->BlockSpecificData.FindOrAdd(OxygenFillerBlockConstants::ItemTags);

	if (hasItem)
	{
		ensure(currentFillingItem == NULL);

		auto newItm = NewObject<UInventoryBuildableBlockInfo>();

		newItm->ID = OxygenTankID;

		auto gameInstance = Cast<UTCF2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		ensure(gameInstance);

		auto holder = Cast<UBlockHolder>(gameInstance->BlockHolder);

		ensure(holder);

		newItm->BlockDefinition = holder->GetDefinitionFor(newItm->ID);
		newItm->Scale = newItm->BlockDefinition->CustomBlockScale;

		newItm->OxygenInfo = NewObject<UBlockWithOxygenInfo>();
		newItm->OxygenInfo->CurrentObjectOxygen = currentF;

		newItm->SetTagsFlatlined(tags);

		newItm->DefinitionSet();
		newItm->UpdateDisplayValue();

		currentFillingItem = newItm;
	}

	OxygenTankFillerMesh->SetVisibility(currentFillingItem != NULL);

	PoweredBlockInfo = ElectricityComponent->ElectricityInfo->PoweredBlockInfo;
	ensure(PoweredBlockInfo);
}

void  AOxygenTankFillerBlock::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);



	dynInfoMat = UMaterialInstanceDynamic::Create(OxygenTankFillerMesh->GetMaterial(0), this);
	OxygenTankFillerMesh->SetMaterial(0, dynInfoMat);
	OxygenTankFillerMesh->SetVisibility(currentFillingItem != NULL);
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


void AOxygenTankFillerBlock::BeginPlay()
{
	Super::BeginPlay();

	SelectTargetComponent->EnableUse(500);
	SelectTargetComponent->CustomUsingMessage = NSLOCTEXT("TCF2LocSpace", "LC.OxygenTankFillerBlock.Pickup", "Doplnit kyslík / Otevřít konzoli");

	FUseDelegate Subscriber;
	Subscriber.BindUObject(this, &AOxygenTankFillerBlock::ListeningOnUse);
	ListeningHandle = SelectTargetComponent->AddEventListener(Subscriber);

}

void AOxygenTankFillerBlock::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ListeningHandle.IsValid() && SelectTargetComponent)
		SelectTargetComponent->RemoveEventListener(ListeningHandle);

	Cast<IControllableBlock>(this)->Execute_SetController(this, NULL);

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

void AOxygenTankFillerBlock::SetControlState_Implementation(bool isOn) {
	ensure(BlockInfo->ID == OxygenTankFillerID);
	PoweredBlockInfo->IsOn = isOn;
}
void AOxygenTankFillerBlock::SetOutputPowerPercentage_Implementation(float percentage) {
	ensure(BlockInfo->ID == OxygenTankFillerID);
	PoweredBlockInfo->PowerConsumptionPercent = percentage;
}

void AOxygenTankFillerBlock::SetController_Implementation(ABlock* controller) {
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
}
ABlock* AOxygenTankFillerBlock::GetController_Implementation() { return usedController; }

void AOxygenTankFillerBlock::Tick(float DeltaSeconds)
{
	if (!PoweredBlockInfo->IsOn)
	{
		Super::Tick(DeltaSeconds);
		return;
	}

	processCurrentFillingItem(DeltaSeconds);

	auto status = OxygenComponent->OxygenInfo;

	auto diff = status->CurrentObjectMaximumOxygen - status->CurrentObjectOxygen;

	if (FMath::IsNearlyZero(diff) || FMath::IsNearlyZero(ElectricityComponent->ElectricityInfo->CurrentObjectEnergy))
	{
		Super::Tick(DeltaSeconds);
		return;
	}

	auto elapsedSeconds = DeltaSeconds * GameDefinitions::GameDayMultiplier;

	auto max = ElectricityComponent->GetDefinition()->MaxConsumedEnergyPerGameSecond;
	auto i = ElectricityComponent->GetInfo();

	auto toObtain = elapsedSeconds * i->GetPowerOutput()  * max;

	auto toWithdraw = FMath::Min(diff *  GameDefinitions::OxygenToEnergy, toObtain);

	auto originalElectricity = i->CurrentObjectEnergy;
	float actuallyObtained = 0;
	float actuallyPutted = 0;
	float acuallyReturned = 0;
	if (ElectricityComponent->ObtainAmount(toWithdraw, actuallyObtained)) {
		float toReturn = actuallyObtained;
		if (OxygenComponent->PutAmount(actuallyObtained * GameDefinitions::EnergyToOxygen, actuallyPutted))
			toReturn -= actuallyPutted * GameDefinitions::OxygenToEnergy;

		toReturn = FMath::Max(0.0f, toReturn);

		ElectricityComponent->PutAmount(toReturn, acuallyReturned);

		ElectricityComponent->EnergyConsumed += FMath::Max(0.0f, originalElectricity - i->CurrentObjectEnergy);
	}

	Super::Tick(DeltaSeconds);
}

UInventoryBuildableBlockInfo* AOxygenTankFillerBlock::TakeCurrentFillingItem(bool& success)
{
	FillingItemCritical.Lock();

	success = currentFillingItem != NULL;
	auto ret = currentFillingItem;
	currentFillingItem = NULL;
	OxygenTankFillerMesh->SetVisibility(false);

	ensure(BlockInfo->ID == OxygenTankFillerID);
	BlockInfo->BlockSpecificData[OxygenFillerBlockConstants::HasItem] = FString::FromInt((uint8)(currentFillingItem != NULL));
	BlockInfo->BlockSpecificData[OxygenFillerBlockConstants::ItemTags] = currentFillingItem ? currentFillingItem->GetTagsFlatlined() : TEXT("");

	FillingItemCritical.Unlock();

	if (ret)
		ret->UpdateDisplayValue();

	return ret;
}

bool AOxygenTankFillerBlock::SetCurrentFillingItem(UInventoryBuildableBlockInfo* info)
{
	FillingItemCritical.Lock();
	if (currentFillingItem)
	{
		FillingItemCritical.Unlock();
		return false;
	}

	ensure(BlockInfo->ID == OxygenTankFillerID);
	BlockInfo->BlockSpecificData[OxygenFillerBlockConstants::HasItem] = FString::FromInt((uint8)(info != NULL));
	BlockInfo->BlockSpecificData[OxygenFillerBlockConstants::ItemTags] = info->GetTagsFlatlined();

	currentFillingItem = info;

	OxygenTankFillerMesh->SetVisibility(info != NULL);
	updateDisplayedMesh();
	FillingItemCritical.Unlock();

	return true;
}

TArray<FString> AOxygenTankFillerBlock::GetSupportedAdditionals()
{
	TArray<FString> result;
	result.Add(OxygenFillerBlockConstants::CurrentFilling);
	result.Add(OxygenFillerBlockConstants::HasItem);
	result.Add(OxygenFillerBlockConstants::ItemTags);
	return result;
}

void AOxygenTankFillerBlock::SetAutoregulatePower(bool newAutoregulatePower)
{
	PoweredBlockInfo->AutoregulatePower = newAutoregulatePower;
}