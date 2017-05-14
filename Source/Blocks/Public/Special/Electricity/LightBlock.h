#pragma once

#include "Block.h"
#include "Interfaces/BlockWithShowableWidget.h"
#include "Components/ElectricityComponent.h"
#include "BlockWithElectricity.h"
#include "ControllableBlock.h"
#include "ControllerBlock.h"
#include "Info/Components/PoweredBlockInfo.h"
#include "LightBlock.generated.h"

#define LIGHT_TURN_ON NSLOCTEXT("TCF2LocSpace", "LC.LightBlock.TurnOn", "Zapnout / Nastavit")

#define LIGHT_TURN_OFF NSLOCTEXT("TCF2LocSpace", "LC.LightBlock.TurnOff", "Vypnout / Nastavit")

#define LIGHT_TURN_USE NSLOCTEXT("TCF2LocSpace", "LC.LightBlock.TurnOff", " --- / Nastavit")

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

	UPROPERTY(Transient)
		UUserWidget* shownWidget;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | LightBlock")
		ABlock* usedController;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | LightBlock")
		UPoweredBlockInfo* PoweredBlockInfo;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | LightBlock")
		void SetAutoregulatePower(bool newAutoregulatePower);

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaSeconds) override;

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

private:

	FDelegateHandle ListeningHandle;

	void ListeningOnUse(AActor* actor, bool isSpecial);

	void updateLightByConsumption(float consumption, float max)
	{
		if (LightComp && LightComp->IsValidLowLevelFast())
			LightComp->SetIntensity(FMath::IsNearlyZero(max) ? 0 : ((consumption / max) * 5000.0f));
	}

	FORCEINLINE void updateUsingMessage()
	{
		SelectTargetComponent->CustomUsingMessage = usedController != NULL ? LIGHT_TURN_USE : (PoweredBlockInfo->IsOn ? LIGHT_TURN_OFF : LIGHT_TURN_ON);
	}
};
