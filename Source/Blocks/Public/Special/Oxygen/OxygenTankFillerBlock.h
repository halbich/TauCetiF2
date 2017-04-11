#pragma once

#include "Block.h"
#include "Components/OxygenComponent.h"
#include "Components/ElectricityComponent.h"
#include "BlockWithOxygen.h"
#include "BlockWithElectricity.h"
#include "BlockWithShowableWidget.h"
#include "OxygenTankFillerBlock.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API AOxygenTankFillerBlock : public ABlock, public IBlockWithOxygen, public IBlockWithElectricity, public IBlockWithShowableWidget
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

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetDisplayedWidget(UUserWidget* widget) override;

	virtual UUserWidget* GetShownWidget() override;

	FORCEINLINE virtual UElectricityComponent* GetElectricityComponent() override
	{
		return ElectricityComponent;
	}

	FDelegateHandle ListeningHandle;
	FDelegateHandle OxygenDataChangedHandle;



private:
	void ListeningOnUse(AActor* actor, bool isSpecial);

protected:
	UFUNCTION()
		void ListeningOnOxygenCompChanged(UBlockWithOxygenInfo* source);

};
