

#pragma once

#include "Block.h"
#include "Switcher.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKS_API ASwitcher : public ABlock
{
	GENERATED_BODY()
	
public:
	ASwitcher();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | SwitcherBlock")
		UStaticMeshComponent* SwitcherMesh;


	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | SwitcherBlock", meta = (AllowPrivateAcces = "true"))
		UElectricityComponent* ElectricityComponent;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FDelegateHandle ListeningHandle;

private:
	void ListeningOnUse(AActor* actor, bool isSpecial);
};
