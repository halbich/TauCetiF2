#pragma once

#include "Block.h"
#include "Components/ElectricityComponent.h"
#include "BlockWithElectricity.h"
#include "GeneratorBlock.generated.h"

USTRUCT()
struct FHittedSpot
{
	GENERATED_BODY()

		UPROPERTY(Transient)
		uint8 X;

	UPROPERTY(Transient)
		uint8 Y;

	UPROPERTY(Transient)
		float ActualTime;
};

/**
 *
 */
UCLASS()
class BLOCKS_API AGeneratorBlock : public ABlock, public IBlockWithElectricity
{
	GENERATED_BODY()

public:
	AGeneratorBlock();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | GeneratorBlock")
		UStaticMeshComponent* GeneratorMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | GeneratorBlock", meta = (AllowPrivateAcces = "true"))
		UElectricityComponent* ElectricityComponent;

	UPROPERTY(Transient)
		UTexture2D* DynamicTexture;

	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	virtual void WasHitByStorm(const FVector& blockHitLocation, const float amount) override;

	FORCEINLINE virtual UElectricityComponent* GetElectricityComponent() override
	{
		return ElectricityComponent;
	}

	UPROPERTY(Transient)
		TArray<FHittedSpot> spots;

private:

	TArray<class UMaterialInstanceDynamic*> dynamicMaterials;
	FUpdateTextureRegion2D* updateTextureRegion;
	uint8* dynamicColors;
	float* dynamicColorsFloat;
	uint32 dataSize;
	uint32 dataSqrtSize;

	int32 rowSize;
	int32 arraySize;

	void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData);

	void UpdateCustomTexture();

	int32 pixelsPerBaseBlock;
};
