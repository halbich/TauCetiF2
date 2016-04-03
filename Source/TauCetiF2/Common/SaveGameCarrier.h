

#pragma once

#include "Object.h"
#include "SaveGameCarrier.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class TAUCETIF2_API USaveGameCarrier : public UObject
{
	GENERATED_BODY()

public:
	USaveGameCarrier();


	UPROPERTY(BlueprintReadOnly, Category = SaveGameCarrier)
		bool ContainsFullSaveData;

	UPROPERTY(BlueprintReadOnly, Category = SaveGameCarrier)
		bool SaveLoaded;


	UPROPERTY(BlueprintReadWrite, Category = SaveGameCarrier)
		FString SaveName;

	UPROPERTY(BlueprintReadWrite, Category = SaveGameCarrier)
		FString WorldName;

	UPROPERTY(BlueprintReadOnly, Category = SaveGameCarrier)
		FDateTime SavedDate;

	UPROPERTY(BlueprintReadWrite, Category = SaveGameCarrier)
		FTimespan PlayedTime;



	// game - related stuff


	UPROPERTY(BlueprintReadWrite, Category = SaveGameCarrier)
		uint8 TimeOfDay;
};
