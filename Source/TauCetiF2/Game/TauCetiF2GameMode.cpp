// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TauCetiF2.h"
#include "TauCetiF2GameMode.h"
#include "TauCetiF2Character.h"

ATauCetiF2GameMode::ATauCetiF2GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		PawnToSpawn = PlayerPawnBPClass.Class;
		DefaultPawnClass = nullptr;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Blueprints/Common/TCF2PlayerController"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}