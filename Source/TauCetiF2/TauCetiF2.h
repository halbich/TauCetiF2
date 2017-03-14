// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#ifndef __TAUCETIF2_H__
#define __TAUCETIF2_H__

#include "Engine.h"
#include "Commons/Public/Enums.h"
#include "Commons/Public/GameDefinitions.h"
#include "Helpers/Helpers.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5, FColor::White,text)
#define printL(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1,60, FColor::White,text)

#endif
