#pragma optimize("", off)

#pragma once
#include "World/MinMaxBox.h"

/**
 *
 */
struct TAUCETIF2_API BlockHelpers
{
	//// TODO
	//static bool CheckBlockValidity(const FBlockDefinition* definition, const UBlockInfo* blockInfo, FString& reason)
	//{

	//	// TODO
	//	return true;
	//}

	//static bool CheckBlockValidity(const UBlockDefinition* definition, const UBlockInfo* blockInfo, FString& reason)
	//{

	//	// TODO
	//	return true;
	//}


	static FORCEINLINE FVector GetLocalCoordinate(const FVector& vect)
	{
		return (vect / GameDefinitions::CubeMinSize).GridSnap(1);
	}

private:
	static FORCEINLINE FVector GetWorldCoordinate(const FVector& vect)
	{
		return FVector(vect) * GameDefinitions::CubeMinSize;
	}

	static FORCEINLINE FVector GetSpawnOffset(const FRotator& rotator, const FVector& size)
	{
		auto transMove = FVector((int32)(size.X + 1) % 2, (int32)(size.Y + 1) % 2, (int32)(size.Z + 1) % 2) * 0.5;
		auto rotatedVect = rotator.RotateVector(transMove);
		return rotatedVect;
	}

	static FORCEINLINE FVector GetSpawnCoords(const FVector& localPosition, const FVector& size, const FRotator& rotator)
	{
		return GetWorldCoordinate(localPosition + GetSpawnOffset(rotator, size));
	}

public:

	//TODO delete
	/*static FORCEINLINE FTransform GetSpawnTransform(const FBlockDefinition* definition, const UBlockInfo* blockInfo)
	{
		FTransform trans;
		trans.SetScale3D(definition->GetMeshScale(blockInfo->Scale));
		trans.SetLocation(GetSpawnCoords(blockInfo->Location, definition->GetObjectScale(blockInfo->Scale), blockInfo->Rotation));
		trans.SetRotation(FQuat(blockInfo->Rotation));
		return trans;
	}*/

	static FORCEINLINE FTransform GetSpawnTransform(const UBlockDefinition* definition, const UBlockInfo* blockInfo)
	{
		FTransform trans;
		trans.SetScale3D(definition->GetMeshScale(blockInfo->Scale));
		trans.SetLocation(GetSpawnCoords(blockInfo->Location, definition->GetObjectScale(blockInfo->Scale), blockInfo->Rotation));
		trans.SetRotation(FQuat(blockInfo->Rotation));
		return trans;
	}

	// TODO delete this fn
	/*static UMinMaxBox* GetSpawnBox(const FBlockDefinition* definition, const UBlockInfo* blockInfo)
	{
		auto objectScale = definition->GetObjectScale(blockInfo->Scale).GridSnap(1);
		auto spawnCoord = GetSpawnCoords(blockInfo->Location, objectScale, blockInfo->Rotation);
		auto scaleHalf = blockInfo->Rotation.RotateVector(objectScale) * GameDefinitions::CubeMinSize* 0.5;
		return NewObject<UMinMaxBox>()->InitBoxChecked((spawnCoord - scaleHalf).GridSnap(GameDefinitions::CubeMinSizeHalf), (spawnCoord + scaleHalf).GridSnap(GameDefinitions::CubeMinSizeHalf));
	}*/

	static UMinMaxBox* GetSpawnBox(const UBlockDefinition* definition, const UBlockInfo* blockInfo)
	{
		auto objectScale = definition->GetObjectScale(blockInfo->Scale).GridSnap(1);
		auto spawnCoord = GetSpawnCoords(blockInfo->Location, objectScale, blockInfo->Rotation);
		auto scaleHalf = blockInfo->Rotation.RotateVector(objectScale) * GameDefinitions::CubeMinSize* 0.5;
		return NewObject<UMinMaxBox>()->InitBoxChecked((spawnCoord - scaleHalf).GridSnap(GameDefinitions::CubeMinSizeHalf), (spawnCoord + scaleHalf).GridSnap(GameDefinitions::CubeMinSizeHalf));
	}

	// TODo delete
	/*static  FVector GetSpawnPoint(const FVector& ImpactPointWithSnap, const FVector& ImpactNormal, const FBlockDefinition* definition, const UBlockInfo* blockInfo) {

		auto baseLocation = ImpactPointWithSnap / GameDefinitions::CubeMinSize;
		auto blockScale = definition->GetObjectScale(blockInfo->Scale);

		auto rotationScale = (blockInfo->Rotation.RotateVector(blockScale)* 0.5).GridSnap(0.5f).GetAbs();
		auto offsetInNormal = rotationScale * ImpactNormal.GetAbs();

		auto rotationOffset = GetSpawnOffset(blockInfo->Rotation, blockScale).GridSnap(0.5f);

		auto resCenAbs = ((rotationScale - ImpactNormal* rotationOffset) * ImpactNormal).GetAbs();

		return baseLocation + ImpactNormal * FVector(FMath::FloorToInt(resCenAbs.X), FMath::FloorToInt(resCenAbs.Y), FMath::FloorToInt(resCenAbs.Z));

	}*/

	static  FVector GetSpawnPoint(const FVector& ImpactPointWithSnap, const FVector& ImpactNormal, const UBlockDefinition* definition, const UBlockInfo* blockInfo) {

		auto baseLocation = ImpactPointWithSnap / GameDefinitions::CubeMinSize;
		auto blockScale = definition->GetObjectScale(blockInfo->Scale);

		auto rotationScale = (blockInfo->Rotation.RotateVector(blockScale)* 0.5).GridSnap(0.5f).GetAbs();
		auto offsetInNormal = rotationScale * ImpactNormal.GetAbs();

		auto rotationOffset = GetSpawnOffset(blockInfo->Rotation, blockScale).GridSnap(0.5f);

		auto resCenAbs = ((rotationScale - ImpactNormal* rotationOffset) * ImpactNormal).GetAbs();

		return baseLocation + ImpactNormal * FVector(FMath::FloorToInt(resCenAbs.X), FMath::FloorToInt(resCenAbs.Y), FMath::FloorToInt(resCenAbs.Z));

	}
};



#pragma optimize("", on)
