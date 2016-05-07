

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
	BuildableItems = TArray<UBuildableBlockInfo*>(carrier->BuildableBlocks);

	ForceItemsChanged();
}

void UInventoryComponent::SaveToCarrier(USaveGameCarrier* carrier)
{
	check(carrier != nullptr);

	carrier->BuildableBlocks = TArray<UBuildableBlockInfo*>(BuildableItems);
}