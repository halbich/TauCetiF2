

#include "TauCetiF2.h"
#include "MinMaxBox.h"

FMinMaxBox::FMinMaxBox(FVector min, FVector max) : B1(nullptr), B2(nullptr), Min(min), Max(max)
{
	B1 = nullptr;
	B2 = nullptr;
}

FMinMaxBox::~FMinMaxBox()
{
	if (B1)
	{
		delete B1;
		B1 = nullptr;
	}

	if (B2)
	{
		delete B2;
		B2 = nullptr;
	}
}


void FMinMaxBox::DEBUGDrawContainingBox(UWorld* world)
{
	if (!world)
		return;

	auto center = (Max + Min) * 0.5;
	auto extend = Max - center;

	DrawDebugBox(world, center, extend, FColor::Red, true);

	if (B1)
		B1->DEBUGDrawContainingBox(world);

	if (B2)
		B2->DEBUGDrawContainingBox(world);
}

