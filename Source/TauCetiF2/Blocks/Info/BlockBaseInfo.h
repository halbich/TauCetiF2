

#pragma once

#include "Object.h"
#include "Helpers/Saving/BlockSaveInfo.h"
#include "Widgets/Common/InventoryFlagItem.h"
#include "Blocks/Definitions/FBlockDefinitionHolder.h"
#include "BlockBaseInfo.generated.h"

/*
 *	Base info about block, used in game. (Not used for saving, see FBlockBaseInfo)
 */
UCLASS(BlueprintType)
class TAUCETIF2_API UBlockBaseInfo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
		int32 ID;

	UPROPERTY(BlueprintReadOnly, Category = BlockInfo)
		FVector Scale;

	UPROPERTY(BlueprintReadOnly, Category = BlockInfo)
		FString Name;

	UPROPERTY()
		TMap<FString, int32> AdditionalFlags;

	FORCEINLINE FBlockBaseInfo ToBaseContainer() {
		FBlockBaseInfo result;
		result.ID = ID;
		result.Scale = Scale;
		result.Name = Name;
		result.AdditionalFlags = AdditionalFlags;
		return result;
	}


	FORCEINLINE void FromBaseContainer(const FBlockBaseInfo& block) {
		ID = block.ID;
		Scale = block.Scale;
		Name = block.Name;
		AdditionalFlags = block.AdditionalFlags;
	}


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = BlockConstructorSelector)
		TArray<UInventoryFlagItem*> GetBlockFlags();

};
