#pragma once

#include "Block.h"
#include "Interfaces/BlockWithShowableWidget.h"
#include "Components/ElectricityComponent.h"
#include "BlockWithElectricity.h"
#include "Commons/Public/TCF2GameInstance.h"
#include "ControllableBlock.h"
#include "LightBlock.generated.h"


namespace LightBlockConstants
{
	static FString IsAutoregulated = TEXT("IsAutoregulated");
}

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

	UPROPERTY(BlueprintReadonly, Category = "TCF2 | LightBlock")
		bool AutoregulatePowerOutput;

	UPROPERTY(Transient)
		float isDaytime;

	UPROPERTY(Transient)
		UUserWidget* shownWidget;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | LightBlock")
		ABlock* usedController;


	virtual void OnConstruction(const FTransform& Transform) override;
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
	virtual void ShowWidget_Implementation() override;

	FORCEINLINE virtual UElectricityComponent* GetElectricityComponent() override
	{
		return ElectricityComponent;
	}

	virtual void SetBlockInfo(UBlockInfo* info) override;

	UFUNCTION()
		void OnNightChanged(bool isNight);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | LightBlock")
		void UpdateAutoregulate(bool newAutoregulate);

private:

	FDelegateHandle ListeningHandle;

	void ListeningOnUse(AActor* actor, bool isSpecial);

	FORCEINLINE float getAutoregulatedPower(const float p)
	{
		if (isDaytime)
			return 0.0f;

		if (p >= 50.0f)
			return 1.0f;

		if (p >= 25)
			return .5f;

		return 0.1f;
	}

	void updateLightByConsumption(float consumption, float max)
	{
		if (LightComp && LightComp->IsValidLowLevelFast())
			LightComp->SetIntensity(FMath::IsNearlyZero(max) ? 0 : ((consumption / max) * 5000.0f));
	}
};
