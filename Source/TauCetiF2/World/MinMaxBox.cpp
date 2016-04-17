

#include "TauCetiF2.h"
#include "MinMaxBox.h"




UMinMaxBox* UMinMaxBox::InitBox(FVector min, FVector max)
{
	Min = min;
	Max = max;
	return this;
}

UMinMaxBox* UMinMaxBox::InitBox(const FTransform& transform)
{
	AWorldObject::GetBoundingBox(transform, Min, Max);
	return this;
}