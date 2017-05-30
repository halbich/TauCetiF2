#pragma once

#include "Block.h"
#include "Components/ElectricityComponent.h"
#include "Components/Electricity/ElectricNetwork.h"
#include "BlockWithElectricity.h"
#include "Commons/Public/CommonHelpers.h"
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
		bool AnimationEnabled;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TCF2 | GeneratorBlock")
		UParticleSystem* ParticleEmitter;

	UPROPERTY(Transient)
		UWorld* world;

	UPROPERTY(Transient)
		TArray<UParticleSystemComponent*> particles;

	UPROPERTY(Transient)
		FVector Scale;


	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	virtual void WasHitByStorm(const FVector& blockHitLocation, const float amount, const FVector& hitWorldLocation) override;

	FORCEINLINE virtual UElectricityComponent* GetElectricityComponent() override
	{
		return ElectricityComponent;
	}

	FORCEINLINE int32 getParticleIndex(FVector scale)
	{
		return FMath::FloorToInt(scale.X + Scale.X * scale.Y);
	}
};
