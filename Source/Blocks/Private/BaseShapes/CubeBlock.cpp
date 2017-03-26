#include "Blocks.h"
#include "CubeBlock.h"

ACubeBlock::ACubeBlock()
	: Super()
{
	ElectricityComponent = CreateDefaultSubobject<UElectricityComponent>(TEXT("ElectricityComponent"));
	AddOwnedComponent(ElectricityComponent);
}

