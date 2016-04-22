

#pragma once

/**
 * 
 */
class TAUCETIF2_API GameDefinitions
{
public:
	static const FVector WorldCenterMapping;
	static const FVector WorldBorders;
	static const int32 CubeMinSize;
	static const FRotator WorldGrid;
};


/* Stencil index mapping to PP_OutlineColored  http://www.tomlooman.com/multi-color-outline-post-process-in-unreal-engine-4/ */
#define STENCIL_MINIMUM_USABLE_EXCLUDING 251
#define STENCIL_FRIENDLY_OUTLINE 252
#define STENCIL_DELETE_OUTLINE 253
#define STENCIL_USE_HIGHLIGHT 254
#define STENCIL_ITEM_HIGHLIGHT 255