#pragma once

#include "GameFramework/Actor.h"
#include "GenericBlock.h"
#include "BlockDefinition.h"
#include "BlockAdditionalFlags.h"
#include "SelectTargetComponent.h"
#include "BlockInfo.h"
#include "BlockWithElectricity.h"
#include "BlockWithOxygen.h"
#include "Components/WorldObjectComponent.h"
#include "Components/ElectricityComponent.h"
#include "Block.generated.h"

class ABlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDestroyRequestDelegate, ABlock*, block);

UCLASS()
class BLOCKS_API ABlock : public AActor, public IGenericBlock
{
	GENERATED_BODY()

public:
	ABlock();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | Block")
		TSubclassOf<UBlockDefinition> Definition;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Block")
		UBlockInfo* BlockInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TCF2 | Block", meta = (AllowPrivateAccess = "true"))
		USelectTargetComponent* SelectTargetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TCF2 | Block", meta = (AllowPrivateAccess = "true"))
		UWorldObjectComponent* WorldObjectComponent;

	UPROPERTY(BlueprintAssignable, Category = "TCF2 | Block")
		FDestroyRequestDelegate OnDestroyRequestedEvent;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TCF2 | Block")
		UStaticMeshComponent* GetMeshStructureComponent(int32 BlockMeshStructureDefIndex);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TCF2 | Block")
		UPrimitiveComponent* GetComponentForObjectOutline();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TCF2 | Block")
		void UpdateBlockOnConstruction(UBlockDefinition* BlockDef);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | Block")
		UTexture2D* GetDefaultTexture();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | Block")
		bool GetIsController();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReasonType) override;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	virtual void UpdateBlockOnConstruction_Implementation(UBlockDefinition* BlockDef) override;

	virtual FVector GetBlockScale();

	virtual void SetBlockInfo(UBlockInfo* info);

	FORCEINLINE bool IsInUsableArea(AActor* owner)
	{
		return SelectTargetComponent && SelectTargetComponent->IsValidLowLevel() && SelectTargetComponent->IsInUsableArea(owner);
	}

	void InitWorldObjectComponent();

	virtual void WasHitByStorm(const FVector& blockHitLocation, const float amount);

	virtual UPatternGroupInfo* GetPatternGroupImpl();

	void RenewPatternInfo();

	FORCEINLINE UElectricityComponent* TryGetElectricityComp()
	{
		auto _t = Cast<IBlockWithElectricity>(this);
		if (!_t)
			return NULL;
		return _t->GetElectricityComponent();
	}

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

	FORCEINLINE float buildingCoeficient(UBlockDefinition* def)
	{
		switch (def->ShapeType)
		{
		case EShapeType::Empty: return 1.0f;
		case EShapeType::CubeBody: return 0.5f;
		case EShapeType::CubeSide: return (1.0f / 3.0f);

		default: return 1.0f;
		}
	}

	FORCEINLINE void HealthUpdated()
	{
		auto lastSev = BlockInfo->HealthSeverity;
		if (!BlockInfo->UpdateHealthSeverity())
			return;	// severity has not changed

		auto electricity = Cast<IBlockWithElectricity>(this);
		if (electricity)
			electricity->RefreshHealthSeverity(lastSev);
	}
};
