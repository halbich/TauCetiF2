﻿#pragma once

#include "Block.h"
#include "Components/OxygenComponent.h"
#include "Components/ElectricityComponent.h"
#include "BlockWithOxygen.h"
#include "BlockWithElectricity.h"
#include "BlockWithShowableWidget.h"
#include "ControllableBlock.h"
#include "Info/InventoryBuildableBlockInfo.h"
#include "OxygenTankFillerBlock.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API AOxygenTankFillerBlock : public ABlock, public IBlockWithOxygen, public IBlockWithElectricity, public IBlockWithShowableWidget, public IControllableBlock
{
	GENERATED_BODY()

public:

	AOxygenTankFillerBlock();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | OxygenTankFiller")
		UStaticMeshComponent* OxygenTankFillerBodyMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | OxygenTankFiller")
		UStaticMeshComponent* OxygenTankFillerHeadMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | OxygenTankFiller", meta = (AllowPrivateAcces = "true"))
		UOxygenComponent* OxygenComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | OxygenTankFiller", meta = (AllowPrivateAcces = "true"))
		UElectricityComponent* ElectricityComponent;

	UPROPERTY(Transient)
		UUserWidget* shownWidget;

	UPROPERTY(BlueprintReadonly, Transient, Category = "TCF2 | LightBlock")
		bool IsOn;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | OxygenTankFiller")
		ABlock* usedController;

	UPROPERTY(Transient)
		UInventoryBuildableBlockInfo* currentFillingItem;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | OxygenTankFiller")
		UInventoryBuildableBlockInfo* TakeCurrentFillingItem(bool& success);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | OxygenTankFiller")
		bool SetCurrentFillingItem(UInventoryBuildableBlockInfo* info);


	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetDisplayedWidget(UUserWidget* widget) override;
	virtual UUserWidget* GetShownWidget() override;
	virtual void ShowWidget_Implementation() override;

	virtual void SetControlState_Implementation(bool isOn) override;
	virtual void SetOutputPowerPercentage_Implementation(float percentage) override;
	virtual void SetController_Implementation(ABlock* controller) override;
	virtual ABlock* GetController_Implementation() override;

	FORCEINLINE virtual UElectricityComponent* GetElectricityComponent() override
	{
		return ElectricityComponent;
	}

	FDelegateHandle ListeningHandle;
	FDelegateHandle OxygenDataChangedHandle;

private:
	void ListeningOnUse(AActor* actor, bool isSpecial);

	FCriticalSection FillingItemCritical;


	void processCurrentFillingItem(float DeltaSeconds)
	{
		FillingItemCritical.Lock();

		if (currentFillingItem && currentFillingItem->IsValidLowLevelFast())
		{
			auto diff = currentFillingItem->OxygenInfo->CurrentObjectMaximumOxygen - currentFillingItem->OxygenInfo->CurrentObjectOxygen;
			ensure(diff >= 0.0f);

			if (FMath::IsNearlyZero(diff) || FMath::IsNearlyZero(OxygenComponent->OxygenInfo->CurrentObjectOxygen))
			{
				FillingItemCritical.Unlock();
				return;
			}

			auto elapsedSeconds = DeltaSeconds * GameDefinitions::GameDayMultiplier;
			auto max = OxygenComponent->GetDefinition()->MaxConsumedOxygenPerGameSecond;
			auto possibleOxygen = elapsedSeconds * max;	// now we can withdraw only this amount;

			auto toWithdraw = FMath::Min(diff, possibleOxygen);

			float actuallyObtained = 0;
			float actuallyPutted = 0;
			float acuallyReturned = 0;
			if (OxygenComponent->ObtainAmount(toWithdraw, actuallyObtained)) {
				currentFillingItem->OxygenInfo->CurrentObjectOxygen += actuallyObtained;
				

				// todo update mesh

				auto returnOxygen = actuallyObtained - toWithdraw;
				if (returnOxygen >= 0.0f)
					OxygenComponent->PutAmount(returnOxygen, actuallyPutted);
			}

		}

		FillingItemCritical.Unlock();
	}

protected:
	UFUNCTION()
		void ListeningOnOxygenCompChanged(UBlockWithOxygenInfo* source);

};
