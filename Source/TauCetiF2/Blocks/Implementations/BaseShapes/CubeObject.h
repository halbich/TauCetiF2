
#pragma once

#include "Common/Enums.h"
#include "Blocks/WorldObject.h"
#include "CubeObject.generated.h"

/**
 *
 */
UCLASS()
class TAUCETIF2_API ACubeObject : public AWorldObject
{
	GENERATED_UCLASS_BODY()

public:
	virtual void OnConstruction(const FTransform& Transform) override;



};
