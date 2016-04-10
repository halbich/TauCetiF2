

#pragma once

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


	virtual void PostInitProperties() override;
	virtual void PostActorCreated() override;
	virtual void PostLoad() override;

	virtual void OnConstruction(const FTransform& Transform) override;
};
