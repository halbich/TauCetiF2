

#pragma once

#include "Object.h"
#include "Common/SaveGameCarrier.h"
#include "NewGameSaveHolder.generated.h"

/**
 *
 */
UCLASS()
class TAUCETIF2_API UNewGameSaveHolder : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = TCF2SAveGame)
		static TArray<USaveGameCarrier*> GetNewSaveGamesList();

	UFUNCTION(BlueprintCallable, Category = TCF2SAveGame)
		static USaveGameCarrier* DEBUG_GetTestSave();

	UFUNCTION(BlueprintCallable, Category = TCF2SAveGame)
		static USaveGameCarrier* GetSaveForMainMenu();

private:

	static USaveGameCarrier* getDefaultGameSave();
	static USaveGameCarrier* getEmptyGameSave();
};
