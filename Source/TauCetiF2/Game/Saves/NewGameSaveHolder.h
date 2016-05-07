

#pragma once

#include "Object.h"
#include "Common/Enums.h"
#include "Game/Localization/LocalizationHelpers.h"
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

	static UNewGameSaveHolder* Instance();

	void init();

	//The Function Pointer Variable Type
	//Functions take in 0 parameters and return void
	typedef USaveGameCarrier* (UNewGameSaveHolder::*FunctionPtrType)(void);

	//A static array of to Function Pointers
	FunctionPtrType fillingFunctions[(uint8)ENamedHardcodedLevel::HardcodedLevelsMax];

	UPROPERTY(Transient)
		TArray<USaveGameCarrier*> newGameSaves;

	UPROPERTY(Transient)
		USaveGameCarrier* mainMenuSave;

	USaveGameCarrier* getDefaultGameSave();
	USaveGameCarrier* getEmptyGameSave();
	USaveGameCarrier* getMainMenuSave();


	FORCEINLINE UBlockInfo* make(int32 id, FVector location, FVector blockScale, FRotator blockRotation)
	{
		auto ret = NewObject<UBlockInfo>(this);
		ret->ID = id;
		ret->Location = location;
		ret->Scale = blockScale;
		ret->Rotation = blockRotation;
		return ret;

	}

	FORCEINLINE UBlockInfo* make(EBlockName id, FVector location, FVector blockScale, FRotator blockRotation)
	{
		return make((int32)id, location, blockScale, blockRotation);
	}

	FORCEINLINE UBuildableBlockInfo* makeBuildable(EBlockName id, FVector blockScale)
	{
		auto ret = NewObject<UBuildableBlockInfo>(this);
		ret->ID = (int32)id;
		ret->Scale = blockScale;
		return ret;
	}
};
