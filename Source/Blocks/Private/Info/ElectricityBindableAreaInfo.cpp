#include "Blocks.h"
#include "ElectricityBindableAreaInfo.h"

#pragma optimize("", off)

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

void UElectricityBindableAreaInfo::DEBUG_DrawPoints(UWorld* world)
{
	return;

	for (auto p : AreaPoints)
		DrawDebugSphere(world, p, 5, 10, FColor::Green, false, 60);

	for (int32 i = 0; i < AreaPoints.Num(); i++)
	{
		auto p1 = AreaPoints[i];
		auto p2 = AreaPoints[(i + 1) % AreaPoints.Num()];

		DrawDebugLine(world, p1, p2, FColor::Green, false, 60, 0, 3);
	}
	TArray<int> indices;
	indices.Add(0); indices.Add(1); indices.Add(2);

	if (AreaPoints.Num() == 4)
	{
		indices.Add(0); indices.Add(2); indices.Add(3);
	}

	DrawDebugMesh(world, AreaPoints, indices, FColor::Green, false, 30);
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

#pragma optimize("", on)