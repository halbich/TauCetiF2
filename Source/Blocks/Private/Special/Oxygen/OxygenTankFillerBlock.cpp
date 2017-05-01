#include "Blocks.h"
#include "OxygenTankFillerBlock.h"

AOxygenTankFillerBlock::AOxygenTankFillerBlock()
	: Super(), FillingItemCritical()
{
	PrimaryActorTick.bCanEverTick = true;

	OxygenTankFillerBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OxygenTankFillerBodyMesh"));
	OxygenTankFillerBodyMesh->SetupAttachment(GetRootComponent());

	OxygenTankFillerHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OxygenTankFillerHeadMesh"));
	OxygenTankFillerHeadMesh->SetupAttachment(OxygenTankFillerBodyMesh);

	OxygenTankFillerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OxygenTankFillerMesh"));
	OxygenTankFillerMesh->SetupAttachment(OxygenTankFillerBodyMesh);

	OxygenTankFillerMesh->SetVisibility(false);

	OxygenComponent = CreateDefaultSubobject<UOxygenComponent>(TEXT("OxygenComponent"));
	AddOwnedComponent(OxygenComponent);

	ElectricityComponent = CreateDefaultSubobject<UElectricityComponent>(TEXT("ElectricityComponent"));
	AddOwnedComponent(ElectricityComponent);

	IsOn = true;
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

		newItm->DefinitionSet();
		newItm->UpdateDisplayValue();

		currentFillingItem = newItm;
	}

	OxygenTankFillerMesh->SetVisibility(currentFillingItem != NULL);
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

void AOxygenTankFillerBlock::ListeningOnOxygenCompChanged(UBlockWithOxygenInfo* source)
{
}

void AOxygenTankFillerBlock::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ListeningHandle.IsValid() && SelectTargetComponent)
		SelectTargetComponent->RemoveEventListener(ListeningHandle);

	if (OxygenComponent)
		OxygenComponent->OnComponentDataChangedEvent.RemoveDynamic(this, &AOxygenTankFillerBlock::ListeningOnOxygenCompChanged);

	this->Execute_SetController(this, NULL);

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

void AOxygenTankFillerBlock::SetControlState_Implementation(bool isOn) { IsOn = isOn; }
void AOxygenTankFillerBlock::SetOutputPowerPercentage_Implementation(float percentage) {
	ensure(BlockInfo->ID == OxygenTankFillerID);
	BlockInfo->ElectricityInfo->PowerConsumptionPercent = percentage;
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
			this->Execute_SetControlState(this, interf->Execute_GetControlState(usedController));
	}
	else
		IsOn = true;
}
ABlock* AOxygenTankFillerBlock::GetController_Implementation() { return usedController; }

void AOxygenTankFillerBlock::Tick(float DeltaSeconds)
{
	if (!IsOn)
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
	auto powerConsumption = IsOn ? ElectricityComponent->GetInfo()->PowerConsumptionPercent : 0;

	auto possibleEnergy = elapsedSeconds * powerConsumption  * max; // now we can withdraw only this amount;

	auto toWithdraw = FMath::Min(diff *  GameDefinitions::OxygenToEnergy, possibleEnergy);

	float actuallyObtained = 0;
	float actuallyPutted = 0;
	float acuallyReturned = 0;
	if (ElectricityComponent->ObtainAmount(toWithdraw, actuallyObtained)) {
		float toReturn = actuallyObtained;
		if (OxygenComponent->PutAmount(actuallyObtained * GameDefinitions::EnergyToOxygen, actuallyPutted))
			toReturn -= actuallyPutted * GameDefinitions::OxygenToEnergy;

		toReturn = FMath::Max(0.0f, toReturn);

		ElectricityComponent->PutAmount(toReturn, acuallyReturned);
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
	return result;
}