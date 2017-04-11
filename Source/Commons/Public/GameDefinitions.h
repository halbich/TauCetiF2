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
	static const int32 DefaultInventoryTagGroupsCount;
	static const float CubeSurfaceInMetersSquared;
	static const float GameDayLength;
};

/* Stencil index mapping to PP_OutlineColored  http://www.tomlooman.com/multi-color-outline-post-process-in-unreal-engine-4/ */
#define STENCIL_MINIMUM_USABLE_EXCLUDING 252
#define STENCIL_DELETE_OUTLINE 253
#define STENCIL_USE_HIGHLIGHT 254
#define STENCIL_ITEM_HIGHLIGHT 255

/* BlockIDs*/
#define EmptyHandID -10
#define DeleteID -5
#define ElectricToolID -4

#define CubeBaseID 0
#define CubeBuildingID 1
#define CubePolycarbonateID 2
#define CubeSideBaseID 100
#define CubeSideBuildingID 101
#define CubeBodyBuildingID 201

#define TerminalID 500
#define WindowID 600
#define DoorID 700


#define LightSmallID 1000
#define SwitcherID 1050
#define GeneratorID 1100
#define CreatorID 1150
#define BatteryID 1200

#define OxygenTankFillerID 1400
#define OxygenTankID  1500
