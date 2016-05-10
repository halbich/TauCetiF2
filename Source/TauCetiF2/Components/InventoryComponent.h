

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


	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	UPROPERTY(BlueprintReadWrite, Category = InventoryComponent)
		TArray<UBuildableBlockInfo*> BuildableItems;

	UPROPERTY(BlueprintAssignable, Category = InventoryComponent)
		FHudBuildableItemsChanged OnHudBuildableItemsChanged;

	UFUNCTION(BlueprintCallable, Category = InventoryComponent)
		void ForceItemsChanged();

	UPROPERTY(BlueprintReadWrite, Transient, Category = InventoryComponent)
		UInventoryTags* InventoryTags;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = InventoryComponent)
		TArray<UBuildableBlockInfo*> GetItemsForCurrentBank();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = InventoryComponent)
		FString GetCurrentBankName();
	
	FDelegateHandle ListeningHandle;

	void LoadFromCarrier(USaveGameCarrier* carrier);

	void SaveToCarrier(USaveGameCarrier* carrier);

	void SelectNextBank();
	void SelectPrevBank();

	void AddItem(UBuildableBlockInfo* block);
};
