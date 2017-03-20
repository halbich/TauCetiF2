﻿#include "Blocks.h"
#include "WeatherTargetsKDTree.h"

#pragma optimize("", off)

UWeatherTargetsKDTree* UWeatherTargetsKDTree::Init(FVector min, FVector max, int8 dividingIndex)
{
	InitBox(min, max);
	DividingIndex = dividingIndex;
	DividingCoord = FVector(dividingIndex % 2 == 0 ? 1 : 0, dividingIndex % 2 == 1 ? 1 : 0, 0);

	recomputeDividingCoordValue();
	return this;
}

UWeatherTargetsKDTree* UWeatherTargetsKDTree::Init(UMinMaxBox* box)
{
	ensure(box != nullptr);
	return Init(box->Min, box->Max, 0);
}

UWeatherTargetsKDTree* UWeatherTargetsKDTree::Init(UMinMaxBox* box, UMinMaxBox* constrainingBox)
{
	ensure(box != nullptr);
	ensure(constrainingBox != nullptr);
	FVector min(FMath::Max(box->Min.X - GameDefinitions::CubeMinSize, constrainingBox->Min.X), FMath::Max(box->Min.Y - GameDefinitions::CubeMinSize, constrainingBox->Min.Y), FMath::Max(box->Min.Z - GameDefinitions::CubeMinSize, constrainingBox->Min.Z));
	FVector max(FMath::Min(box->Max.X + GameDefinitions::CubeMinSize, constrainingBox->Max.X), FMath::Min(box->Max.Y + GameDefinitions::CubeMinSize, constrainingBox->Max.Y), FMath::Min(box->Max.Z + GameDefinitions::CubeMinSize, constrainingBox->Max.Z));
	return Init(min, max, 0);
}

void UWeatherTargetsKDTree::AddToTree(UWeatherTargetsKDTree* box)
{
	ensure(box != nullptr);

	check(GtMin2(box->Min) && LtMax2(box->Max)); // todo opravit na 2

	bool forceSplit = false;
	if (((Max - Min) * FVector(1, 1, 0)) == FVector(1, 1, 0) * GameDefinitions::CubeMinSize)
	{
		ChildHeap.HeapPush(box, WeatherTargetsPriorityPredicate());
		return;
	}

	addToTreeByCoord(box);
}

void UWeatherTargetsKDTree::addToTreeByCoord(UWeatherTargetsKDTree* box) {
	if (sum(box->Max * DividingCoord) <= DividingCoordValue)		// whole object is in left plane
	{
		if (!B1 || !B1->IsValidLowLevel() || B1->IsPendingKill())
		{
			B1 = NewObject<UWeatherTargetsKDTree>();
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
			B2 = NewObject<UWeatherTargetsKDTree>();
			B2->SetParent(this);
			B2->Init((FVector(1, 1, 1) - DividingCoord) *  Min + (DividingCoord * DividingCoordValue), Max, DividingIndex + 1);
		}

		B2->AddToTree(box);
		return;
	}

	// object is in between. We need to split and then add object to both branches

	UWeatherTargetsKDTree* newB1 = NewObject<UWeatherTargetsKDTree>();
	newB1->InitBox(box->Min, (FVector(1, 1, 1) - DividingCoord) *  box->Max + (DividingCoord * DividingCoordValue));
	newB1->recomputeDividingCoordValue();
	UWeatherTargetsKDTree* newB2 = NewObject<UWeatherTargetsKDTree>();
	newB2->InitBox((FVector(1, 1, 1) - DividingCoord) *  box->Min + (DividingCoord * DividingCoordValue), box->Max);
	newB2->recomputeDividingCoordValue();

	addToTreeByCoord(newB1);
	addToTreeByCoord(newB2);
}

void UWeatherTargetsKDTree::DEBUGDrawContainingBox(UWorld* world)
{
	if (!world || IsPendingKill())
		return;

	auto tMax = Max; tMax.Z = 5;

	auto center = (tMax + Min) * 0.5 * (FVector(1, 1, 1) - DividingCoord) + DividingCoord * DividingCoordValue;
	auto extend = (tMax - center) *  (FVector(1, 1, 1) - DividingCoord);

	auto di = DividingIndex % 2;

	/*if ((B1 && B1->IsValidLowLevel() && !B1->IsPendingKill()) || (B2 && B2->IsValidLowLevel() && !B2->IsPendingKill()))
		DrawDebugBox(world, center, extend, di == 0 ? FColor::Red : FColor::Green, true);*/

	if (ChildHeap.Num() > 0)
	{
		auto stMin = ChildHeap[0]->Min;
		auto stMax = ChildHeap.Top()->Max;
		stMax.Z += 10;

		auto stCentre = (stMin + stMax) * 0.5f;
		auto stExtend = (stMax - stCentre);

		DrawDebugBox(world, stCentre, stExtend, FColor::Yellow, true);
		return;
	}
	if (B1 && B1->IsValidLowLevel() && !B1->IsPendingKill())
		B1->DEBUGDrawContainingBox(world);

	if (B2 && B2->IsValidLowLevel() && !B2->IsPendingKill())
		B2->DEBUGDrawContainingBox(world);
}

