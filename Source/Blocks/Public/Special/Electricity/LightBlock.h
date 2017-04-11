#pragma once

#include "Block.h"
#include "Components/ElectricityComponent.h"
#include "BlockWithElectricity.h"
#include "LightBlock.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API ALightBlock : public ABlock, public IBlockWithElectricity
{
	GENERATED_BODY()

public:
	ALightBlock();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | LightBlock", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* LightBlockMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | LightBlock", meta = (AllowPrivateAccess = "true"))
		UPointLightComponent* LightComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | LightBlock", meta = (AllowPrivateAcces = "true"))
		UElectricityComponent* ElectricityComponent;

	UPROPERTY(BlueprintReadWrite, Category = "TCF2 | LightBlock")
		bool AutoregulatePowerOutput;

	virtual void Tick(float DeltaSeconds) override;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	FORCEINLINE virtual UElectricityComponent* GetElectricityComponent() override
	{
		return ElectricityComponent;
	}

	UPROPERTY(Transient)
		float dayMultiplier;

private:
	FORCEINLINE float getAutoregulatedPower(const float p, const float max)
	{
		if (p >= 50.0f)
			return max;

		if (p >= 25)
			return max / 2.0f;

		return max / 10.0f;
	}

	void updateLightByConsumption(float consumption, float max)
	{
		if (LightComp && LightComp->IsValidLowLevelFast())
			LightComp->SetIntensity(FMath::IsNearlyZero(max) ? 0 : ((consumption / max) * 5000.0f));
	}
};
