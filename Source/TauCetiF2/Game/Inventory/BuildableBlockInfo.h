

#pragma once

#include "Object.h"
#include "BuildableBlockInfo.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class TAUCETIF2_API UBuildableBlockInfo : public UObject
{
	GENERATED_BODY()
public:
	UBuildableBlockInfo();


	UPROPERTY()
		uint32 ID;

	UPROPERTY()
		FVector Scale;

	UPROPERTY()
		FString Name;

	UPROPERTY()
		TArray<FString> Tags;


	FORCEINLINE FInventoryBuildableBlockInfo ToContainer() {
		FInventoryBuildableBlockInfo result;
		result.ID = ID;
		result.Scale = Scale;
		result.Name = Name;
		result.Tags = Tags;
		return result;
	}

	FORCEINLINE void FromContainer(FInventoryBuildableBlockInfo& info) {
		ID = info.ID;
		Scale = info.Scale;
		Name = info.Name;
		Tags = info.Tags;
	}

};
