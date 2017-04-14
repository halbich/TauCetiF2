#include "Blocks.h"
#include "WindowBlock.h"

AWindowBlock::AWindowBlock()
	:Super()
{
	PrimaryActorTick.bCanEverTick = false;

	ElectricityComponent = CreateDefaultSubobject<UElectricityComponent>(TEXT("ElectricityComponent"));
	AddOwnedComponent(ElectricityComponent);
}