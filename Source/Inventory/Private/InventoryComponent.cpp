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

	InventoryTags = InventoryHelpers::GetInventoryTags(carrier);

	FSelectionChanged Subscriber;
	Subscriber.BindUObject(this, &UInventoryComponent::InventoryTagsSelectionChanged);
	ListeningHandle = InventoryTags->AddEventListener(Subscriber);

	auto inst = Cast<UTCF2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ensure(inst);
	auto blockRef = Cast<UBlockHolder>(inst->BlockHolder);

	check(blockRef);

	auto aviable = blockRef->GetAviableItems();

	for (auto buildable : InventoryHelpers::GetBuildableBlocks(carrier))
	{
		if (aviable.Contains(buildable->ID))
		{
			buildable->BlockDefinition = blockRef->GetDefinitionFor(buildable->ID);
			buildable->DefinitionSet();

			if (buildable->ValidateObject(validationErrors, blockRef))
				BuildableItems.Add(buildable);
		}
		else
		{
			validationErrors.Add(FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.InventoryComp.FailedToLoadBlock", "Nepodařilo se nahrát blok! ID bloku: {0} "), buildable->ID));
		}
	}

	for (auto buildable : InventoryHelpers::GetInventoryBuildableBlocks(carrier))
	{
		if (aviable.Contains(buildable->ID))
		{
			buildable->BlockDefinition = blockRef->GetDefinitionFor(buildable->ID);
			buildable->DefinitionSet();

			if (buildable->ValidateObject(validationErrors, blockRef))
				InventoryItems.Add(buildable);
		}
		else
		{
			validationErrors.Add(FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.InventoryComp.FailedToLoadBlock", "Nepodařilo se nahrát blok! ID bloku: {0} "), buildable->ID));
		}
	}

	ForceItemsChanged(false);
}

void UInventoryComponent::SaveToCarrier(USaveGameCarrier* carrier)
{
	check(carrier != nullptr);

	InventoryHelpers::SetInventoryTags(carrier, InventoryTags);
	InventoryHelpers::SetBuildableBlocks(carrier, BuildableItems);
	InventoryHelpers::SetInventoryBuildableBlocks(carrier, InventoryItems);
}

void UInventoryComponent::SelectNextBank()
{
	InventoryTags->NextBank();
}

void UInventoryComponent::SelectPrevBank()
{
	InventoryTags->PrevBank();
}

void UInventoryComponent::SelectNextItem()
{
	auto filterGroup = InventoryTags->GetCurrentActiveTagGroup();
	switch (filterGroup->GroupType)
	{
	case EInventoryGroupType::Building: {
		if (!filterGroup->IsBuildableCacheValid) rebuildBuildableCache(filterGroup);
		auto count = filterGroup->BuildableCache.Num();
		if (count > 0)
		{
			CurrentSelectedIndex = (CurrentSelectedIndex + 1) % count;
			ensure(filterGroup->BuildableCache.IsValidIndex(CurrentSelectedIndex));
			OnCurrentSelectedIndexChanged.Broadcast(CurrentSelectedIndex, filterGroup->BuildableCache[CurrentSelectedIndex]);
		}
		break;

	}
	case EInventoryGroupType::Inventory: {
		if (!filterGroup->IsInventoryCacheValid) rebuildInventoryCache(filterGroup);
		auto count = filterGroup->InventoryCache.Num();
		if (count > 0)
		{
			CurrentSelectedIndex = (CurrentSelectedIndex + 1) % count;
			ensure(filterGroup->BuildableCache.IsValidIndex(CurrentSelectedIndex));
			OnCurrentSelectedIndexChanged.Broadcast(CurrentSelectedIndex, filterGroup->BuildableCache[CurrentSelectedIndex]);
		}
		break;
	}
	default:
		checkNoEntry();
		break;
	}
}

void UInventoryComponent::SelectPrevItem()
{
	auto filterGroup = InventoryTags->GetCurrentActiveTagGroup();
	switch (filterGroup->GroupType)
	{
	case EInventoryGroupType::Building: {
		if (!filterGroup->IsBuildableCacheValid) rebuildBuildableCache(filterGroup);
		auto count = filterGroup->BuildableCache.Num();
		if (count > 0)
		{
			CurrentSelectedIndex = (CurrentSelectedIndex + count  - 1) % count;
			ensure(filterGroup->BuildableCache.IsValidIndex(CurrentSelectedIndex));
			OnCurrentSelectedIndexChanged.Broadcast(CurrentSelectedIndex, filterGroup->BuildableCache[CurrentSelectedIndex]);
		}
		break;

	}
	case EInventoryGroupType::Inventory: {
		if (!filterGroup->IsInventoryCacheValid) rebuildInventoryCache(filterGroup);
		auto count = filterGroup->InventoryCache.Num();
		if (count > 0)
		{
			CurrentSelectedIndex = (CurrentSelectedIndex + count  - 1) % count;
			ensure(filterGroup->BuildableCache.IsValidIndex(CurrentSelectedIndex));
			OnCurrentSelectedIndexChanged.Broadcast(CurrentSelectedIndex, filterGroup->BuildableCache[CurrentSelectedIndex]);
		}
		break;
	}
	default:
		checkNoEntry();
		break;
	}
}

void UInventoryComponent::EmptyHand()
{
	CurrentSelectedIndex = -1;
	OnCurrentSelectedIndexChanged.Broadcast(CurrentSelectedIndex, NULL);
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

	switch (filterGroup->GroupType)
	{
	case EInventoryGroupType::Building:

		if (filterGroup->IsBuildableCacheValid)
			return filterGroup->BuildableCache;


		rebuildBuildableCache(filterGroup);
		return filterGroup->BuildableCache;

		break;

	case EInventoryGroupType::Inventory:

		if (filterGroup->IsInventoryCacheValid)
			return filterGroup->InventoryCache;

		rebuildInventoryCache(filterGroup);
		return filterGroup->InventoryCache;

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

	for (auto grp : InventoryTags->InventoryGroupList)
		grp->IsBuildableCacheValid = false;

	ForceItemsChanged(false);
}

void UInventoryComponent::AddItem(UInventoryBuildableBlockInfo* block)
{
	block->DefinitionSet();
	InventoryItems.AddUnique(block);

	for (auto grp : InventoryTags->InventoryGroupList)
		grp->IsInventoryCacheValid = false;

	ForceItemsChanged(false);
}

void UInventoryComponent::ItemBuilt(UInventoryBuildableBlockInfo* block)
{
	auto removed = InventoryItems.Remove(block);
	check(removed == 1);

	for (auto grp : InventoryTags->InventoryGroupList)
		grp->IsInventoryCacheValid = false;

	ForceItemsChanged(false);
}