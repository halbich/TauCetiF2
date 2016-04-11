

#pragma once

#include "Common/WorldObject.h"

/**
 *
 */
class TAUCETIF2_API FMinMaxBox
{

public:
	FMinMaxBox(FVector min, FVector max);
	~FMinMaxBox();

	FVector Min;
	FVector Max;

	FMinMaxBox* B1;

	FMinMaxBox* B2;

	AWorldObject* containingObject;

	void DEBUGDrawContainingBox(UWorld* world);

	void AddToTree(FMinMaxBox* box);

	static FMinMaxBox* FromWorldObject(AWorldObject* object) {

		FVector min;
		FVector max;
		object->GetBoundingBox(min, max);
		auto ret = new FMinMaxBox(min, max);
		ret->containingObject = object;
		return ret;
	}

};
