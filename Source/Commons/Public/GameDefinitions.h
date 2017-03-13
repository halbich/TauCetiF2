#pragma once

/**
 *
 */
class COMMONS_API GameDefinitions
{
public:
	static const FVector WorldCenterMapping;
	static const FVector WorldBorders;
	static const int32 CubeMinSize;
	static const int32 CubeMinSizeHalf;
	static const FRotator WorldGrid;
	static const int32 CubeMaxSizeMultiplier;
};

/* Stencil index mapping to PP_OutlineColored  http://www.tomlooman.com/multi-color-outline-post-process-in-unreal-engine-4/ */
#define STENCIL_MINIMUM_USABLE_EXCLUDING 252
#define STENCIL_DELETE_OUTLINE 253
#define STENCIL_USE_HIGHLIGHT 254
#define STENCIL_ITEM_HIGHLIGHT 255

/* BlockIDs*/
#define EmptyHandID -10
#define DeleteID -5

#define CubeBaseID 0
#define CubeBuildingID 1
#define CubePolycarbonateID 2
#define CubeSideBaseID 100
#define CubeSideBuildingID 101
#define CubeBodyBuildingID 201

#define TerminalID 500
#define WindowID 600

#define DoorID 700

#define EnergyBlobID 999
#define LightSmallID 1000
#define GeneratorID 1100
#define BatteryID 1200

#define OxygenTankFiller 1400
#define OxygenTank  1500
