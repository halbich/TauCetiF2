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

	UPROPERTY(BlueprintReadOnly, Transient, EditAnywhere, Category = "TCF2 | BlockConstructorSelector", meta = (DisplayName = "Block ID", ExposeOnSpawn = true))
		int32 BlockID;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockConstructorSelector", meta = (DisplayName = "Block Holder", ExposeOnSpawn = true))
		UBlockHolder* BlockHolder;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockConstructorSelector")
		UBlockDefinition* BlockDefinition;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | BlockConstructorSelector")
		bool IsDefinitionValid();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | BlockConstructorSelector")
		void GetMinMax(FVector& minSize, FVector& maxSize);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | BlockConstructorSelector")
		TArray<UInventoryFlagItem*> GetAdditionalParams();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | BlockConstructorSelector")
		TArray<FString> GetImplicitTags();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | BlockConstructorSelector")
		UTexture2D* GetBlockTexture();

	virtual void SynchronizeProperties() override;
};
