#include "Blocks.h"
#include "WindowBlock.h"

AWindowBlock::AWindowBlock()
	:Super()
{
	PrimaryActorTick.bCanEverTick = false;

	GetRootComponent()->SetMobility(EComponentMobility::Stationary);

	ElectricityComponent = CreateDefaultSubobject<UElectricityComponent>(TEXT("ElectricityComponent"));
	AddOwnedComponent(ElectricityComponent);
}