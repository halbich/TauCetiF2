

#pragma once

#include "Blocks/WorldObject.h"
#include "TerminalObject.generated.h"

/**
 * 
 */
UCLASS()
class TAUCETIF2_API ATerminalObject : public AWorldObject
{
	GENERATED_UCLASS_BODY()

public:
	virtual void OnConstruction(const FTransform& Transform) override;

};