void UWeatherTargetsKDTree::DEBUGDrawSurrondings(UWorld* world, FColor usedColor)
{
	if (!world || IsPendingKill())
		return;

	auto bcenter = (Max + Min) * 0.5;
	auto bextend = (Max - bcenter);
	DrawDebugBox(world, bcenter, bextend, usedColor, true);
}

bool UWeatherTargetsKDTree::IsPlaceEmpty(const UMinMaxBox* box) {
	if (!(GtMin(box->Min) && LtMax(box->Max)))
		return false;

	if (ChildHeap.Num() == 1)
		return ChildHeap.Top()->isPlaceEmptySingleChild(box);

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

bool UWeatherTargetsKDTree::isPlaceEmptySingleChild(const UMinMaxBox* box)
{
	auto x = box->Max.X <= Min.X || box->Min.X >= Max.X;		// false if there is netrivial intersection
	auto y = box->Max.Y <= Min.Y || box->Min.Y >= Max.Y;		// false if there is netrivial intersection
	auto z = box->Max.Z <= Min.Z || box->Min.Z >= Max.Z;		// false if there is netrivial intersection

	return x || y || z;
}

void UWeatherTargetsKDTree::GetContainingObjects(const UMinMaxBox* box, TArray<UObject*>& outArray, const UObject* ignoreElement)
{
	if (!(GtMin(box->Min) && LtMax(box->Max)))
		return;

	if (ChildHeap.Num() == 1)
	{
		if (!ignoreElement || !ignoreElement->IsValidLowLevel())
			return;

		/*if (ChildStack.Top()->ContainingObject == ignoreElement)
			return;

		if (outArray.Contains(ChildStack.Top()->ContainingObject))
			return;*/

			//TODO
			//if (!CheckCommonBoundaries(SingleChild->ContainingObject, ignoreElement))
		return;

		//outArray.Add(ChildStack.Top()->ContainingObject);
		return;
	}

	if (sum(box->Max * DividingCoord) <= DividingCoordValue)		// whole object is in left plane
	{
		if (!B1)
			return;
		B1->GetContainingObjects(box, outArray, ignoreElement);
		return;
	}

	if (sum(box->Min *DividingCoord) >= DividingCoordValue)		// whole object is in right plane
	{
		if (!B2)
			return;
		B2->GetContainingObjects(box, outArray, ignoreElement);
		return;
	}

	// object is in between. We need to split and then add object to both branches

	UMinMaxBox* newB1 = NewObject<UMinMaxBox>()->InitBox(box->Min, (FVector(1, 1, 1) - DividingCoord) *  box->Max + (DividingCoord * DividingCoordValue));
	UMinMaxBox* newB2 = NewObject<UMinMaxBox>()->InitBox((FVector(1, 1, 1) - DividingCoord) *  box->Min + (DividingCoord * DividingCoordValue), box->Max);

	GetContainingObjects(newB1, outArray, ignoreElement);
	GetContainingObjects(newB2, outArray, ignoreElement);
}

void UWeatherTargetsKDTree::GetContainingObjectsFromBottom(const UMinMaxBox* box, TArray<UObject*>& outArray, const UObject* ignoreElement)
{
	if (!(GtMin(box->Min) && LtMax(box->Max)))
	{
		auto parent = GetParent();
		ensure(parent != nullptr);
		parent->GetContainingObjectsFromBottom(box, outArray, ignoreElement);
		return;
	}

	GetContainingObjects(box, outArray, ignoreElement);
}

void UWeatherTargetsKDTree::UpdateAfterChildDestroyed()
{
	check(ChildHeap.Num() == 1 && !B1 && !B2);

	auto parent = GetParent();
	MarkPendingKill();
	if (parent && parent->IsValidLowLevel() && parent->canBeDeleted())
		parent->updateAfterChildDestroyedInner();
}

void UWeatherTargetsKDTree::updateAfterChildDestroyedInner()
{
	if (!canBeDeleted())
		return;

	check(ChildHeap.Num() == 1 || B1 || B2);

	auto hasSingle = checkElem(ChildHeap.Top());
	auto hasB1 = checkElem(B1);
	auto hasB2 = checkElem(B2);

	if (hasSingle || hasB1 || hasB2)
		return;

	auto parent = GetParent();
	MarkPendingKill();
	if (parent && parent->IsValidLowLevel() && parent->canBeDeleted())
		parent->updateAfterChildDestroyedInner();
}

#pragma optimize("", on)