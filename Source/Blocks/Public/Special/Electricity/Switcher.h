#pragma once

#include "Block.h"
#include "Interfaces/BlockWithShowableWidget.h"
#include "Components/ElectricityComponent.h"
#include "BlockWithElectricity.h"
#include "ControllerBlock.h"
#include "ControllableBlock.h"
#include "Commons/Public/Enums.h"
#include "Switcher.generated.h"


namespace SwitcherBlockConstants
{
	static FString IsOn = TEXT("IsON");
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

	UPROPERTY(BlueprintReadOnly, Category = "TCF2 | SwitcherBlock")
		bool IsOn;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual bool BindControl_Implementation(ABlock* controllableBlock) override;

	virtual bool UnbindControl_Implementation(ABlock* controllableBlock) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetDisplayedWidget(UUserWidget* widget) override;
	virtual UUserWidget* GetShownWidget() override;
	virtual void ShowWidget_Implementation() override;

	virtual void SetBlockInfo(UBlockInfo* info) override;

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
		mat->SetScalarParameterValue(TEXT("IsOn"), IsOn);
	}
};
