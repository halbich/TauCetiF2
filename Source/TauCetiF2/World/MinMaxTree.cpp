

#include "TauCetiF2.h"
#include "MinMaxTree.h"

#pragma optimize("", off)

UMinMaxTree* UMinMaxTree::Init(FVector min, FVector max)
{
	InitBox(min, max);
	return this;
}

UMinMaxTree* UMinMaxTree::Init(UMinMaxBox* box)
{
	ensure(box != nullptr);
	return Init(box->Min, box->Max);
}

void UMinMaxTree::Insert(UMinMaxTree* other)
{
	check(GetParent() == nullptr);

	if (other->GetParent() != nullptr)
	{
		Insert(other->GetRoot());
		return;
	}

	if (GtMin(other->Min) && LtMax(other->Max))
	{
		// other is fully in my box
		//print(TEXT("fully in me"));
		AppendChildren(other);
		return;
	}

	//print(TEXT("MustExtend"));

	auto tmin = FVector(FMath::Min(Min.X, other->Min.X), FMath::Min(Min.Y, other->Min.Y), FMath::Min(Min.Z, other->Min.Z));
	auto tmax = FVector(FMath::Max(Max.X, other->Max.X), FMath::Max(Max.Y, other->Max.Y), FMath::Max(Max.Z, other->Max.Z));

	auto newParent = NewObject<UMinMaxTree>()->Init(tmin, tmax);
	newParent->AppendChildren(this);
	newParent->AppendChildren(other);
}

void UMinMaxTree::ChildrenDeleted()
{

	FVector currentMin = FVector::ZeroVector;
	FVector currentMax = FVector::ZeroVector;


	TArray<UMinMaxTree*> newChildren;
	for (auto child : Children)
	{
		if (child && child->IsValidLowLevel() && !child->IsPendingKill())
		{
			newChildren.Add(child);
			//print(TEXT("Child"));
			if (currentMin == currentMax && currentMin == FVector::ZeroVector)
			{
				currentMin = child->Min;
				currentMax = child->Max;
				continue;
			}

			currentMin = FVector(FMath::Min(currentMin.X, child->Min.X), FMath::Min(currentMin.Y, child->Min.Y), FMath::Min(currentMin.Z, child->Min.Z));
			currentMax = FVector(FMath::Max(currentMax.X, child->Max.X), FMath::Max(currentMax.Y, child->Max.Y), FMath::Max(currentMax.Z, child->Max.Z));
		}
	}

	Children = newChildren;


	if (Children.Num() == 0)
	{
		auto parent = GetParent();
		MarkPendingKill();
		if (parent)
			parent->ChildrenDeleted();
		return;
	}

	if (currentMin == Min && currentMax == Max)
		return;

	Min = currentMin;
	Max = currentMax;

	auto parent = GetParent();
	if (parent)
		parent->ChildrenDeleted();



}

#pragma optimize("", on)