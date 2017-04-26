#pragma once
#include "Components/ElectricityComponent.h"
#include "Info/BlockInfo.h"
#include "BlockWithElectricity.generated.h"

UINTERFACE(BlueprintType)
class BLOCKS_API UBlockWithElectricity : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class BLOCKS_API IBlockWithElectricity
{
	GENERATED_IINTERFACE_BODY()

public:

	// Try to "catch" hit withing electricity buffer. Returns remaining hitpoints.
	float WasHitByStorm(const float amount);

	virtual UElectricityComponent* GetElectricityComponent() = 0;


	void RefreshHealthSeverity(EHealthSeverity oldSeverity);

	virtual void NetworkChanged();
};