

#pragma once

#include "Object.h"
#include "Commons/Public/Enums.h"
#include "Game/Localization/LocalizationHelpers.h"
#include "Common/SaveGameCarrier.h"
#include "Game/Inventory/InventoryTags.h"
#include "NewGameSaveHolder.generated.h"


#define CubeBaseID 0
#define CubeBuildingID 1


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
	UFUNCTION()
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

	// TODO
	/*FORCEINLINE UBlockInfo* make(EBlockName id, FVector location, FVector blockScale, FRotator blockRotation)
	{
		return make((int32)id, location, blockScale, blockRotation);
	}*/

	FORCEINLINE UBuildableBlockInfo* makeBuildable(int32 blockID, FVector blockScale)
	{
		auto ret = NewObject<UBuildableBlockInfo>(this);
		ret->ID = blockID;
		ret->Scale = blockScale;
		ret->AddImplicitTags();
		return ret;
	}
};
