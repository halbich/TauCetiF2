

#pragma once

#include "Common/WorldObject.h"
#include "CubeSideObject.generated.h"

/**
 *
 */
UCLASS()
class TAUCETIF2_API ACubeSideObject : public AWorldObject
{
	GENERATED_UCLASS_BODY()

public:
	virtual void OnConstruction(const FTransform& Transform) override;



};
