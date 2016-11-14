#pragma once

#include "GameFramework/Actor.h"
#include "GenericBlock.h"
#include "BlockDefinition.h"
#include "Block.generated.h"

UCLASS()
class BLOCKS_API ABlock : public AActor , public IGenericBlock
{
	GENERATED_BODY()

public:
	ABlock();

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Block")
		TSubclassOf<UBlockDefinition> Definition;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReasonType) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TCF2 | Block")
	UStaticMeshComponent* GetMeshStructureComponent(int32 BlockMeshStructureDefIndex);

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

private:

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

	FORCEINLINE void setMaterial(UPrimitiveComponent* mesh, UMaterial* material, int32 index, const FVector2D& vect) {
		setMaterial(mesh, material, index, vect.X, vect.Y);
	}
};
