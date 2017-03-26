#pragma once
#include "Block.h"
#include "Components/ElectricityComponent.h"
#include "EnergyBlobBlock.generated.h"

UCLASS()
class BLOCKS_API AEnergyBlobBlock : public ABlock
{
	GENERATED_BODY()

public:
	AEnergyBlobBlock();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | EnergyBlobBlock")
		UStaticMeshComponent* BlobBlockMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | EnergyBlobBlock", meta = (AllowPrivateAcces = "true"))
		UElectricityComponent* ElectricityComponent;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FDelegateHandle ListeningHandle;

private:
	void ListeningOnUse(AActor* actor, bool isSpecial);
};
