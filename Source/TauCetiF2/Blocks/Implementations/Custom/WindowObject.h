

#pragma once

#include "Blocks/WorldObject.h"
#include "WindowObject.generated.h"

/**
 *
 */
UCLASS()
class TAUCETIF2_API AWindowObject : public AWorldObject
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldObject)
		UStaticMeshComponent* LeftBottomCorner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldObject)
		UStaticMeshComponent* Bottom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldObject)
		UStaticMeshComponent* RightBottomCorner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldObject)
		UStaticMeshComponent* Right;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldObject)
		UStaticMeshComponent* RightTopCorner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldObject)
		UStaticMeshComponent* Top;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldObject)
		UStaticMeshComponent* LeftTopCorner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldObject)
		UStaticMeshComponent* Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldObject)
		UStaticMeshComponent* Center;


	virtual void OnConstruction(const FTransform& Transform) override;


	virtual void SetBlockInfo(UBlockInfo* info, UBlockDefinition* definition) override;

protected:

	virtual UStaticMeshComponent* GetPrimitiveComponentByName(const FName& name) override;




};
