

#pragma once

#include "Components/ActorComponent.h"
#include "Blocks/Info/BuildableBlockInfo.h"
#include "Game/Inventory/InventoryTags.h"
#include "Common/SaveGameCarrier.h"
#include "InventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHudBuildableItemsChanged);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TAUCETIF2_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

	// Called when the game starts
	virtual void BeginPlay() override;



	UPROPERTY(BlueprintReadWrite, Category = InventoryComponent)
		TArray<UBuildableBlockInfo*> BuildableItems;

	UPROPERTY(BlueprintAssignable, Category = InventoryComponent)
		FHudBuildableItemsChanged OnHudBuildableItemsChanged;

	UFUNCTION(BlueprintCallable, Category = InventoryComponent)
		void ForceItemsChanged();

	UPROPERTY(BlueprintReadWrite, Transient, Category = InventoryComponent)
		UInventoryTags* InventoryTags;

	void LoadFromCarrier(USaveGameCarrier* carrier);

	void SaveToCarrier(USaveGameCarrier* carrier);
};
