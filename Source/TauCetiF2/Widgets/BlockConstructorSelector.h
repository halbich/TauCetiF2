

#pragma once

#include "Common/Enums.h"
#include "Widgets/SynchronizeWidget.h"
#include "Blocks/Definitions/FBlockDefinition.h"
#include "Blocks/Definitions/FBlockDefinitionHolder.h"
#include "Blocks/Info/BuildableBlockInfo.h"
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = BlockConstructorSelector, meta = (DisplayName = "Block ID", ExposeOnSpawn = true))
		int32 BlockID;

	UPROPERTY(BlueprintReadOnly, Category = BlockConstructorSelector)
		UTexture2D* BlockTexture;

	FBlockDefinition* Definition;

	UFUNCTION(BlueprintCallable, Category = BlockConstructorSelector)
		bool IsValid();

	UFUNCTION(BlueprintCallable, Category = BlockConstructorSelector)
		void GetMinMax(UPARAM(ref)FVector& minSize, UPARAM(ref)FVector& maxSize);

	virtual void SynchronizeProperties() override;
};
