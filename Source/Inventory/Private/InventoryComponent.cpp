#include "Inventory.h"
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

void UInventoryComponent::LoadFromCarrier(USaveGameCarrier* carrier, TArray<FText>& validationErrors)
{
	check(carrier != nullptr);

	InventoryTags = carrier->GetInventoryTags();

	FSelectionChanged Subscriber;
	Subscriber.BindUObject(this, &UInventoryComponent::InventoryTagsSelectionChanged);
	ListeningHandle = InventoryTags->AddEventListener(Subscriber);

	auto blockRef = UBlockHolderComponent::GetInstance();

	check(blockRef);

	auto aviable = blockRef->GetAviableItems();

	for (auto buildable : carrier->GetBuildableBlockData())
	{
		if (aviable.Contains(buildable->ID))
		{
			buildable->BlockDefinition = blockRef->GetDefinitionFor(buildable->ID);
			buildable->DefinitionSet();

			if (buildable->ValidateObject(validationErrors))
				BuildableItems.Add(buildable);
		}
		else
		{
			validationErrors.Add(FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.InventoryComp.FailedToLoadBlock", "Nepodaøilo se nahrát blok! ID bloku: {0} "), buildable->ID));
		}
	}

	for (auto buildable : carrier->GetInventoryBuildableBlockData())
	{
		if (aviable.Contains(buildable->ID))
		{
			buildable->BlockDefinition = blockRef->GetDefinitionFor(buildable->ID);
			buildable->DefinitionSet();

			if (buildable->ValidateObject(validationErrors))
				InventoryItems.Add(buildable);
		}
		else
		{
			validationErrors.Add(FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.InventoryComp.FailedToLoadBlock", "Nepodaøilo se nahrát blok! ID bloku: {0} "), buildable->ID));
		}
	}

	ForceItemsChanged(false);
}

void UInventoryComponent::SaveToCarrier(USaveGameCarrier* carrier)
{
	check(carrier != nullptr);

	carrier->FillData(InventoryTags);
	carrier->FillData(BuildableItems);
	carrier->FillData(InventoryItems);
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

	print(TEXT("Getting items for bank"));

	if (!filterGroup)
		return result;

	switch (filterGroup->GroupType)
	{
	case EInventoryGroupType::Building:
		for (auto b : BuildableItems)
		{
			if (b->BlockDefinition->IsPlaceable && filterGroup->IsSatisfied(b->Tags))
				result.Add(b);
		}
		break;

	case EInventoryGroupType::Inventory:
		for (auto b : InventoryItems)
		{
			if (b->BlockDefinition->IsInventoryObject && filterGroup->IsSatisfied(b->Tags))
				result.Add(b);
		}
		break;
	default:
		checkNoEntry();
		break;
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
	block->DefinitionSet();
	BuildableItems.Add(block);
	ForceItemsChanged(false);
}

void UInventoryComponent::AddItem(UInventoryBuildableBlockInfo* block)
{
	block->DefinitionSet();
	InventoryItems.AddUnique(block);
	ForceItemsChanged(false);
}

void UInventoryComponent::ItemBuilt(UInventoryBuildableBlockInfo* block)
{
	auto removed = InventoryItems.Remove(block);
	check(removed == 1);
	ForceItemsChanged(false);

}