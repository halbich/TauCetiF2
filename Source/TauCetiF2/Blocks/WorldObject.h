

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


	virtual void SetBlockInfo(UBlockInfo* info, FBlockDefinition* definition);

protected:

	virtual UStaticMeshComponent* GetPrimitiveComponentByName(const FName& name);

	FORCEINLINE void setMaterial(UPrimitiveComponent* mesh, UMaterial* material, int32 index, float scaleX, float scaleY) {

		UMaterialInstanceDynamic* mi = nullptr;
		if (material) {
			mi = UMaterialInstanceDynamic::Create(material, this);
			if (mi)
			{
				mi->SetScalarParameterValue(TEXT("Kx"), scaleX);
				mi->SetScalarParameterValue(TEXT("Ky"), scaleY);
			}
		}
		mesh->SetMaterial(index, mi);
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

	 void setMaterial(UPrimitiveComponent* mesh,EMaterialInstance& instance, int32 index, FVector2D scale) {
		UMaterial* material = UHelpers::GetMaterialByInstance(instance);

		check(material && "Failed to find given material");

		setMaterial(mesh, material, index, scale.X, scale.Y);
	}

	FORCEINLINE void setConstructionMaterial(UPrimitiveComponent* mesh, EMaterialInstance& instance, int32 index, FVector2D scale) {
		UMaterial* material = UHelpers::GetMaterialByInstance(instance, true);

		check(material && "Failed to find given material");

		setMaterial(mesh,material, index, scale.X, scale.Y);
	}

	FORCEINLINE void constructorSetMeshes(UStaticMesh* mesh)
	{
		auto mc = GetStaticMeshComponent();
		checkf(mc, TEXT("Failed to find mesh component"));

		mc->SetStaticMesh(mesh);
		TranslucentSelectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TranslucentMeshSelector"));
		TranslucentSelectMesh->SetStaticMesh(mesh);
		TranslucentSelectMesh->SetRenderInMainPass(false);
		TranslucentSelectMesh->Deactivate();
		TranslucentSelectMesh->SetupAttachment(mc);
	}
};
