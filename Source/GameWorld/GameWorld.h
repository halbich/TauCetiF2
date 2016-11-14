
#pragma once

#include "Engine.h"
#include "GameWorldClasses.h"

#define print0(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1,0, FColor::White,text)
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5, FColor::White,text)
#define printL(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1,60, FColor::White,text)
