#pragma once

#include "Components/ActorComponent.h"
#include "Blocks/Public/Info/BuildableBlockInfo.h"
#include "Blocks/Public/Info/InventoryBuildableBlockInfo.h"
#include "InventoryTags.h"
#include "GameSave/Public/SaveGameCarrier.h"
#include "GameSave/Public/Inventory/FTagGroup.h"
#include "InventoryHelpers.h"
#include "Blocks/Public/Components/BlockHolderComponent.h"
#include "InventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHudBuildableItemsChanged, bool, ShowGroupName);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INVENTORY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(BlueprintReadWrite, Transient, Category = InventoryComponent)
		TArray<UBuildableBlockInfo*> BuildableItems;

	UPROPERTY(BlueprintReadWrite, Transient, Category = InventoryComponent)
		TArray<UInventoryBuildableBlockInfo*> InventoryItems;

	UPROPERTY(BlueprintAssignable, Transient, Category = InventoryComponent)
		FHudBuildableItemsChanged OnHudBuildableItemsChanged;

	UPROPERTY(BlueprintReadWrite, Transient, Category = InventoryComponent)
		UInventoryTags* InventoryTags;

	UFUNCTION(BlueprintCallable, Category = InventoryComponent)
		void ForceItemsChanged(bool showGroupName);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = InventoryComponent)
		TArray<UBuildableBlockInfo*> GetItemsForCurrentBank();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = InventoryComponent)
		TArray<UBuildableBlockInfo*> GetItemsForBank(UInventoryTagGroup* filterGroup);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = InventoryComponent)
		FString GetCurrentBankName();

	FDelegateHandle ListeningHandle;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void InventoryTagsSelectionChanged();

	void LoadFromCarrier(USaveGameCarrier* carrier, TArray<FText>& validationErrors);

	void SaveToCarrier(USaveGameCarrier* carrier);

	void SelectNextBank();
	void SelectPrevBank();

	void AddItem(UBuildableBlockInfo* block);

	void AddItem(UInventoryBuildableBlockInfo* block);

	void ItemBuilt(UInventoryBuildableBlockInfo* block);
};
