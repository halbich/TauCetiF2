#pragma once

#include "Block.h"
#include "Components/OxygenComponent.h"
#include "Components/ElectricityComponent.h"
#include "BlockWithOxygen.h"
#include "BlockWithElectricity.h"
#include "BlockWithShowableWidget.h"
#include "ControllableBlock.h"
#include "ControllerBlock.h"
#include "Info/InventoryBuildableBlockInfo.h"
#include "Info/Components/PoweredBlockInfo.h"
#include "OxygenTankFillerBlock.generated.h"

namespace OxygenFillerBlockConstants
{
	static FString CurrentFilling = TEXT("CurrentFilling");
	static FString HasItem = TEXT("HasItem");
	static FString ItemTags = TEXT("ItemTags");
}

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

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | OxygenTankFiller")
		UStaticMeshComponent* OxygenTankFillerMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | OxygenTankFiller", meta = (AllowPrivateAcces = "true"))
		UOxygenComponent* OxygenComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | OxygenTankFiller", meta = (AllowPrivateAcces = "true"))
		UElectricityComponent* ElectricityComponent;

	UPROPERTY(Transient)
		UUserWidget* shownWidget;


	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | OxygenTankFiller")
		ABlock* usedController;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | OxygenTankFiller")
		UPoweredBlockInfo* PoweredBlockInfo;

	UPROPERTY(Transient)
		UInventoryBuildableBlockInfo* currentFillingItem;

	UPROPERTY(Transient)
		UMaterialInstanceDynamic* dynInfoMat;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | OxygenTankFiller")
		UInventoryBuildableBlockInfo* TakeCurrentFillingItem(bool& success);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | OxygenTankFiller")
		bool SetCurrentFillingItem(UInventoryBuildableBlockInfo* info);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | OxygenTankFiller")
		void SetAutoregulatePower(bool newAutoregulatePower);

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	virtual void SetBlockInfo(UBlockInfo* info) override;

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

	virtual TArray<FString> GetSupportedAdditionals() override;

	FORCEINLINE virtual UElectricityComponent* GetElectricityComponent() override
	{
		return ElectricityComponent;
	}

	FDelegateHandle ListeningHandle;
	FDelegateHandle OxygenDataChangedHandle;

private:
	void ListeningOnUse(AActor* actor, bool isSpecial);

	FCriticalSection FillingItemCritical;

	FORCEINLINE void updateDisplayedMesh() {
		if (!currentFillingItem || !currentFillingItem->IsValidLowLevel())
			return;

		if (!dynInfoMat)
			return;

		dynInfoMat->SetScalarParameterValue(TEXT("Filling"), currentFillingItem->OxygenInfo->GetRemainingPercentageUnit());
	}

	FORCEINLINE void processCurrentFillingItem(float DeltaSeconds)
	{
		FillingItemCritical.Lock();

		if (currentFillingItem && currentFillingItem->IsValidLowLevelFast())
		{
			auto diff = FMath::Max(currentFillingItem->OxygenInfo->CurrentObjectMaximumOxygen - currentFillingItem->OxygenInfo->CurrentObjectOxygen, 0.0f);

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
				ensure(BlockInfo->ID == OxygenTankFillerID);

				currentFillingItem->OxygenInfo->CurrentObjectOxygen = FMath::Max(currentFillingItem->OxygenInfo->CurrentObjectOxygen + actuallyObtained, currentFillingItem->OxygenInfo->CurrentObjectMaximumOxygen);
				BlockInfo->BlockSpecificData[OxygenFillerBlockConstants::CurrentFilling] = FString::SanitizeFloat(currentFillingItem->OxygenInfo->CurrentObjectOxygen);

				updateDisplayedMesh();

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
