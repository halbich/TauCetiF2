

#include "TauCetiF2.h"
#include "InventoryComponent.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UInventoryComponent::ForceItemsChanged()
{
	OnHudBuildableItemsChanged.Broadcast();
}

void UInventoryComponent::LoadFromCarrier(USaveGameCarrier* carrier)
{
	check(carrier != nullptr);

	InventoryTags = UInventoryTags::GetDefault();

	FSelectionChanged Subscriber;
	Subscriber.BindUObject(this, &UInventoryComponent::ForceItemsChanged);
	ListeningHandle = InventoryTags->AddEventListener(Subscriber);
	
	BuildableItems = TArray<UBuildableBlockInfo*>(carrier->BuildableBlocks);

	ForceItemsChanged();
}

void UInventoryComponent::SaveToCarrier(USaveGameCarrier* carrier)
{
	check(carrier != nullptr);

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
	TArray<UBuildableBlockInfo*> result;

	for (auto b : BuildableItems)
	{
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