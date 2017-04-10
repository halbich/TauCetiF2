#pragma once

#include "Object.h"
#include "BlockSaveInfo.h"


#include "SaveHelpers.generated.h"

/**
 *
 */
UCLASS(Transient)
class GAMESAVE_API USaveHelpers : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "TCF2 | Helpers")
		static TArray<FString> GetAllSaveGameSlots();

	static FORCEINLINE FString USaveHelpers::GetCleanSaveFileName(const FString& worldName, const FDateTime& saveTime) {
		auto name = worldName.Replace(TEXT(" "), TEXT("_"));
		name.ToLowerInline();
		auto date = saveTime.ToString(TEXT("%d_%m_%Y_%H_%M_%S"));

		return FString::Printf(TEXT("%s_%s"), *name, *date);
	}


};
