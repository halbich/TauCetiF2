#include "Blocks.h"
#include "ElectricityBindableAreaInfo.h"

void UElectricityBindableAreaInfo::InitArea(TArray<FVector> usedPoints, FElectricityBindableArea& definedPlane, FVector& blockScale, FRotator& blockRotation, FVector worldLocation)
{
	auto scaleMultipl = blockScale * GameDefinitions::CubeMinSizeHalf;

	auto dominantPlane = FVector(1, 1, 1);

	// we need at least 3 points to define plane
	check(definedPlane.Plane.Num() >= 3);

	for (auto index : definedPlane.Plane)
	{
		check(usedPoints.IsValidIndex(index));

		auto origPoint = usedPoints[index];
		auto scaled = origPoint * scaleMultipl;
		auto rotated = blockRotation.RotateVector(scaled).GridSnap(GameDefinitions::CubeMinSizeHalf);
		AreaPoints.Add(worldLocation + rotated);

		if (AreaPoints.Num() > 1)
		{
			auto diff = AreaPoints[AreaPoints.Num() - 1] - AreaPoints[AreaPoints.Num() - 2];
			dominantPlane *= FVector(diff.X == 0 ? 1 : 0, diff.Y == 0 ? 1 : 0, diff.Z == 0 ? 1 : 0);
		}
	}

	DominantPlane = dominantPlane * AreaPoints[0];
}

FBox UElectricityBindableAreaInfo::GetBox()
{
	FVector min = FVector(1, 1, 1) * GameDefinitions::WorldBorders;
	FVector max = (-1)* FVector(1, 1, 1) * GameDefinitions::WorldBorders;

	for (auto p : AreaPoints)
	{
		if (p.X < min.X)
			min.X = p.X;

		if (p.Y < min.Y)
			min.Y = p.Y;

		if (p.Z < min.Z)
			min.Z = p.Z;

		if (p.X > max.X)
			max.X = p.X;

		if (p.Y > max.Y)
			max.Y = p.Y;

		if (p.Z > max.Z)
			max.Z = p.Z;
	}

	return FBox(min, max);
}