#pragma optimize("", off)

#pragma once
#include "Blocks/Implementations/BaseShapes/CubeObject.h"
#include "Blocks/Implementations/BaseShapes/CubeSideObject.h"
#include "Blocks/Implementations/BaseShapes/CubeBodyObject.h"
#include "Blocks/Implementations/Custom/TerminalObject.h"
#include "Blocks/Implementations/Custom/DoorObject.h"
#include "Blocks/Implementations/Custom/WindowObject.h"
#include "World/MinMaxBox.h"

/**
 *
 */
struct TAUCETIF2_API BlockHelpers
{

	static FORCEINLINE UClass* GetClassByShape(const FBlockDefinition* definition) {

		switch (definition->ShapeType)
		{
		case EShapeType::Cube: return ACubeObject::StaticClass();
		case EShapeType::CubeSide: return ACubeSideObject::StaticClass();
		case EShapeType::CubeBody: return ACubeBodyObject::StaticClass();
		case EShapeType::Custom: {

			if (definition->ID == (int32)EBlockName::Terminal)
				return ATerminalObject::StaticClass();
			else if (definition->ID == (int32)EBlockName::Door)
				return ADoorObject::StaticClass();
			else if (definition->ID == (int32)EBlockName::WindowPowered)
				return AWindowObject::StaticClass();
			break;
		}

		default:

			return nullptr;
			break;
		}

		return nullptr;
	}

	static  bool CheckBlockValidity(const FBlockDefinition* definition, const UBlockInfo* blockInfo, FString& reason)
	{
		print(TEXT("check validity"));

		// TODO
		return true;
	}


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
	static FORCEINLINE FTransform GetSpawnTransform(const FBlockDefinition* definition, const UBlockInfo* blockInfo)
	{
		FTransform trans;
		trans.SetScale3D(definition->GetMeshScale(blockInfo->Scale));
		trans.SetLocation(GetSpawnCoords(blockInfo->Location, definition->GetObjectScale(blockInfo->Scale), blockInfo->Rotation));
		trans.SetRotation(FQuat(blockInfo->Rotation));
		return trans;
	}

	static UMinMaxBox* GetSpawnBox(const FBlockDefinition* definition, const UBlockInfo* blockInfo)
	{
		auto objectScale = definition->GetObjectScale(blockInfo->Scale).GridSnap(1);
		auto spawnCoord = GetSpawnCoords(blockInfo->Location, objectScale, blockInfo->Rotation);
		auto scaleHalf = blockInfo->Rotation.RotateVector(objectScale) * GameDefinitions::CubeMinSize* 0.5;
		return NewObject<UMinMaxBox>()->InitBoxChecked((spawnCoord - scaleHalf).GridSnap(GameDefinitions::CubeMinSizeHalf), (spawnCoord + scaleHalf).GridSnap(GameDefinitions::CubeMinSizeHalf));
	}

	static  FVector GetSpawnPoint(const FVector& ImpactPointWithSnap, const FVector& ImpactNormal, const FBlockDefinition* definition, const UBlockInfo* blockInfo) {

		auto baseLocation = ImpactPointWithSnap / GameDefinitions::CubeMinSize;
		auto blockScale = definition->GetObjectScale(blockInfo->Scale);

		auto rotationScale = (blockInfo->Rotation.RotateVector(blockScale)* 0.5).GridSnap(0.5f).GetAbs();
		auto offsetInNormal = rotationScale * ImpactNormal.GetAbs();
		/*if (FMath::Frac(offsetInNormal.GetMax()) == 0.5)
			return baseLocation + ImpactNormal * FVector(FMath::RoundToInt(offsetInNormal.X), FMath::RoundToInt(offsetInNormal.Y), FMath::RoundToInt(offsetInNormal.Z));
*/
		

		auto rotationOffset = GetSpawnOffset(blockInfo->Rotation, blockScale).GridSnap(0.5f);

		auto resCenAbs = ((rotationScale - ImpactNormal* rotationOffset) * ImpactNormal).GetAbs();

		//if (ImpactNormal.GetMax() > 0)
			return baseLocation + ImpactNormal * FVector(FMath::FloorToInt(resCenAbs.X), FMath::FloorToInt(resCenAbs.Y), FMath::FloorToInt(resCenAbs.Z));
		//else    // we are heading to negative direction
			//return baseLocation + ImpactNormal * FVector(FMath::RoundToInt(resCenAbs.X), FMath::RoundToInt(resCenAbs.Y), FMath::RoundToInt(resCenAbs.Z));
	}

};



#pragma optimize("", on)
