#pragma once

#include "Block.h"
#include "Interfaces/BlockWithShowableWidget.h"
#include "Components/ElectricityComponent.h"
#include "BlockWithElectricity.h"
#include "Commons/Public/TCF2GameInstance.h"
#include "ControllableBlock.h"
#include "LightBlock.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API ALightBlock : public ABlock, public IBlockWithShowableWidget, public IBlockWithElectricity, public IControllableBlock
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

	UPROPERTY(Transient)
		float isDaytime;

	UPROPERTY(Transient)
		UUserWidget* shownWidget;

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	virtual void SetControlState_Implementation(bool isOn) override;
	virtual void SetOutputPowerPercentage_Implementation(float percentage) override;
	virtual void SetController_Implementation(ABlock* controller) override;
	virtual ABlock* GetController_Implementation() override;

	virtual void SetDisplayedWidget(UUserWidget* widget) override;

	virtual UUserWidget* GetShownWidget() override;

	FORCEINLINE virtual UElectricityComponent* GetElectricityComponent() override
	{
		return ElectricityComponent;
	}

	virtual void SetBlockInfo(UBlockInfo* info);

	UFUNCTION()
		void OnNightChanged(bool isNight);

private:

	void ListeningOnUse(AActor* actor, bool isSpecial);

	FORCEINLINE float getAutoregulatedPower(const float p, const float max)
	{
		if (isDaytime)
			return 0.0f;

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
