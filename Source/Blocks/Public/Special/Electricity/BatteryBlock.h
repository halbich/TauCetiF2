#pragma once

#include "Block.h"
#include "Components/ElectricityComponent.h"
#include "BlockWithElectricity.h"
#include "BatteryBlock.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API ABatteryBlock : public ABlock, public IBlockWithElectricity
{
	GENERATED_BODY()

public:

	ABatteryBlock();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | BatteryBlock")
		UStaticMeshComponent* BatteryBlockMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | BatteryBlock", meta = (AllowPrivateAcces = "true"))
		UElectricityComponent* ElectricityComponent;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FORCEINLINE virtual UElectricityComponent* GetElectricityComponent() override
	{
		return ElectricityComponent;
	}


protected:
	UFUNCTION()
		void ListeningOnElectricityCompChanged(UBlockWithElectricityInfo* source);
};
