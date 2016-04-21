

#include "TauCetiF2.h"
#include "GameDefinitions.h"

const FVector GameDefinitions::WorldBorders(50000, 50000, 10000);
const FVector GameDefinitions::WorldCenterMapping(GameDefinitions::WorldBorders.X / 2, GameDefinitions::WorldBorders.Y / 2, 0);
const int32 GameDefinitions::CubeMinSize(20);
const FRotator GameDefinitions::WorldGrid(90, 90, 90);