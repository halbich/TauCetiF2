// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "TauCetiF2GameMode.generated.h"

UCLASS(minimalapi)
class ATauCetiF2GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATauCetiF2GameMode();

	UPROPERTY(BlueprintReadOnly, Category = GameMode)
		TSubclassOf<APawn> PawnToSpawn;
};



