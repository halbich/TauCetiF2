#include "Blocks.h"
#include "Creator.h"

#pragma optimize("", off)
ACreator::ACreator()
	: Super()
{
	CreatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CreatorMesh"));
	CreatorMesh->SetupAttachment(GetRootComponent());

	ElectricityComponent = CreateDefaultSubobject<UElectricityComponent>(TEXT("ElectricityComponent"));
	AddOwnedComponent(ElectricityComponent);
}

UStaticMeshComponent* ACreator::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	switch (BlockMeshStructureDefIndex)
	{
	case 0:
		return CreatorMesh;
	default:
		return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
	}
}

UPrimitiveComponent* ACreator::GetComponentForObjectOutline_Implementation() {
	return CreatorMesh;
}

UPatternGroupInfo* ACreator::GetPatternGroupImpl() {
	return NewObject<UCreatorPatternGroupInfo>();
}

void ACreator::ComputeCreator()
{
	auto pi = Cast<UCreatorPatternGroupInfo>(WorldObjectComponent->PatternGroupInfo);
	ensure(pi);

	auto wb = pi->WatchingBox;
	ensure(wb);

	auto twb = pi->TreeWatchingBox;
	ensure(twb);

	pi->IsValidCreator = false;

	auto world = GetWorld();

	auto wholeBox = NewObject<UMinMaxBox>();
	wholeBox->InitBox(wb->Min, twb->Max);

	TArray<UObject*> objectsToCast;

	WorldObjectComponent->RootBox->GetContainingObjects(wholeBox, objectsToCast);

	TMap<ABlock*, FBox> objectBoxes;

	TMap<ABlock*, FBox> creatorBoxes;

	for (auto obj : objectsToCast)
	{
		auto bl = Cast<ABlock>(obj);
		ensure(bl);

		// we are interrested only in polycarbonate blocks or creators
		if (bl->BlockInfo->ID == CubePolycarbonateID)
			objectBoxes.Add(bl, bl->WorldObjectComponent->DefiningBox->GetBox());

		if (bl->BlockInfo->ID == CreatorID)
			creatorBoxes.Add(bl, bl->WorldObjectComponent->DefiningBox->GetBox());
	}

	// check base for blocks
	if (!checkForBlock(wb, creatorBoxes))
		return;

	auto emptyBox = NewObject<UMinMaxBox>();

	FVector min = twb->Min;
	FVector max = twb->Max;

	auto mult = FVector(1, 1, 1) * GameDefinitions::CubeMinSize;

	min += mult;
	max -= mult;

	emptyBox->InitBox(min, max);
	auto fbox = emptyBox->GetBox();

	for (auto obj : objectBoxes)
	{
		if (obj.Value.Intersect(fbox) && IntersectExcl(obj.Value, fbox))
			return;
	}

	auto left = NewObject<UMinMaxBox>();
	left->InitBoxChecked(FVector(emptyBox->Min.X, twb->Min.Y, emptyBox->Min.Z), FVector(emptyBox->Max.X, emptyBox->Min.Y, emptyBox->Max.Z));
	if (!checkForBlock(left, objectBoxes))
		return;

	auto right = NewObject<UMinMaxBox>();
	right->InitBoxChecked(FVector(emptyBox->Min.X, emptyBox->Max.Y, emptyBox->Min.Z), FVector(emptyBox->Max.X, twb->Max.Y, emptyBox->Max.Z));
	if (!checkForBlock(right, objectBoxes))
		return;

	auto front = NewObject<UMinMaxBox>();
	front->InitBoxChecked(FVector(emptyBox->Max.X, emptyBox->Min.Y, emptyBox->Min.Z), FVector(twb->Max.X, emptyBox->Max.Y, emptyBox->Max.Z));
	if (!checkForBlock(front, objectBoxes))
		return;

	auto back = NewObject<UMinMaxBox>();
	back->InitBoxChecked(FVector(twb->Min.X, emptyBox->Min.Y, emptyBox->Min.Z), FVector(emptyBox->Min.X, emptyBox->Max.Y, emptyBox->Max.Z));
	if (!checkForBlock(back, objectBoxes))
		return;

	auto top = NewObject<UMinMaxBox>();
	top->InitBoxChecked(FVector(emptyBox->Min.X, emptyBox->Min.Y, emptyBox->Max.Z), FVector(emptyBox->Max.X, emptyBox->Max.Y, twb->Max.Z));
	if (!checkForBlock(top, objectBoxes))
		return;

	pi->IsValidCreator = true;
};

void ComputeCreator(ABlock* block)
{
	auto c = Cast<ACreator>(block);
	ensure(c);
	c->ComputeCreator();
}

#pragma optimize("", on)