

#include "TauCetiF2.h"
#include "MinMaxBox.h"




UMinMaxBox* UMinMaxBox::InitBox(FVector min, FVector max)
{
	Min = min;
	Max = max;

	ensure(Min.X <= Max.X);
	ensure(Min.Y <= Max.Y);
	ensure(Min.Z <= Max.Z);
	return this;
}

UMinMaxBox* UMinMaxBox::InitBox(const FTransform& transform)
{
	AWorldObject::GetBoundingBox(transform, Min, Max);

	ensure(Min.X <= Max.X);
	ensure(Min.Y <= Max.Y);
	ensure(Min.Z <= Max.Z);
	return this;
}