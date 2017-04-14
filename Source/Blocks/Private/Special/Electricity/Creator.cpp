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

UMinMaxBox* ACreator::GetWatchingBox()
{
	if (BlockInfo->Scale.X != BlockInfo->Scale.Y)
		return nullptr;

	if (watchingBox && watchingBox->IsValidLowLevel())
		return watchingBox;

	auto result = NewObject<UMinMaxBox>();

	auto db = WorldObjectComponent->DefiningBox;

	FVector min = db->Min;
	FVector max = db->Max;

	max.Z += (BlockInfo->Scale.X + 1) * GameDefinitions::CubeMinSize;

	auto mult = FVector(1, 1, 0) * GameDefinitions::CubeMinSize;

	min -= mult;
	max += mult;

	result->InitBox(min, max);

	watchingBox = result;
	return watchingBox;
}

void ACreator::CheckWatchingBox()
{
	print(TEXT("Checking region"));

	ensure(watchingBox);

	IsValidCreator = false;

	auto world = GetWorld();

	FlushPersistentDebugLines(world);

	watchingBox->DEBUGDrawBorder(GetWorld(), FColor::Orange, 30.0f);

	TArray<UObject*> objectsToCast;

	WorldObjectComponent->RootBox->GetContainingObjects(watchingBox, objectsToCast, this);

	TMap<ABlock*, FBox> objectBoxes;

	for (auto obj : objectsToCast)
	{
		auto bl = Cast<ABlock>(obj);
		ensure(bl);

		// we are interrested only in polycarbonate blocks
		if (bl->Definition.GetDefaultObject()->BlockID == CubePolycarbonateID)
			objectBoxes.Add(bl, bl->WorldObjectComponent->DefiningBox->GetBox());

		bl->WorldObjectComponent->DefiningBox->DEBUGDrawBorder(GetWorld(), FColor::White, 30.0f);
	}

	auto emptyBox = NewObject<UMinMaxBox>();

	FVector min = watchingBox->Min;
	FVector max = watchingBox->Max;

	auto mult = FVector(1, 1, 1) * GameDefinitions::CubeMinSize;

	min += mult;
	max -= mult;

	emptyBox->InitBox(min, max);
	auto fbox = emptyBox->GetBox();

	emptyBox->DEBUGDrawBorder(GetWorld(), FColor::Magenta, 30.0f);

	for (auto obj : objectBoxes)
	{
		if (obj.Value.Intersect(fbox) && IntersectExcl(obj.Value, fbox))
			return;
	}

	auto left = NewObject<UMinMaxBox>();
	left->InitBoxChecked(FVector(emptyBox->Min.X, watchingBox->Min.Y, emptyBox->Min.Z), FVector(emptyBox->Max.X, emptyBox->Min.Y, emptyBox->Max.Z));
	left->DEBUGDrawBorder(GetWorld(), FColor::Red, 30.0f);
	if (!checkForBlock(left, objectBoxes))
		return;

	auto right = NewObject<UMinMaxBox>();
	right->InitBoxChecked(FVector(emptyBox->Min.X, emptyBox->Max.Y, emptyBox->Min.Z), FVector(emptyBox->Max.X, watchingBox->Max.Y, emptyBox->Max.Z));
	if (!checkForBlock(right, objectBoxes))
		return;

	auto front = NewObject<UMinMaxBox>();
	front->InitBoxChecked(FVector(emptyBox->Max.X, emptyBox->Min.Y, emptyBox->Min.Z), FVector(watchingBox->Max.X, emptyBox->Max.Y, emptyBox->Max.Z));
	if (!checkForBlock(front, objectBoxes))
		return;

	auto back = NewObject<UMinMaxBox>();
	back->InitBoxChecked(FVector(watchingBox->Min.X, emptyBox->Min.Y, emptyBox->Min.Z), FVector(emptyBox->Min.X, emptyBox->Max.Y, emptyBox->Max.Z));
	if (!checkForBlock(back, objectBoxes))
		return;

	auto top = NewObject<UMinMaxBox>();
	top->InitBoxChecked(FVector(emptyBox->Min.X, emptyBox->Min.Y, emptyBox->Max.Z), FVector(emptyBox->Max.X, emptyBox->Max.Y, watchingBox->Max.Z));
	if (!checkForBlock(top, objectBoxes))
		return;

	IsValidCreator = true;

	print(TEXT("NoInnerIntersection"));
};

void ACreator::SetControlState_Implementation(bool isOn) {}
void ACreator::SetOutputPowerPercentage_Implementation(float percentage) {}
void ACreator::SetController_Implementation(ABlock* controller) {}
ABlock* ACreator::GetController_Implementation() { return NULL; }
#pragma optimize("", on)