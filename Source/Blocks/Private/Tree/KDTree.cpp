#include "Blocks.h"
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

void UKDTree::AddToTree(UKDTree* box)
{
	ensure(box != nullptr);

	addToTree(box);

	for (auto checkedRegion : watchedBoxes)
	{
		auto reg = checkedRegion.Value;

		if (box->GetBox().Intersect(reg->GetBox()))
			WatchingRegionChanged(checkedRegion.Key);
	}
}

void UKDTree::NotifyRegionChanged(UMinMaxBox* box)
{
	ensure(box != nullptr);

	for (auto checkedRegion : watchedBoxes)
	{
		auto reg = checkedRegion.Value;

		if (box->GetBox().Intersect(reg->GetBox()))
			WatchingRegionChanged(checkedRegion.Key);
	}
}

void UKDTree::addToTree(UKDTree* box, bool forceInsert)
{
	ensure(box != nullptr);

	check(GtMin(box->Min) && LtMax(box->Max));

	if (!B1 && !B2 && !SingleChild && GtMin(box->Min) && LtMax(box->Max))
	{
		SingleChild = box;
		box->SetParent(this);

		AddToTreeElements(SingleChild->ContainingObject, box);
		return;
	}

	if (SingleChild && !forceInsert)
	{
		SingleChild->SetParent(nullptr);

		RemoveFromTreeElements(SingleChild->ContainingObject, box);

		addToTree(SingleChild, true); // forcing to insert

		SingleChild = nullptr;
	}

	addToTreeByCoord(box);
}

void UKDTree::addToTreeByCoord(UKDTree* box) {
	if (sum(box->Max * DividingCoord) <= DividingCoordValue)		// whole object is in left plane
	{
		if (!B1 || !B1->IsValidLowLevel() || B1->IsPendingKill())
		{
			B1 = NewObject<UKDTree>(this);
			B1->SetParent(this);
			B1->Init(Min, (FVector(1, 1, 1) - DividingCoord) *  Max + (DividingCoord * DividingCoordValue), DividingIndex + 1);
		}
		B1->AddToTree(box);
		return;
	}

	if (sum(box->Min *DividingCoord) >= DividingCoordValue)		// whole object is in right plane
	{
		if (!B2 || !B2->IsValidLowLevel() || B2->IsPendingKill())
		{
			B2 = NewObject<UKDTree>(this);
			B2->SetParent(this);
			B2->Init((FVector(1, 1, 1) - DividingCoord) *  Min + (DividingCoord * DividingCoordValue), Max, DividingIndex + 1);
		}

		B2->AddToTree(box);
		return;
	}

	// object is in between. We need to split and then add object to both branches

	UKDTree* newB1 = NewObject<UKDTree>(this);
	newB1->InitBox(box->Min, (FVector(1, 1, 1) - DividingCoord) *  box->Max + (DividingCoord * DividingCoordValue));
	newB1->recomputeDividingCoordValue();
	newB1->ContainingObject = box->ContainingObject;
	UKDTree* newB2 = NewObject<UKDTree>(this);
	newB2->InitBox((FVector(1, 1, 1) - DividingCoord) *  box->Min + (DividingCoord * DividingCoordValue), box->Max);
	newB2->recomputeDividingCoordValue();
	newB2->ContainingObject = box->ContainingObject;

	addToTreeByCoord(newB1);
	addToTreeByCoord(newB2);
}

void UKDTree::DEBUGDrawContainingBox(UWorld* world)
{
	if (!world || IsPendingKill())
		return;

	auto center = (Max + Min) * 0.5 * (FVector(1, 1, 1) - DividingCoord) + DividingCoord * DividingCoordValue;
	auto extend = (Max - center) *  (FVector(1, 1, 1) - DividingCoord);

	auto di = DividingIndex % 3;

	if ((B1 && B1->IsValidLowLevel() && !B1->IsPendingKill()) || (B2 && B2->IsValidLowLevel() && !B2->IsPendingKill()))
		DrawDebugBox(world, center, extend, di == 0 ? FColor::Red : (di == 1 ? FColor::Green : FColor::Blue), true);

	if (ContainingObject && !ContainingObject->IsPendingKill())
	{
		auto bcenter = (Max + Min) * 0.5;
		auto bextend = (Max - bcenter);
		DrawDebugBox(world, bcenter, bextend, FColor::White, true);
	}

	if (SingleChild && !SingleChild->IsPendingKill())
	{
		SingleChild->DEBUGDrawContainingBox(world);
		return;
	}
	if (B1 && B1->IsValidLowLevel() && !B1->IsPendingKill())
		B1->DEBUGDrawContainingBox(world);

	if (B2 && B2->IsValidLowLevel() && !B2->IsPendingKill())
		B2->DEBUGDrawContainingBox(world);

	if (!GetParent())
	{
		for (auto i : watchedBoxes)
		{
			auto box = i.Value;
			auto bcenter = (box->Max + box->Min) * 0.5;
			auto bextend = (box->Max - bcenter);
			DrawDebugBox(world, bcenter, bextend, FColor::Orange, true);
		}
	}
}

