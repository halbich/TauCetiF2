

#pragma once

#include "Object.h"
#include "Helpers/Saving/BlockSaveInfo.h"
#include "BlockBaseInfo.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class TAUCETIF2_API UBlockBaseInfo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
		uint32 ID;

	UPROPERTY(BlueprintReadOnly, Category = BlockInfo)
		FVector Scale;

	UPROPERTY(BlueprintReadOnly, Category = BlockInfo)
		FString Name;

	FORCEINLINE FBlockBaseInfo ToBaseContainer() {
		FBlockBaseInfo result;
		result.ID = ID;
		result.Scale = Scale;
		result.Name = Name;
		return result;
	}


	FORCEINLINE void FromBaseContainer(FBlockBaseInfo& block) {
		ID = block.ID;
		Scale = block.Scale;
		Name = block.Name;
	}

};
