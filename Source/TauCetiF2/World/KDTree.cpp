

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

UKDTree* UKDTree::Init(UMinMaxBox* box)
{
	ensure(box != nullptr);
	return Init(box->Min, box->Max, 0);
}

UKDTree* UKDTree::Init(UMinMaxBox* box, UMinMaxBox* constrainingBox)
{
	ensure(box != nullptr);
	ensure(constrainingBox != nullptr);
	FVector min(FMath::Max(box->Min.X - GameDefinitions::CubeMinSize, constrainingBox->Min.X), FMath::Max(box->Min.Y - GameDefinitions::CubeMinSize, constrainingBox->Min.Y), FMath::Max(box->Min.Z - GameDefinitions::CubeMinSize, constrainingBox->Min.Z));
	FVector max(FMath::Min(box->Max.X + GameDefinitions::CubeMinSize, constrainingBox->Max.X), FMath::Min(box->Max.Y + GameDefinitions::CubeMinSize, constrainingBox->Max.Y), FMath::Min(box->Max.Z + GameDefinitions::CubeMinSize, constrainingBox->Max.Z));
	return Init(min, max, 0);
}


void UKDTree::AddToTree(UKDTree* box, TArray<UKDTree*>& usedBoxes, bool forceInsert)
{
	ensure(box != nullptr);

	check(GtMin(box->Min) && LtMax(box->Max));

	if (!B1 && !B2 && !SingleChild && GtMin(box->Min) && LtMax(box->Max))
	{
		SingleChild = box;
		box->ParentNode = this;
		usedBoxes.Add(box);
		return;
	}


	if (SingleChild && !forceInsert)
	{
		SingleChild->ParentNode = nullptr;
		usedBoxes.Remove(SingleChild);			// this box could be split
		AddToTree(SingleChild,usedBoxes, true); // forcing to insert

		SingleChild = nullptr;
	}

	addToTreeByCoord(box, usedBoxes);
}




void UKDTree::addToTreeByCoord(UKDTree* box, TArray<UKDTree*>& usedBoxes) {


	if (sum(box->Max * DividingCoord) <= DividingCoordValue)		// whole object is in left plane
	{
		if (!B1)
		{
			B1 = NewObject<UKDTree>(this);
			B1->ParentNode = this;
			B1->Init(Min, (FVector(1, 1, 1) - DividingCoord) *  Max + (DividingCoord * DividingCoordValue), DividingIndex + 1);
		}
		B1->AddToTree(box, usedBoxes);
		return;
	}

	if (sum(box->Min *DividingCoord) >= DividingCoordValue)		// whole object is in right plane
	{
		if (!B2)
		{
			B2 = NewObject<UKDTree>(this);
			B2->ParentNode = this;
			B2->Init((FVector(1, 1, 1) - DividingCoord) *  Min + (DividingCoord * DividingCoordValue), Max, DividingIndex + 1);
		}

		B2->AddToTree(box, usedBoxes);
		return;
	}

	// object is in between. We need to split and then add object to both branches


	UKDTree* newB1 = NewObject<UKDTree>(this);
	newB1->InitBox(box->Min, (FVector(1, 1, 1) - DividingCoord) *  box->Max + (DividingCoord * DividingCoordValue));
	newB1->recomputeDividingCoordValue();
	newB1->containingObject = box->containingObject;
	UKDTree* newB2 = NewObject<UKDTree>(this);
	newB2->InitBox((FVector(1, 1, 1) - DividingCoord) *  box->Min + (DividingCoord * DividingCoordValue), box->Max);
	newB2->recomputeDividingCoordValue();
	newB2->containingObject = box->containingObject;

	addToTreeByCoord(newB1, usedBoxes);
	addToTreeByCoord(newB2, usedBoxes);

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
	{
		auto bcenter = (Max + Min) * 0.5;
		auto bextend = (Max - bcenter);
		DrawDebugBox(world, bcenter, bextend, FColor::White, true);
	}

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

void UKDTree::DEBUGDrawSurrondings(UWorld* world)
{
	if (!world)
		return;


	auto bcenter = (Max + Min) * 0.5;
	auto bextend = (Max - bcenter);
	DrawDebugBox(world, bcenter, bextend, FColor::Magenta, true);

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


void UKDTree::GetContainingObjects(const UMinMaxBox* box, TArray<AWorldObject*>& outArray)
{
	if (!(GtMin(box->Min) && LtMax(box->Max)))
		return;

	if (SingleChild)
	{
		outArray.Add(SingleChild->containingObject);
		return;
	}

	if (sum(box->Max * DividingCoord) <= DividingCoordValue)		// whole object is in left plane
	{
		if (!B1)
			return;
		B1->GetContainingObjects(box, outArray);
		return;
	}

	if (sum(box->Min *DividingCoord) >= DividingCoordValue)		// whole object is in right plane
	{
		if (!B2)
			return;
		B2->GetContainingObjects(box, outArray);
		return;
	}

	// object is in between. We need to split and then add object to both branches

	UMinMaxBox* newB1 = NewObject<UMinMaxBox>(this)->InitBox(box->Min, (FVector(1, 1, 1) - DividingCoord) *  box->Max + (DividingCoord * DividingCoordValue));
	UMinMaxBox* newB2 = NewObject<UMinMaxBox>(this)->InitBox((FVector(1, 1, 1) - DividingCoord) *  box->Min + (DividingCoord * DividingCoordValue), box->Max);


	GetContainingObjects(newB1, outArray);
	GetContainingObjects(newB2, outArray);


}

void UKDTree::GetContainingObjectsFromBottom(const UMinMaxBox* box, TArray<AWorldObject*>& outArray)
{
	if (!(GtMin(box->Min) && LtMax(box->Max)))
	{
		ensure(ParentNode != nullptr);
		//ParentNode->GetContainingObjectsFromBottom(box, outArray);
		return;
	}

	GetContainingObjects(box, outArray);


}