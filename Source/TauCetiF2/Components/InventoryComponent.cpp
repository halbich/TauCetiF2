

#include "TauCetiF2.h"
#include "InventoryComponent.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UInventoryComponent::ForceItemsChanged(bool showGroupName)
{
	OnHudBuildableItemsChanged.Broadcast(showGroupName);
}

void UInventoryComponent::InventoryTagsSelectionChanged()
{
	ForceItemsChanged(false);
}

void UInventoryComponent::LoadFromCarrier(USaveGameCarrier* carrier)
{
	check(carrier != nullptr);

	InventoryTags = carrier->InventoryTags;

	FSelectionChanged Subscriber;
	Subscriber.BindUObject(this, &UInventoryComponent::InventoryTagsSelectionChanged);
	ListeningHandle = InventoryTags->AddEventListener(Subscriber);


	auto blockRef = UBlockHolderComponent::GetInstance();

	check (blockRef)

	BuildableItems = TArray<UBuildableBlockInfo*>(carrier->BuildableBlocks);

	ForceItemsChanged(false);
}

void UInventoryComponent::SaveToCarrier(USaveGameCarrier* carrier)
{
	check(carrier != nullptr);


	carrier->InventoryTags = InventoryTags;
	carrier->BuildableBlocks = TArray<UBuildableBlockInfo*>(BuildableItems);
}

void UInventoryComponent::SelectNextBank()
{
	InventoryTags->NextBank();
}

void UInventoryComponent::SelectPrevBank()
{
	InventoryTags->PrevBank();
}

TArray<UBuildableBlockInfo*> UInventoryComponent::GetItemsForCurrentBank()
{
	auto currentFilter = InventoryTags->GetCurrentActiveTagGroup();
	return GetItemsForBank(currentFilter);
}

TArray<UBuildableBlockInfo*> UInventoryComponent::GetItemsForBank(UInventoryTagGroup* filterGroup)
{
	TArray<UBuildableBlockInfo*> result;

	if (!filterGroup)
		return result;
	
	for (auto b : BuildableItems)
	{
		if (filterGroup->IsSatisfied(b->Tags))
			result.Add(b);
	}
	return result;
}


FString UInventoryComponent::GetCurrentBankName()
{
	return InventoryTags->GetCurrentActiveTagGroupName();
}


void UInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ListeningHandle.IsValid() && InventoryTags)
		InventoryTags->RemoveEventListener(ListeningHandle);


	UActorComponent::EndPlay(EndPlayReason);
}

void UInventoryComponent::AddItem(UBuildableBlockInfo* block)
{
	BuildableItems.Add(block);
	ForceItemsChanged(false);
}