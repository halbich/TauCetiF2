

#pragma once

#include "Block.h"
#include "Interfaces/BlockWithShowableWidget.h"
#include "Components/ElectricityComponent.h"
#include "BlockWithElectricity.h"
#include "Switcher.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKS_API ASwitcher : public ABlock, public IBlockWithShowableWidget, public IBlockWithElectricity
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

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetDisplayedWidget(UUserWidget* widget) override;

	virtual UUserWidget* GetShownWidget() override;

	FORCEINLINE virtual UElectricityComponent* GetElectricityComponent() override
	{
		return ElectricityComponent;
	}

	FDelegateHandle ListeningHandle;

private:
	void ListeningOnUse(AActor* actor, bool isSpecial);

	

};
