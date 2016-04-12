
#pragma once

#include "Common/Enums.h"
#include "Common/WorldObject.h"
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

	FORCEINLINE void prepareMaterials(EMaterialInstance& matTop, EMaterialInstance& matSide1, EMaterialInstance& matSide2) {

		if (!WorldObjectComponent || !WorldObjectComponent->BlockInfo)
			return;

		auto bi = WorldObjectComponent->BlockInfo;

		switch (bi->MaterialType)
		{
		case EMaterialType::Polycarbon:
			matTop = matSide1 = matSide2 = EMaterialInstance::Polycarbon;
			break;
		case EMaterialType::Steel:
		case EMaterialType::Alloy:
		case EMaterialType::SuperAlloy:
			if (bi->BlockType == EBlockType::Base)
			{
				matTop = EMaterialInstance::BaseFloor;
				matSide1 = matSide2 = EMaterialInstance::BaseSide;
			}
			else
			{
				matTop = matSide1 = matSide2 = EMaterialInstance::ConstructRectangle;
			}
			break;
		default:
			break;
		}
	}



};
