

#pragma once

#include "GameFramework/SaveGame.h"
#include "TCF2SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TAUCETIF2_API UTCF2SaveGame : public USaveGame
{
	GENERATED_BODY()
	
	
public:
	UTCF2SaveGame();


	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;
	

	UFUNCTION(BlueprintCallable, Category = TCF2SAveGame)
		void SaveGame();

	UFUNCTION(BlueprintCallable, Category = TCF2SAveGame)
		UTCF2SaveGame* LoadGame();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = TCF2SAveGame)
		static UTCF2SaveGame* GetEmpty();
	
};