void UKDTree::DEBUGDrawSurrondings(UWorld* world, FColor usedColor)
{
	if (!world || IsPendingKill())
		return;

	auto bcenter = (Max + Min) * 0.5;
	auto bextend = (Max - bcenter);
	DrawDebugBox(world, bcenter, bextend, usedColor, true);
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

	UMinMaxBox* newB1 = NewObject<UMinMaxBox>()->InitBox(box->Min, (FVector(1, 1, 1) - DividingCoord) *  box->Max + (DividingCoord * DividingCoordValue));
	UMinMaxBox* newB2 = NewObject<UMinMaxBox>()->InitBox((FVector(1, 1, 1) - DividingCoord) *  box->Min + (DividingCoord * DividingCoordValue), box->Max);

	return IsPlaceEmpty(newB1) && IsPlaceEmpty(newB2);
}

bool UKDTree::isPlaceEmptySingleChild(const UMinMaxBox* box)
{
	auto x = box->Max.X <= Min.X || box->Min.X >= Max.X;		// false if there is netrivial intersection
	auto y = box->Max.Y <= Min.Y || box->Min.Y >= Max.Y;		// false if there is netrivial intersection
	auto z = box->Max.Z <= Min.Z || box->Min.Z >= Max.Z;		// false if there is netrivial intersection

	return x || y || z;
}

void UKDTree::GetContainingObjects(const UMinMaxBox* box, TArray<UObject*>& outArray, const UObject* ignoreElement, const bool checkForCommonBoundaries)
{
	if (!(GtMin(box->Min) && LtMax(box->Max)))
		return;

	if (SingleChild)
	{
		if (ignoreElement && ignoreElement->IsValidLowLevel() && SingleChild->ContainingObject == ignoreElement)
			return;

		if (outArray.Contains(SingleChild->ContainingObject))
			return;

		auto commonBound = checkForCommonBoundaries
			? CheckCommonBoundaries(SingleChild->ContainingObject, ignoreElement)
			: SingleChild->GetBox().Intersect(box->GetBox());
		if (commonBound)
			outArray.Add(SingleChild->ContainingObject);

		return;
	}

	if (sum(box->Max * DividingCoord) <= DividingCoordValue)		// whole object is in left plane
	{
		if (!B1)
			return;
		B1->GetContainingObjects(box, outArray, ignoreElement, checkForCommonBoundaries);
		return;
	}

	if (sum(box->Min *DividingCoord) >= DividingCoordValue)		// whole object is in right plane
	{
		if (!B2)
			return;
		B2->GetContainingObjects(box, outArray, ignoreElement, checkForCommonBoundaries);
		return;
	}

	// object is in between. We need to split and then add object to both branches

	UMinMaxBox* newB1 = NewObject<UMinMaxBox>()->InitBox(box->Min, (FVector(1, 1, 1) - DividingCoord) *  box->Max + (DividingCoord * DividingCoordValue));
	UMinMaxBox* newB2 = NewObject<UMinMaxBox>()->InitBox((FVector(1, 1, 1) - DividingCoord) *  box->Min + (DividingCoord * DividingCoordValue), box->Max);

	GetContainingObjects(newB1, outArray, ignoreElement, checkForCommonBoundaries);
	GetContainingObjects(newB2, outArray, ignoreElement, checkForCommonBoundaries);
}

void UKDTree::GetContainingObjectsFromBottom(const UMinMaxBox* box, TArray<UObject*>& outArray, const UObject* ignoreElement)
{
	if (!(GtMin(box->Min) && LtMax(box->Max)))
	{
		auto parent = GetParent();
		ensure(parent != nullptr);
		parent->GetContainingObjectsFromBottom(box, outArray, ignoreElement);
		return;
	}

	GetContainingObjects(box, outArray, ignoreElement, true);
}

void UKDTree::UpdateAfterChildDestroyed()
{
	check(SingleChild && !B1 && !B2);

	auto parent = GetParent();

	SingleChild = NULL;

	if (parent)
		MarkPendingKill();

	if (parent && parent->IsValidLowLevel() && parent->canBeDeleted())
		parent->updateAfterChildDestroyedInner();
}

void UKDTree::updateAfterChildDestroyedInner()
{
	if (!canBeDeleted())
		return;

	check(SingleChild || B1 || B2);

	auto hasSingle = checkElem(SingleChild);
	auto hasB1 = checkElem(B1);
	auto hasB2 = checkElem(B2);

	if (hasSingle || hasB1 || hasB2)
		return;

	auto parent = GetParent();
	MarkPendingKill();
	if (parent && parent->IsValidLowLevel() && parent->canBeDeleted())
		parent->updateAfterChildDestroyedInner();
}

void UKDTree::RegisterWatchingBox(UObject* actor, UMinMaxBox* box)
{
	TryUnregisterWatchingBox(actor);

	watchedBoxes.Add(actor, box);

	auto bcenter = (box->Max + box->Min) * 0.5;
	auto bextend = (box->Max - bcenter);
	DrawDebugBox(GetWorld(), bcenter, bextend, FColor::Red, false, 60, 0, 5);
}

void UKDTree::TryUnregisterWatchingBox(UObject* actor)
{
	auto existing = watchedBoxes.Find(actor);
	if (!existing)
		return;

	watchedBoxes.Remove(actor);
}