

#pragma once


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
};
