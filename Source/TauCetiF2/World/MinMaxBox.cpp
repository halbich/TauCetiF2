

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

UMinMaxBox* UMinMaxBox::InitBoxChecked(FVector min, FVector max)
{
	auto tmin = FVector(FMath::Min(min.X, max.X), FMath::Min(min.Y, max.Y), FMath::Min(min.Z, max.Z));
	auto tmax = FVector(FMath::Max(min.X, max.X), FMath::Max(min.Y, max.Y), FMath::Max(min.Z, max.Z));
	return InitBox(tmin, tmax);
}

