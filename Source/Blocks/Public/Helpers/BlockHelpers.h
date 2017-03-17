#pragma once

#include "../Definitions/BlockDefinition.h"
#include "../Info/BlockInfo.h"
#include "Commons/Public/GameDefinitions.h"

#include "GameSave/Public/BlockSaveInfo.h"
#include "../Info/BlockBaseInfo.h"
#include "../Info/BlockInfo.h"
#include "../Info/BuildableBlockInfo.h"
#include "../Info/InventoryBuildableBlockInfo.h"

#include "../Info/Components/BlockWithOxygenInfo.h"
#include "../Info/Components/BlockWithElectricityInfo.h"
#include "GameSave/Public/BlockComponents/OxygenComponentInfo.h"
#include "GameSave/Public/BlockComponents/ElectricityComponentInfo.h"

#include "GameSave/Public/SaveGameCarrier.h"

/**
*
*/
namespace BlockHelpers
{
	static bool CheckBlockValidity(const UBlockDefinition* definition, const UBlockInfo* blockInfo, FString& reason)
	{
		// TODO
		return true;
	}

	static FORCEINLINE FVector GetLocalCoordinate(const FVector& vect)
	{
		return (vect / GameDefinitions::CubeMinSize).GridSnap(1);
	}

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

	static FTransform GetSpawnTransform(const UBlockDefinition* definition, const UBlockInfo* blockInfo)
	{
		FTransform trans;
		trans.SetScale3D(definition->GetMeshScale(blockInfo->Scale));
		trans.SetLocation(GetSpawnCoords(blockInfo->Location, definition->GetObjectScale(blockInfo->Scale), blockInfo->Rotation));
		trans.SetRotation(FQuat(blockInfo->Rotation));
		return trans;
	}

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

namespace BlockSavingHelpers {
	FORCEINLINE static void FromContainer(UBlockWithOxygenInfo* info, const FOxygenComponentInfo& block) {
		info->CurrentFillingValue = block.CurrentFillingValue;
	}

	FORCEINLINE static void ToContainer(FOxygenComponentInfo& block, const UBlockWithOxygenInfo* info) {
		block.CurrentFillingValue = info->CurrentFillingValue;
	}

	FORCEINLINE static void FromContainer(UBlockWithElectricityInfo* info, const FElectricityComponentInfo& block) {
		info->CurrentObjectEnergy = block.CurrentObjectEnergy;
	}

	FORCEINLINE static void ToContainer(FElectricityComponentInfo& block, const UBlockWithElectricityInfo* info) {
		block.CurrentObjectEnergy = info->CurrentObjectEnergy;
	}

	FORCEINLINE static void FromBaseContainer(UBlockBaseInfo* info, const FBlockBaseInfo& block) {
		info->ID = block.ID;
		info->Scale = block.Scale;
		info->Name = block.Name;
		info->AdditionalFlags = block.AdditionalFlags;

		if (block.HasOxygenData)
		{
			info->OxygenInfo = NewObject<UBlockWithOxygenInfo>();
			FromContainer(info->OxygenInfo, block.OxygenInfo);
		}

		if (block.HasElectricityData)
		{
			info->ElectricityInfo = NewObject<UBlockWithElectricityInfo>();
			FromContainer(info->ElectricityInfo, block.ElectricityInfo);
		}
	}

	FORCEINLINE static void ToBaseContainer(FBlockBaseInfo& block, const UBlockBaseInfo* info) {
		block.ID = info->ID;
		block.Scale = info->Scale;
		block.Name = info->Name;
		block.AdditionalFlags = info->AdditionalFlags;

		if (info->OxygenInfo && info->OxygenInfo->IsValidLowLevel())
		{
			block.HasOxygenData = true;
			ToContainer(block.OxygenInfo, info->OxygenInfo);
		}

		if (info->ElectricityInfo && info->ElectricityInfo->IsValidLowLevel())
		{
			block.HasElectricityData = true;
			ToContainer(block.ElectricityInfo, info->ElectricityInfo);
		}
	}

	FORCEINLINE static void FromContainer(UBlockInfo* info, const FBlockInfo& block) {
		FromBaseContainer(info, block);
		info->Location = block.Location;
		info->Rotation = block.Rotation;
	}

	FORCEINLINE static void ToContainer(FBlockInfo& block, const UBlockInfo* info) {
		ToBaseContainer(block, info);
		block.Location = info->Location;
		block.Rotation = info->Rotation;
	}

	FORCEINLINE static void SetBlockData(USaveGameCarrier* carrier, TArray<UBlockInfo*>& UsedBlocks)
	{
		carrier->usedBlocks.Empty();

		for (auto usedBlock : UsedBlocks)
		{
			FBlockInfo block;
			ToContainer(block, usedBlock);
			carrier->usedBlocks.Add(block);
		}
	}

	FORCEINLINE void FillData(USaveGameCarrier* carrier, UBlockWithOxygenInfo* OxygenInfo)
	{
		ToContainer(carrier->PlayerOxygenComponent, OxygenInfo);
	}

	FORCEINLINE void FillData(USaveGameCarrier* carrier, UBlockWithElectricityInfo* ElectricityInfo)
	{
		ToContainer(carrier->PlayerElectricityComponent, ElectricityInfo);
	}

	FORCEINLINE static TArray<UBlockInfo*> GetBlockData(USaveGameCarrier* carrier)
	{
		TArray<UBlockInfo*> result;
		for (auto block : carrier->usedBlocks)
		{
			auto NewItem = NewObject<UBlockInfo>();
			FromContainer(NewItem, block);
			result.Add(NewItem);
		}
		return result;
	}

	FORCEINLINE static UBlockWithOxygenInfo* GetOxygenInfo(USaveGameCarrier* carrier)
	{
		auto result = NewObject<UBlockWithOxygenInfo>();
		FromContainer(result, carrier->PlayerOxygenComponent);
		return result;
	}

	FORCEINLINE static UBlockWithElectricityInfo* GetElectricityInfo(USaveGameCarrier* carrier)
	{
		auto result = NewObject<UBlockWithElectricityInfo>();
		FromContainer(result, carrier->PlayerElectricityComponent);
		return result;
	}
}