#include "Blocks.h"
#include "MinMaxBox.h"

UMinMaxBox* UMinMaxBox::InitBox(FVector min, FVector max)
{
	Min = min;
	Max = max;

	ensure(Min.X <= Max.X);
	ensure(Min.Y <= Max.Y);
	ensure(Min.Z <= Max.Z);

	MinWorldCoord = ((Min + FVector(1, 1, 1) * GameDefinitions::CubeMinSizeHalf) / GameDefinitions::CubeMinSize).GridSnap(1);
	MaxWorldCoord = ((Max - FVector(1, 1, 1) * GameDefinitions::CubeMinSizeHalf) / GameDefinitions::CubeMinSize).GridSnap(1);

	return this;
}

UMinMaxBox* UMinMaxBox::InitBoxChecked(FVector min, FVector max)
{
	auto tmin = FVector(FMath::Min(min.X, max.X), FMath::Min(min.Y, max.Y), FMath::Min(min.Z, max.Z));
	auto tmax = FVector(FMath::Max(min.X, max.X), FMath::Max(min.Y, max.Y), FMath::Max(min.Z, max.Z));
	return InitBox(tmin, tmax);
}

void UMinMaxBox::RemoveElement()
{
}

#pragma optimize("", off)
bool UMinMaxBox::HasCommonBoundaries(const UMinMaxBox* box1, const UMinMaxBox* box2)
{
	ensure(box1 != nullptr && box1->IsValidLowLevel());
	ensure(box2 != nullptr && box2->IsValidLowLevel());

	auto x = box2->Min.X == box1->Max.X || box2->Max.X == box1->Min.X;			// false if there is not connection
	auto y = box2->Min.Y == box1->Max.Y || box2->Max.Y == box1->Min.Y;			// false if there is not connection
	auto z = box2->Min.Z == box1->Max.Z || box2->Max.Z == box1->Min.Z;			// false if there is not connection

	auto sum = (x ? 1 : 0) + (y ? 1 : 0) + (z ? 1 : 0);
	if (sum != 1)
		return false;   // wee need to have touch exactly in one dimension

	auto inRangeX = box2->Min.X < box1->Max.X && box2->Max.X > box1->Min.X;
	auto inRangeY = box2->Min.Y < box1->Max.Y && box2->Max.Y > box1->Min.Y;
	auto inRangeZ = box2->Min.Z < box1->Max.Z && box2->Max.Z > box1->Min.Z;

	auto sX = x && inRangeY && inRangeZ;
	auto sY = y && inRangeX && inRangeZ;
	auto sZ = z && inRangeX && inRangeY;

	return sX || sY || sZ;
}
#pragma optimize("", on)