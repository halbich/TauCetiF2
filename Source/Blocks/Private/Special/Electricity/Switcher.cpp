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

void ASwitcher::SetBlockInfo(UBlockInfo* info)
{
	Super::SetBlockInfo(info);


	ensure(BlockInfo->ID == SwitcherID);
	auto state = BlockInfo->BlockSpecificData.FindOrAdd(SwitcherBlockConstants::IsOn);
	if (state.IsEmpty())
	{
		IsOn = false;
		BlockInfo->BlockSpecificData[SwitcherBlockConstants::IsOn] = FString::FromInt((uint8)IsOn);
	}
	else {
		IsOn = FCString::Atoi(*state) > 0 ? true : false;
	}

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

	ensure(BlockInfo->ID == SwitcherID);
	BlockInfo->BlockSpecificData[SwitcherBlockConstants::IsOn] = FString::FromInt((uint8)IsOn);

	for (auto controlled : controlledBlocks)
	{
		auto interf = Cast<IControllableBlock>(controlled);
		if (interf)
			interf->Execute_SetControlState(controlled, IsOn);
	}

	updateDynamicColor();
}

void ASwitcher::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
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
	return IsOn;
}

TArray<ABlock*> ASwitcher::GetControlledBlocks_Implementation()
{
	return controlledBlocks;
}

TArray<FString> ASwitcher::GetSupportedAdditionals()
{
	TArray<FString> result;
	result.Add(SwitcherBlockConstants::IsOn);
	return result;
}