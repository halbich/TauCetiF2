

#pragma once
#include "Common/Enums.h"
#include "Components/WorldObjectComponent.h"
#include "Components/SelectTargetComponent.h"

#include "WorldObject.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class TAUCETIF2_API AWorldObject : public AStaticMeshActor //ADestructibleActor
{
	GENERATED_UCLASS_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Enum)
		EShapeType ShapeType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldObject)
		UWorldObjectComponent* WorldObjectComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldObject)
		USelectTargetComponent* SelectTargetComponent;
	


	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	FORCEINLINE void GetBoundingBox(FVector& min, FVector& max) {
		auto location = GetActorLocation();
		auto scale = GetActorRotation().RotateVector(GetActorScale3D() * 10);

		min = location - scale;
		max = location + scale;

	}


	FORCEINLINE void setMaterial(UMaterial* material, int32 index, float scaleX, float scaleY) {
		auto sm = GetStaticMeshComponent();
		if (!sm)
			return;

		UMaterialInstanceDynamic* mi = nullptr;
		if (material) {
			mi = UMaterialInstanceDynamic::Create(material, this);
			if (mi)
			{
				mi->SetScalarParameterValue(TEXT("Kx"), scaleX);
				mi->SetScalarParameterValue(TEXT("Ky"), scaleY);
			}
		}
		sm->SetMaterial(index, mi);
	}
};
