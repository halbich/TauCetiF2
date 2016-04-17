

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

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Enum)
		EShapeType ShapeType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldObject)
		UWorldObjectComponent* WorldObjectComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldObject)
		USelectTargetComponent* SelectTargetComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldObject)
		UStaticMeshComponent* TranslucentSelectMesh;


	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	virtual void GetBoundingBox(FVector& min, FVector& max);

	static FORCEINLINE void GetBoundingBox(const FTransform& transform, FVector& min, FVector& max) {
		auto location = transform.GetLocation();
		auto scale = transform.GetRotation().RotateVector(transform.GetScale3D() * UHelpers::CubeMinSize * 0.5);

		auto tmin = location - scale;
		auto tmax = location + scale;
		min = FVector(FMath::Min(tmin.X, tmax.X), FMath::Min(tmin.Y, tmax.Y), FMath::Min(tmin.Z, tmax.Z));
		max = FVector(FMath::Max(tmin.X, tmax.X), FMath::Max(tmin.Y, tmax.Y), FMath::Max(tmin.Z, tmax.Z));
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


	FORCEINLINE void setTranslucentMaterials(int32 count) {
		auto sm = TranslucentSelectMesh;
		if (!sm)
			return;

		UMaterial* material = UHelpers::GetMaterialByInstance(EMaterialInstance::Empty);
		check(material && "Failed to find given material");

		for (size_t i = 0; i < count; i++)
		{
			UMaterialInstanceDynamic* mi = nullptr;
			if (material) {
				mi = UMaterialInstanceDynamic::Create(material, this);
				if (mi)
				{
					mi->SetScalarParameterValue(TEXT("Kx"), 1);
					mi->SetScalarParameterValue(TEXT("Ky"), 1);
				}
			}
			sm->SetMaterial(i, mi);
		}
	}

	FORCEINLINE void setMaterial(EMaterialInstance& instance, int32 index, FVector2D scale) {
		UMaterial* material = UHelpers::GetMaterialByInstance(instance);

		check(material && "Failed to find given material");

		setMaterial(material, index, scale.X, scale.Y);
	}

	FORCEINLINE void setConstructionMaterial(EMaterialInstance& instance, int32 index, FVector2D scale) {
		UMaterial* material = UHelpers::GetMaterialByInstance(instance, true);

		check(material && "Failed to find given material");

		setMaterial(material, index, scale.X, scale.Y);
	}


};
