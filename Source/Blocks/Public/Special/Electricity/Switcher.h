#pragma once

#include "Block.h"
#include "Interfaces/BlockWithShowableWidget.h"
#include "Components/ElectricityComponent.h"
#include "BlockWithElectricity.h"
#include "ControllerBlock.h"
#include "ControllableBlock.h"
#include "Commons/Public/Enums.h"
#include "Commons/Public/TCF2GameInstance.h"
#include "Info/Components/PoweredBlockInfo.h"
#include "Switcher.generated.h"


namespace SwitcherBlockConstants
{
	static FString ReactsToDayCycle = TEXT("ReactsToDayCycle");
	static FString StateAtDay = TEXT("StateAtDay");
	static FString StateAtNight = TEXT("StateAtNight");

}

/**
 *
 */
UCLASS()
class BLOCKS_API ASwitcher : public ABlock, public IBlockWithShowableWidget, public IBlockWithElectricity, public IControllerBlock
{
	GENERATED_BODY()

public:
	ASwitcher();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | SwitcherBlock")
		UStaticMeshComponent* SwitcherMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | SwitcherBlock", meta = (AllowPrivateAcces = "true"))
		UElectricityComponent* ElectricityComponent;

	UPROPERTY(Transient)
		UUserWidget* shownWidget;

	UPROPERTY(Transient)
		TArray<ABlock*> controlledBlocks;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | SwitcherBlock")
		UPoweredBlockInfo* PoweredBlockInfo;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | SwitcherBlock")
		bool FlipCurrentOnState();

	UPROPERTY(BlueprintReadonly, Transient, Category = "TCF2 | SwitcherBlock")
		bool ReactsToDayCycle;

	UPROPERTY(BlueprintReadonly, Transient, Category = "TCF2 | SwitcherBlock")
		bool StateAtDay;

	UPROPERTY(BlueprintReadonly, Transient, Category = "TCF2 | SwitcherBlock")
		bool StateAtNight;


	UFUNCTION()
		void OnNightChanged(bool isNight, bool calledByLevelLoad);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | SwitcherBlock")
		void SetReactsToDayCycle(bool newReactsToDayCycle);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | SwitcherBlock")
		void SetStateAtDay(bool newStateAtDay);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | SwitcherBlock")
		void SetStateAtNight(bool newStateAtNight);

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual bool BindControl_Implementation(ABlock* controllableBlock) override;
	virtual bool UnbindControl_Implementation(ABlock* controllableBlock) override;
	virtual bool GetControlState_Implementation() override;
	virtual TArray<ABlock*> GetControlledBlocks_Implementation() override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetDisplayedWidget(UUserWidget* widget) override;
	virtual UUserWidget* GetShownWidget() override;
	virtual void ShowWidget_Implementation() override;

	virtual void SetBlockInfo(UBlockInfo* info) override;

	virtual TArray<FString> GetSupportedAdditionals() override;

	FORCEINLINE virtual UElectricityComponent* GetElectricityComponent() override
	{
		return ElectricityComponent;
	}

	



	FDelegateHandle ListeningHandle;

private:
	void ListeningOnUse(AActor* actor, bool isSpecial);

	void updateDynamicColor()
	{
		auto mat = Cast<UMaterialInstanceDynamic>(SwitcherMesh->GetMaterial(2));
		if (!mat)
			return;

		mat->SetScalarParameterValue(TEXT("IsConnected"), controlledBlocks.Num());
		mat->SetScalarParameterValue(TEXT("IsOn"), PoweredBlockInfo->IsOn);
	}
};
