#pragma once

#include "Object.h"
#include "UGameSaveInterface.generated.h"

UINTERFACE(BlueprintType)
class GAMESAVE_API UGameSaveInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class GAMESAVE_API IGameSaveInterface
{
	GENERATED_IINTERFACE_BODY()

public:

	//classes using this interface must implement SaveGame
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TCF2 | Game Save")
		bool SaveGame();

	//classes using this interface must implement LoadGame
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TCF2 | Game Save")
		bool LoadGame();
};
