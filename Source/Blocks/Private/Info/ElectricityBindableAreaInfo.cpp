

#include "Blocks.h"
#include "ElectricityBindableAreaInfo.h"

#pragma optimize("", off)

void UElectricityBindableAreaInfo::InitArea(UBlockInfo* blockInfo, TArray<FVector> usedPoints, FElectricityBindableArea& definedPlane, FVector worldLocation)
{

	auto scaleMultipl = blockInfo->Scale * GameDefinitions::CubeMinSizeHalf;

	auto dominantPlane = FVector(1, 1, 1);

	// we need at least 3 points to define plane
	check(definedPlane.Plane.Num() >= 3);

	for (auto index : definedPlane.Plane)
	{
		check(usedPoints.IsValidIndex(index));

		auto origPoint = usedPoints[index];
		auto scaled = origPoint * scaleMultipl;
		auto rotated = blockInfo->Rotation.RotateVector(scaled).GridSnap(GameDefinitions::CubeMinSizeHalf);
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

	for (auto p : AreaPoints)
		DrawDebugSphere(world, p, 5, 10, FColor::Green, false, 60);
}

#pragma optimize("", on)