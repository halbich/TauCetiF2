

#pragma once
#include "Common/Enums.h"
#include "Components/WorldObjectComponent.h"
#include "WorldObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TAUCETIF2_API AWorldObject : public ADestructibleActor
{
	GENERATED_UCLASS_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Enum)
		EShapeType ShapeType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldObject)
		UWorldObjectComponent* WorldObjectComponent;


	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	FORCEINLINE void GetBoundingBox(FVector& min,FVector& max ) {
		auto location = GetActorLocation();
		auto scale = GetActorRotation().RotateVector( GetActorScale3D() * 10);

		min = location - scale;
		max = location + scale;

	}
};
