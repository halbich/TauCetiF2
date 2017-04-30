#include "Blocks.h"
#include "Creator.h"

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
	wholeBox->InitBox(wb->Min - FVector(1,1,-1) * GameDefinitions::CubeMinSize, twb->Max + FVector(1, 1, 1) * GameDefinitions::CubeMinSize);

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

	auto fbox = twb->GetBox();

	for (auto obj : objectBoxes)
	{
		if (obj.Value.Intersect(fbox) && IntersectExcl(obj.Value, fbox))
			return;
	}

	auto left = NewObject<UMinMaxBox>();
	left->InitBoxChecked(FVector(twb->Min.X, wholeBox->Min.Y, twb->Min.Z), FVector(twb->Max.X, twb->Min.Y, twb->Max.Z));
	if (!checkForBlock(left, objectBoxes))
		return;

	auto right = NewObject<UMinMaxBox>();
	right->InitBoxChecked(FVector(twb->Min.X, twb->Max.Y, twb->Min.Z), FVector(twb->Max.X, wholeBox->Max.Y, twb->Max.Z));
	if (!checkForBlock(right, objectBoxes))
		return;

	auto front = NewObject<UMinMaxBox>();
	front->InitBoxChecked(FVector(twb->Max.X, twb->Min.Y, twb->Min.Z), FVector(wholeBox->Max.X, twb->Max.Y, twb->Max.Z));
	if (!checkForBlock(front, objectBoxes))
		return;

	auto back = NewObject<UMinMaxBox>();
	back->InitBoxChecked(FVector(wholeBox->Min.X, twb->Min.Y, twb->Min.Z), FVector(twb->Min.X, twb->Max.Y, twb->Max.Z));
	if (!checkForBlock(back, objectBoxes))
		return;

	auto top = NewObject<UMinMaxBox>();
	top->InitBoxChecked(FVector(twb->Min.X, twb->Min.Y, twb->Max.Z), FVector(twb->Max.X, twb->Max.Y, wholeBox->Max.Z));
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

