#include "Blocks.h"
#include "BlockWithElectricity.h"

UBlockWithElectricity::UBlockWithElectricity(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void IBlockWithElectricity::UpdateHealth(UElectricityComponent* comp, UBlockInfo* info)
{
}

float IBlockWithElectricity::WasHitByStorm(const float amount)
{
	float energyToRemove = amount *  GameDefinitions::RainHitpointToEnergy;

	if (FMath::IsNearlyZero(energyToRemove))
		return energyToRemove;

	float actuallyObtained = 0;
	if (GetElectricityComponent()->ObtainAmount(energyToRemove, actuallyObtained))
		energyToRemove -= actuallyObtained;

	return energyToRemove * GameDefinitions::EnergyToRainHitpoint;	// we want to return remaining rain hitpoints
}