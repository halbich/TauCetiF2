#include "Blocks.h"
#include "Switcher.h"

ASwitcher::ASwitcher()
	: Super()
{
	PrimaryActorTick.bCanEverTick = false;

	GetRootComponent()->SetMobility(EComponentMobility::Stationary);

	SwitcherMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitcherMesh"));
	SwitcherMesh->SetupAttachment(GetRootComponent());
	SwitcherMesh->SetMobility(EComponentMobility::Stationary);

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

	
}

void ASwitcher::SetBlockInfo(UBlockInfo* info)
{
	Super::SetBlockInfo(info);

	auto reacts = BlockInfo->BlockSpecificData.FindOrAdd(SwitcherBlockConstants::ReactsToDayCycle);
	if (reacts.IsEmpty())
		BlockInfo->BlockSpecificData[SwitcherBlockConstants::ReactsToDayCycle] = FString::FromInt((uint8)ReactsToDayCycle);
	else
		ReactsToDayCycle = FCString::Atoi(*reacts) > 0 ? true : false;

	auto stateAtDay = BlockInfo->BlockSpecificData.FindOrAdd(SwitcherBlockConstants::StateAtDay);
	if (stateAtDay.IsEmpty())
		BlockInfo->BlockSpecificData[SwitcherBlockConstants::StateAtDay] = FString::FromInt((uint8)StateAtDay);
	else
		StateAtDay = FCString::Atoi(*stateAtDay) > 0 ? true : false;

	auto stateAtNight = BlockInfo->BlockSpecificData.FindOrAdd(SwitcherBlockConstants::StateAtNight);
	if (stateAtNight.IsEmpty())
		BlockInfo->BlockSpecificData[SwitcherBlockConstants::StateAtNight] = FString::FromInt((uint8)StateAtNight);
	else
		StateAtNight = FCString::Atoi(*stateAtNight) > 0 ? true : false;

	PoweredBlockInfo = ElectricityComponent->ElectricityInfo->PoweredBlockInfo;
	ensure(PoweredBlockInfo);

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

	FlipCurrentOnState();
}

void ASwitcher::BeginPlay() {
	Super::BeginPlay();

	auto inst = Cast<UTCF2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ensure(inst);
	inst->OnDaytimeChangedEvent.AddDynamic(this, &ASwitcher::OnNightChanged);

	SelectTargetComponent->EnableUse(500);
	SelectTargetComponent->CustomUsingMessage = NSLOCTEXT("TCF2LocSpace", "LC.Switcher.Use", "Přepnout / Nastavit");

	FUseDelegate Subscriber;
	Subscriber.BindUObject(this, &ASwitcher::ListeningOnUse);
	ListeningHandle = SelectTargetComponent->AddEventListener(Subscriber);

	updateDynamicColor();


}

void ASwitcher::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	auto inst = Cast<UTCF2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ensure(inst);
	inst->OnDaytimeChangedEvent.RemoveDynamic(this, &ASwitcher::OnNightChanged);

	if (ListeningHandle.IsValid() && SelectTargetComponent)
		SelectTargetComponent->RemoveEventListener(ListeningHandle);

	TQueue<ABlock*> toRemove;

	for (auto controlled : controlledBlocks)
		toRemove.Enqueue(controlled);

	ABlock* rem;
	while (toRemove.Dequeue(rem))
	{
		auto interf = Cast<IControllableBlock>(rem);
		if (interf)
			interf->Execute_SetController(rem, NULL);
	}

	Super::EndPlay(EndPlayReason);
}

void ASwitcher::SetDisplayedWidget(UUserWidget* widget)
{
	shownWidget = widget;
}

UUserWidget* ASwitcher::GetShownWidget() { return shownWidget; }

void ASwitcher::ShowWidget_Implementation()
{
	auto def = Definition->GetDefaultObject<UBlockDefinition>();
	check(def);
	IBlockWithShowableWidget::CallShowWidget(this, def->UsableDef.ShowWidgetOnUse);
}

