

#include "TauCetiF2.h"
#include "MinMaxTree.h"


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
	check(ParentNode == nullptr);

	if (other->ParentNode != nullptr)
	{
		Insert(other->GetRoot());
		return;
	}

	if (GtMin(other->Min) && LtMax(other->Max))
	{
		// other is fully in my box
		print(TEXT("fully in me"));
		AppendChildren(other);
		return;
	}

	print(TEXT("MustExtend"));

	auto tmin = FVector(FMath::Min(Min.X, other->Min.X), FMath::Min(Min.Y, other->Min.Y), FMath::Min(Min.Z, other->Min.Z));
	auto tmax = FVector(FMath::Max(Max.X, other->Max.X), FMath::Max(Max.Y, other->Max.Y), FMath::Max(Max.Z, other->Max.Z));

	auto newParent = NewObject<UMinMaxTree>()->Init(tmin, tmax);
	newParent->AppendChildren(this);
	newParent->AppendChildren(other);
}