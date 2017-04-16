#include "Commons.h"
#include "GameDefinitions.h"

const FVector GameDefinitions::WorldBorders(50000, 50000, 10000);
const FVector GameDefinitions::WorldCenterMapping(GameDefinitions::WorldBorders.X / 2, GameDefinitions::WorldBorders.Y / 2, 0);
const int32 GameDefinitions::CubeMinSize(20);
const int32 GameDefinitions::CubeMinSizeHalf(GameDefinitions::CubeMinSize / 2);
const FRotator GameDefinitions::WorldGrid(90, 90, 90);
const int32 GameDefinitions::CubeMaxSizeMultiplier(20);
const int32 GameDefinitions::DefaultInventoryTagGroupsCount(10);
const float GameDefinitions::CubeSurfaceInMetersSquared((GameDefinitions::CubeMinSize / 100.0f) * (GameDefinitions::CubeMinSize / 100.0f));

const float GameDefinitions::GameDayLength(3600);
const float GameDefinitions::GameDayMultiplier(86400.0f / GameDefinitions::GameDayLength);


const float GameDefinitions::RainHitpointToEnergy(1000);
const float GameDefinitions::EnergyToRainHitpoint(1.0f / GameDefinitions::RainHitpointToEnergy);

const float GameDefinitions::EnergyToOgygen(10);
const float GameDefinitions::OxygenToEnergy(1.0f / GameDefinitions::EnergyToOgygen);

const float GameDefinitions::EnergyToHealth(.002f);
const float GameDefinitions::HealthToEnergy(1.0f / GameDefinitions::EnergyToHealth);