bool ASwitcher::BindControl_Implementation(ABlock* controllableBlock)
{
	auto interf = Cast<IControllableBlock>(controllableBlock);
	if (!interf)
		return false;

	auto electr = Cast<IBlockWithElectricity>(controllableBlock);
	check(electr);

	if (electr->GetElectricityComponent()->Network != ElectricityComponent->Network)
		return false;

	ensure(BlockInfo->ID == SwitcherID);
	auto my = BlockInfo->RelationsInfo;
	ensure(my && my->IsValidLowLevel());

	auto other = controllableBlock->BlockInfo->RelationsInfo;
	ensure(other && other->IsValidLowLevel());

	if (my->ID == other->ID)
		return false;

	auto controlledRel = NewObject<URelationshipInfo>();
	controlledRel->TargetID = my->ID;
	controlledRel->RelationshipType = (uint8)EControlRelationship::IsControlledByTarget;

	auto c = other->Relationships.FindByPredicate([controlledRel](URelationshipInfo* info) {
		return info->TargetID == controlledRel->TargetID && info->RelationshipType == controlledRel->RelationshipType;
	});

	if (!c)
		other->Relationships.Add(controlledRel);

	auto controllingRel = NewObject<URelationshipInfo>();
	controllingRel->TargetID = other->ID;
	controllingRel->RelationshipType = (uint8)EControlRelationship::IsControllingTarget;

	auto c1 = my->Relationships.FindByPredicate([controllingRel](URelationshipInfo* info) {
		return info->TargetID == controllingRel->TargetID && info->RelationshipType == controllingRel->RelationshipType;
	});

	if (!c1)
		my->Relationships.Add(controllingRel);

	interf->Execute_SetController(controllableBlock, this);
	controlledBlocks.AddUnique(controllableBlock);
	updateDynamicColor();
	return true;
}

bool ASwitcher::UnbindControl_Implementation(ABlock* controllableBlock)
{
	auto interf = Cast<IControllableBlock>(controllableBlock);
	if (!interf)
		return false;

	ensure(BlockInfo->ID == SwitcherID);
	auto my = BlockInfo->RelationsInfo;
	ensure(my && my->IsValidLowLevel());

	auto other = controllableBlock->BlockInfo->RelationsInfo;
	ensure(other && other->IsValidLowLevel());

	auto res = controlledBlocks.Remove(controllableBlock) != 0;

	updateDynamicColor();
	interf->Execute_SetController(controllableBlock, NULL);

	other->RemoveRelationshipsByTargetID(my->ID);
	my->RemoveRelationshipsByTargetID(other->ID);

	return res;
}

bool ASwitcher::GetControlState_Implementation()
{
	return PoweredBlockInfo->IsOn;
}

TArray<ABlock*> ASwitcher::GetControlledBlocks_Implementation()
{
	return controlledBlocks;
}

TArray<FString> ASwitcher::GetSupportedAdditionals()
{
	TArray<FString> result;
	result.Add(SwitcherBlockConstants::ReactsToDayCycle);
	result.Add(SwitcherBlockConstants::StateAtDay);
	result.Add(SwitcherBlockConstants::StateAtNight);
	return result;
}

bool ASwitcher::FlipCurrentOnState()
{
	ensure(BlockInfo->ID == SwitcherID);
	auto isOn = PoweredBlockInfo->IsOn = !PoweredBlockInfo->IsOn;

	for (auto controlled : controlledBlocks)
	{
		auto interf = Cast<IControllableBlock>(controlled);
		if (interf)
			interf->Execute_SetControlState(controlled, isOn);
	}

	updateDynamicColor();
	return isOn;
}

void ASwitcher::OnNightChanged(bool isNight, bool calledByLevelLoad)
{
	if (!ReactsToDayCycle)
		return;

	auto oldValue = calledByLevelLoad ? PoweredBlockInfo->IsOn : false;

	auto newIsON = isNight ? StateAtNight : StateAtDay;

	if (calledByLevelLoad && oldValue != newIsON)	// we forced other value, thus we need to correct it
		newIsON = oldValue;

	PoweredBlockInfo->IsOn = newIsON;

	for (auto controlled : controlledBlocks)
	{
		auto interf = Cast<IControllableBlock>(controlled);
		if (interf)
			interf->Execute_SetControlState(controlled, newIsON);
	}

	updateDynamicColor();
}

void ASwitcher::SetReactsToDayCycle(bool newReactsToDayCycle)
{
	ensure(BlockInfo->ID == SwitcherID);
	ReactsToDayCycle = newReactsToDayCycle;
	BlockInfo->BlockSpecificData[SwitcherBlockConstants::ReactsToDayCycle] = FString::FromInt((uint8)ReactsToDayCycle);

	auto inst = Cast<UTCF2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ensure(inst);
	OnNightChanged(inst->IsNightInGame, false);
}

void ASwitcher::SetStateAtDay(bool newStateAtDay)
{
	ensure(BlockInfo->ID == SwitcherID);
	StateAtDay = newStateAtDay;
	BlockInfo->BlockSpecificData[SwitcherBlockConstants::StateAtDay] = FString::FromInt((uint8)StateAtDay);

	auto inst = Cast<UTCF2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ensure(inst);
	OnNightChanged(inst->IsNightInGame, false);
}

void ASwitcher::SetStateAtNight(bool newStateAtNight)
{
	ensure(BlockInfo->ID == SwitcherID);
	StateAtNight = newStateAtNight;
	BlockInfo->BlockSpecificData[SwitcherBlockConstants::StateAtNight] = FString::FromInt((uint8)StateAtNight);

	auto inst = Cast<UTCF2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ensure(inst);
	OnNightChanged(inst->IsNightInGame, false);
}