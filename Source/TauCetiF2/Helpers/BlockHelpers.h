#pragma optimize("", off)

#pragma once
#include "Blocks/Implementations/BaseShapes/CubeObject.h"
#include "Blocks/Implementations/BaseShapes/CubeSideObject.h"
#include "Blocks/Implementations/BaseShapes/CubeBodyObject.h"
#include "Blocks/Implementations/Custom/TerminalObject.h"
#include "Blocks/Implementations/Custom/DoorObject.h"
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

			if (definition->ID == (uint32)EBlockName::Terminal)
				return ATerminalObject::StaticClass();
			else if (definition->ID == (uint32)EBlockName::Door)
				return ADoorObject::StaticClass();

			break;
		}

		default:

			return nullptr;
			break;
		}

		return nullptr;
	}

	static FORCEINLINE bool CheckBlockValidity(const FBlockDefinition* definition, const UBlockInfo* blockInfo, FString& reason)
	{
		// TODO
		return true;
	}


private:
	static FORCEINLINE FVector GetWorldCoordinate(const FVector& vect)
	{
		return FVector(vect) * GameDefinitions::CubeMinSize;
	}

	static FORCEINLINE FVector GetCleaned(const FVector& vector)
	{
		return FVector((int32)vector.X, (int32)vector.Y, (int32)vector.Z);
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

	static FORCEINLINE UMinMaxBox* GetSpawnBox(const FBlockDefinition* definition, const UBlockInfo* blockInfo)
	{
		auto objectScale = GetCleaned(definition->GetObjectScale(blockInfo->Scale));
		auto spawnCoord = GetSpawnCoords(blockInfo->Location, objectScale, blockInfo->Rotation);
		auto scaleHalf = blockInfo->Rotation.RotateVector(objectScale) * GameDefinitions::CubeMinSize* 0.5;
		return NewObject<UMinMaxBox>()->InitBoxChecked(spawnCoord - scaleHalf, spawnCoord + scaleHalf);
	}

	static  FVector GetSpawnPoint(const FVector& ImpactPointWithSnap, const FVector& ImpactNormal, const FBlockDefinition* definition, const UBlockInfo* blockInfo) {

		auto baseLocation = ImpactPointWithSnap / GameDefinitions::CubeMinSize;
		auto blockScale = definition->GetObjectScale(blockInfo->Scale);
		auto rotatedScale = blockInfo->Rotation.RotateVector(blockScale);
		auto offset = ImpactNormal *GetSpawnOffset(blockInfo->Rotation, blockScale);
		auto normalAdd = ImpactNormal * rotatedScale.GetAbs() * 0.5 - offset;
		auto normA = FVector(FMath::FloorToInt(normalAdd.X), FMath::FloorToInt(normalAdd.Y), FMath::FloorToInt(normalAdd.Z));
		auto result = baseLocation + normA;
		auto snap = result.GridSnap(GameDefinitions::CubeMinSize);
		return result;
	}

};


#pragma optimize("", on)

