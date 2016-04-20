// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#ifndef __TAUCETIF2_H__
#define __TAUCETIF2_H__



#include "Engine.h"
#include "Common/Enums.h"
#include "Game/GameDefinitions.h"
#include "Helpers/Helpers.h"
#include "Helpers/Saving/ArchiveHelpers.h"

/* Stencil index mapping to PP_OutlineColored  http://www.tomlooman.com/multi-color-outline-post-process-in-unreal-engine-4/ */
#define STENCIL_FRIENDLY_OUTLINE 252;
#define STENCIL_NEUTRAL_OUTLINE 253;
#define STENCIL_USE_HIGHLIGHT 254;
#define STENCIL_ITEM_HIGHLIGHT 255;

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1,10, FColor::White,text)
#define printL(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1,60, FColor::White,text)


#endif
