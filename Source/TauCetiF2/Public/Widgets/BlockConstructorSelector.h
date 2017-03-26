#pragma once

#include "Widgets/SynchronizeWidget.h"
#include "Blocks/Public/Components/BlockHolderComponent.h"
#include "Blocks/Public/Definitions/BlockDefinition.h"
#include "Widgets/Common/InventoryFlagItem.h"
#include "BlockConstructorSelector.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class TAUCETIF2_API UBlockConstructorSelector : public USynchronizeWidget
{
	GENERATED_BODY()
public:

	UBlockConstructorSelector();

	UPROPERTY(BlueprintReadOnly, Transient, EditAnywhere, Category = BlockConstructorSelector, meta = (DisplayName = "Block ID", ExposeOnSpawn = true))
		int32 BlockID;

	UPROPERTY(BlueprintReadOnly, Transient, Category = BlockConstructorSelector, meta = (DisplayName = "Block Holder", ExposeOnSpawn = true))
		UBlockHolderComponent* BlockHolder;

	UPROPERTY(BlueprintReadOnly, Transient, Category = BlockConstructorSelector)
		UBlockDefinition* BlockDefinition;

	UFUNCTION(BlueprintCallable, Category = BlockConstructorSelector)
		bool IsDefinitionValid();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = BlockConstructorSelector)
		void GetMinMax(FVector& minSize, FVector& maxSize);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = BlockConstructorSelector)
		TArray<UInventoryFlagItem*> GetAdditionalParams();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = BlockConstructorSelector)
		TArray<FString> GetImplicitTags();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = BlockConstructorSelector)
		UTexture2D* GetBlockTexture();

	virtual void SynchronizeProperties() override;
};
