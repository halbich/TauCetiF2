#pragma once

#include "Block.h"
#include "Components/ElectricityComponent.h"
#include "BlockWithElectricity.h"
#include "Interfaces/BlockWithShowableWidget.h"
#include "TerminalBlock.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API ATerminalBlock : public ABlock, public IBlockWithShowableWidget
{
	GENERATED_BODY()

public:
	ATerminalBlock();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | TerminalBlock")
		UStaticMeshComponent* TerminalBlockMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | TerminalBlock", meta = (AllowPrivateAcces = "true"))
		UElectricityComponent* ElectricityComponent;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FDelegateHandle ListeningHandle;

private:
	void ListeningOnUse(AActor* actor, bool isSpecial);
};
