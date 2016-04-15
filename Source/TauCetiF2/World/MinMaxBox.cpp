

#include "TauCetiF2.h"
#include "MinMaxBox.h"

FMinMaxBox::FMinMaxBox(FVector min, FVector max, int8 dividingIndex) :
	B1(nullptr),
	B2(nullptr),
	Min(min),
	Max(max),
	containingObject(nullptr),
	DividingIndex(dividingIndex),
	SingleChild(nullptr),
	DividingCoord(dividingIndex % 3 == 0 ? 1 : 0, dividingIndex % 3 == 1 ? 1 : 0, dividingIndex % 3 == 2 ? 1 : 0)
{
	name = TEXT("");


	RecomputeDividingCoordValue();

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

void FMinMaxBox::AddToTree(FMinMaxBox* box, bool forceInsert) {

	ensure(box != nullptr);

	if (!B1 && !B2 && !SingleChild && GtMin(box->Min) && LtMax(box->Max))
	{
		SingleChild = box;
		return;
	}


	if (SingleChild && !forceInsert)
	{
		AddToTree(SingleChild, true); // forcing to insert
		SingleChild = nullptr;
	}

	addToTreeByCoord(box);
	return;
}

void FMinMaxBox::addToTreeByCoord(FMinMaxBox* box) {


	if (sum(box->Max * DividingCoord) <= DividingCoordValue)		// whole object is in left plane
	{
		if (!B1)
		{
			B1 = new FMinMaxBox(Min, (FVector(1, 1, 1) - DividingCoord) *  Max + (DividingCoord * DividingCoordValue), DividingIndex + 1);
			B1->name = FString::Printf(TEXT("%s > B1"), *name);
		}
		B1->AddToTree(box);
		return;
	}

	if (sum(box->Min *DividingCoord) >= DividingCoordValue)		// whole object is in right plane
	{
		if (!B2)
		{
			B2 = new FMinMaxBox((FVector(1, 1, 1) - DividingCoord) *  Min + (DividingCoord * DividingCoordValue), Max, DividingIndex + 1);
			B2->name = FString::Printf(TEXT("%s > B2"), *name);
		}

		B2->AddToTree(box);
		return;
	}

	// object is in between. We need to split and then add object to both branches

	FMinMaxBox* newB1 = new FMinMaxBox(box->Min, box->Max, box->DividingIndex);
	newB1->containingObject = box->containingObject;

	box->Max = (FVector(1, 1, 1) - DividingCoord) *  box->Max + (DividingCoord * DividingCoordValue);
	box->RecomputeDividingCoordValue();
	newB1->Min = (FVector(1, 1, 1) - DividingCoord) *  box->Min + (DividingCoord * DividingCoordValue);
	newB1->RecomputeDividingCoordValue();
	addToTreeByCoord(box);
	addToTreeByCoord(newB1);


}


void FMinMaxBox::DEBUGDrawContainingBox(UWorld* world)
{
	if (!world)
		return;


	auto center = (Max + Min) * 0.5;
	auto extend = Max - center;



	auto di = DividingIndex % 3;

	DrawDebugBox(world, center, extend, di == 0 ? FColor::Red : (di == 1 ? FColor::Green : FColor::Blue), true);

	if (SingleChild)
	{
		SingleChild->DEBUGDrawContainingBox(world);
		return;
	}
	if (B1)
		B1->DEBUGDrawContainingBox(world);

	if (B2)
		B2->DEBUGDrawContainingBox(world);


}


bool FMinMaxBox::IsPlaceEmpty(const FMinMaxBox& box) {

	return false;
}
