

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

	/*addToTreeByCoord(box);
	return;*/


	switch (DividingIndex % 3) {
	case 0:
		addToTreeByX(box, DividingCoordValue);
		break;
	case 1:
		addToTreeByY(box, DividingCoordValue);

		break;
	case 2:
		addToTreeByZ(box, DividingCoordValue);

		break;
	}
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

	box->Max = (FVector(1, 1, 1) - DividingCoord) *  Max + (DividingCoord * DividingCoordValue);
	box->RecomputeDividingCoordValue();
	newB1->Min = (FVector(1, 1, 1) - DividingCoord) *  Min + (DividingCoord * DividingCoordValue);
	newB1->RecomputeDividingCoordValue();
	addToTreeByCoord(box);
	addToTreeByCoord(newB1);


}


void FMinMaxBox::addToTreeByX(FMinMaxBox* box, float& dividingCoord) {

	if (box->Max.X <= dividingCoord)		// whole object is in left plane
	{
		if (!B1)
		{
			B1 = new FMinMaxBox(Min, FVector(dividingCoord, Max.Y, Max.Z), DividingIndex + 1);
			B1->name = FString::Printf(TEXT("%s > B1"), *name);
		}
		B1->AddToTree(box);
		return;
	}

	if (box->Min.X >= dividingCoord)		// whole object is in right plane
	{
		if (!B2)
		{
			B2 = new FMinMaxBox(FVector(dividingCoord, Min.Y, Min.Z), Max, DividingIndex + 1);
			B2->name = FString::Printf(TEXT("%s > B2"), *name);
		}

		B2->AddToTree(box);
		return;
	}

	// object is in between. We need to split and then add object to both branches

	FMinMaxBox* newB1 = new FMinMaxBox(box->Min, box->Max, box->DividingIndex);
	newB1->containingObject = box->containingObject;

	box->Max.X = dividingCoord;
	newB1->Min.X = dividingCoord;
	addToTreeByX(box, dividingCoord);
	addToTreeByX(newB1, dividingCoord);



}


void FMinMaxBox::addToTreeByY(FMinMaxBox* box, float dividingCoord) {

	if (box->Max.Y <= dividingCoord)		// whole object is in left plane
	{
		if (!B1)
		{
			B1 = new FMinMaxBox(Min, FVector(Max.X, dividingCoord, Max.Z), DividingIndex + 1);
			B1->name = FString::Printf(TEXT("%s > B1"), *name);
		}
		B1->AddToTree(box);
		return;
	}

	if (box->Min.Y >= dividingCoord)		// whole object is in right plane
	{
		if (!B2)
		{
			B2 = new FMinMaxBox(FVector(Min.X, dividingCoord, Min.Z), Max, DividingIndex + 1);
			B2->name = FString::Printf(TEXT("%s > B2"), *name);
		}
		B2->AddToTree(box);
		return;
	}

	// object is in between. We need to split and then add object to both branches


	FMinMaxBox* newB1 = new FMinMaxBox(box->Min, box->Max, box->DividingIndex);
	newB1->containingObject = box->containingObject;

	box->Max.Y = dividingCoord;
	newB1->Min.Y = dividingCoord;
	addToTreeByY(box, dividingCoord);
	addToTreeByY(newB1, dividingCoord);
}

void FMinMaxBox::addToTreeByZ(FMinMaxBox* box, float dividingCoord) {

	if (box->Max.Z <= dividingCoord)		// whole object is in left plane
	{
		if (!B1)
		{
			B1 = new FMinMaxBox(Min, FVector(Max.X, Max.Y, dividingCoord), DividingIndex + 1);
			B1->name = FString::Printf(TEXT("%s > B1"), *name);
		}
		B1->AddToTree(box);
		return;
	}

	if (box->Min.Z >= dividingCoord)		// whole object is in right plane
	{
		if (!B2)
		{
			B2 = new FMinMaxBox(FVector(Min.X, Min.Y, dividingCoord), Max, DividingIndex + 1);
			B2->name = FString::Printf(TEXT("%s > B1"), *name);
		}
		B2->AddToTree(box);
		return;
	}

	// object is in between. We need to split and then add object to both branches


	FMinMaxBox* newB1 = new FMinMaxBox(box->Min, box->Max, box->DividingIndex);
	newB1->containingObject = box->containingObject;

	box->Max.Z = dividingCoord;
	newB1->Min.Z = dividingCoord;
	addToTreeByZ(box, dividingCoord);
	addToTreeByZ(newB1, dividingCoord);
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
