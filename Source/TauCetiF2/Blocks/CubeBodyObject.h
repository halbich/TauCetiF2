

#pragma once

#include "Common/WorldObject.h"
#include "CubeBodyObject.generated.h"

/**
 *
 */
UCLASS()
class TAUCETIF2_API ACubeBodyObject : public AWorldObject
{
	GENERATED_UCLASS_BODY()

public:
	virtual void OnConstruction(const FTransform& Transform) override;


};
