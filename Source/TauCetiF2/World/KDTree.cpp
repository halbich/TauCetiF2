

#include "TauCetiF2.h"
#include "KDTree.h"




UKDTree* UKDTree::Init(FVector min, FVector max, int8 dividingIndex)
{
	InitBox(min, max);
	DividingIndex = dividingIndex;
	DividingCoord = FVector(dividingIndex % 3 == 0 ? 1 : 0, dividingIndex % 3 == 1 ? 1 : 0, dividingIndex % 3 == 2 ? 1 : 0);

	recomputeDividingCoordValue();
	return this;
}

UKDTree* UKDTree::Init(FTransform& transform)
{
	InitBox(transform);
	recomputeDividingCoordValue();
	return this;
}


void UKDTree::AddToTree(UKDTree* box, bool forceInsert)
{
	ensure(box != nullptr);

	check(GtMin(box->Min) && LtMax(box->Max));

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
}




void UKDTree::addToTreeByCoord(UKDTree* box) {


	if (sum(box->Max * DividingCoord) <= DividingCoordValue)		// whole object is in left plane
	{
		if (!B1)
		{
			B1 = NewObject<UKDTree>(this);
			B1->Init(Min, (FVector(1, 1, 1) - DividingCoord) *  Max + (DividingCoord * DividingCoordValue), DividingIndex + 1);
		}
		B1->AddToTree(box);
		return;
	}

	if (sum(box->Min *DividingCoord) >= DividingCoordValue)		// whole object is in right plane
	{
		if (!B2)
		{
			B2 = NewObject<UKDTree>(this);
			B2->Init((FVector(1, 1, 1) - DividingCoord) *  Min + (DividingCoord * DividingCoordValue), Max, DividingIndex + 1);
		}

		B2->AddToTree(box);
		return;
	}

	// object is in between. We need to split and then add object to both branches

	UKDTree* newB1 = NewObject<UKDTree>(this);
	newB1->Init(box->Min, box->Max, box->DividingIndex);
	newB1->containingObject = box->containingObject;

	box->Max = (FVector(1, 1, 1) - DividingCoord) *  box->Max + (DividingCoord * DividingCoordValue);
	box->recomputeDividingCoordValue();
	newB1->Min = (FVector(1, 1, 1) - DividingCoord) *  box->Min + (DividingCoord * DividingCoordValue);
	newB1->recomputeDividingCoordValue();
	addToTreeByCoord(box);
	addToTreeByCoord(newB1);


}


void UKDTree::DEBUGDrawContainingBox(UWorld* world)
{
	if (!world)
		return;


	auto center = (Max + Min) * 0.5 * (FVector(1, 1, 1) - DividingCoord) + DividingCoord * DividingCoordValue;
	auto extend = (Max - center) *  (FVector(1, 1, 1) - DividingCoord);



	auto di = DividingIndex % 3;

	if (B1 || B2)
		DrawDebugBox(world, center, extend, di == 0 ? FColor::Red : (di == 1 ? FColor::Green : FColor::Blue), true);


	if (containingObject)
		DrawDebugBox(world, center, Max - center, FColor::White, true);

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


bool UKDTree::IsPlaceEmpty(const UMinMaxBox* box) {

	if (!(GtMin(box->Min) && LtMax(box->Max)))
		return false;

	if (SingleChild)
		return SingleChild->isPlaceEmptySingleChild(box);

	if (sum(box->Max * DividingCoord) <= DividingCoordValue)		// whole object is in left plane
	{
		return	!B1 ? true : B1->IsPlaceEmpty(box);
	}

	if (sum(box->Min *DividingCoord) >= DividingCoordValue)		// whole object is in right plane
	{
		return	!B2 ? true : B2->IsPlaceEmpty(box);
	}

	// object is in between. We need to split and then add object to both branches

	UMinMaxBox* newB1 = NewObject<UMinMaxBox>(this)->InitBox(box->Min, (FVector(1, 1, 1) - DividingCoord) *  box->Max + (DividingCoord * DividingCoordValue));
	UMinMaxBox* newB2 = NewObject<UMinMaxBox>(this)->InitBox((FVector(1, 1, 1) - DividingCoord) *  box->Min + (DividingCoord * DividingCoordValue), box->Max);

	return IsPlaceEmpty(newB1) && IsPlaceEmpty(newB2);
}


bool UKDTree::isPlaceEmptySingleChild(const UMinMaxBox* box)
{
	auto x = box->Max.X <= Min.X || box->Min.X >= Max.X;		// false if there is netrivial intersection
	auto y = box->Max.Y <= Min.Y || box->Min.Y >= Max.Y;		// false if there is netrivial intersection
	auto z = box->Max.Z <= Min.Z || box->Min.Z >= Max.Z;		// false if there is netrivial intersection

	return x || y || z;
}