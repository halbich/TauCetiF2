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
	float healthToRemove = amount;

	if (FMath::IsNearlyZero(healthToRemove))
		return healthToRemove;

	float actuallyObtained = 0;
	if (GetElectricityComponent()->ObtainAmount(healthToRemove, actuallyObtained))
		healthToRemove -= actuallyObtained;

	return healthToRemove;
}