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

#include "../Info/BlockWithRelationsInfo.h"
#include "../Info/RelationshipInfo.h"

#include "GameSave/Public/SaveGameCarrier.h"

/**
*
*/
namespace BlockHelpers
{
	static bool CheckBlockValidity(const UBlockDefinition* definition, UBlockInfo* blockInfo, FString& reason)
	{

		if (blockInfo->ElectricityInfo && !definition->HasElectricityComponent)
			blockInfo->ElectricityInfo = NULL;

		if (blockInfo->OxygenInfo && !definition->HasOxygenComponent)
			blockInfo->OxygenInfo = NULL;


		if (!definition->IsInLimits(blockInfo->Scale))
		{
			reason = FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.BlockInfo.Invalid_Dimensions", "Blok s ID {0} není v rozmezí platné velikosti. (Min: {1}, Max: {2}, Scale: {3})"), FText::AsNumber(blockInfo->ID), definition->MinBlockScale.ToText(), definition->MaxBlockScale.ToText(), (definition->HasCustomScaling ? definition->CustomBlockScale : blockInfo->Scale).ToText()).ToString();
			return false;
		}

		TArray<FText> validationErrors;
		if (!definition->ValidateFlags(blockInfo->AdditionalFlags, validationErrors))
		{
			FString result = validationErrors[0].ToString();

			for (auto i = 1; i < validationErrors.Num(); i++)
				result += TEXT(", %s") + validationErrors[i].ToString();

			reason = result;

			return false;
		}


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
	static void FromOxygenContainer(UBlockWithOxygenInfo* info, FOxygenComponentInfo& block) {
		info->CurrentObjectOxygen = block.CurrentObjectOxygen;
	}

	static void ToOxygenContainer(FOxygenComponentInfo& block, UBlockWithOxygenInfo* info) {
		block.CurrentObjectOxygen = info->CurrentObjectOxygen;
	}

	static void FromElectricityContainer(UBlockWithElectricityInfo* info, FElectricityComponentInfo& block) {
		info->CurrentObjectEnergy = block.CurrentObjectEnergy;
	}

	static void ToElectricityContainer(FElectricityComponentInfo& block, UBlockWithElectricityInfo* info) {
		block.CurrentObjectEnergy = info->CurrentObjectEnergy;
	}

	static void FromRelationsContainer(UBlockWithRelationsInfo* info, FBlockWithRelationshipInfo& block)
	{
		info->ID = block.ID;

		for (auto rel : block.Relationships)
		{
			auto NewItem = NewObject<URelationshipInfo>();
			NewItem->TargetID = rel.TargetID;
			NewItem->RelationshipType = rel.RelationshipType;
			info->Relationships.Add(NewItem);
		}
	}

	static void ToRelationsContainer(FBlockWithRelationshipInfo& block, UBlockWithRelationsInfo* info)
	{
		block.ID = info->ID;
		block.Relationships.Empty();

		for (auto rel : info->Relationships)
		{
			FRelationshipInfo NewItem;
			NewItem.TargetID = rel->TargetID;
			NewItem.RelationshipType = rel->RelationshipType;
			block.Relationships.Add(NewItem);
		}
	}

	static void FromBaseContainer(UBlockBaseInfo* info, FBlockBaseInfo& block) {
		info->ID = block.ID;
		info->Scale = block.Scale;
		info->Name = block.Name;
		info->AdditionalFlags = block.AdditionalFlags;

		if (block.HasOxygenData)
		{
			info->OxygenInfo = NewObject<UBlockWithOxygenInfo>();
			FromOxygenContainer(info->OxygenInfo, block.OxygenInfo);
		}

		if (block.HasElectricityData)
		{
			info->ElectricityInfo = NewObject<UBlockWithElectricityInfo>();
			FromElectricityContainer(info->ElectricityInfo, block.ElectricityInfo);
		}
	}

	static void ToBaseContainer(FBlockBaseInfo& block, UBlockBaseInfo* info) {
		block.ID = info->ID;
		block.Scale = info->Scale;
		block.Name = info->Name;
		block.AdditionalFlags = info->AdditionalFlags;

		if (info->OxygenInfo && info->OxygenInfo->IsValidLowLevel())
		{
			block.HasOxygenData = true;
			ToOxygenContainer(block.OxygenInfo, info->OxygenInfo);
		}

		if (info->ElectricityInfo && info->ElectricityInfo->IsValidLowLevel())
		{
			block.HasElectricityData = true;
			ToElectricityContainer(block.ElectricityInfo, info->ElectricityInfo);
		}
	}

	static void FromContainer(UBlockInfo* info, FBlockInfo& block) {
		FromBaseContainer(info, block);
		info->Location = block.Location;
		info->Rotation = block.Rotation;
		info->Health = block.Health;
		info->BlockSpecificData = block.BlockSpecificData;

		if (block.HasRelationshipData)
		{
			info->RelationsInfo = NewObject<UBlockWithRelationsInfo>();
			FromRelationsContainer(info->RelationsInfo, block.RelationshipInfo);
		}
	}

	static void ToContainer(FBlockInfo& block, UBlockInfo* info) {
		ToBaseContainer(block, info);
		block.Location = info->Location;
		block.Rotation = info->Rotation;
		block.Health = info->Health;
		block.BlockSpecificData = info->BlockSpecificData;

		if (info->RelationsInfo && info->RelationsInfo->IsValidLowLevel())
		{
			block.HasRelationshipData = true;
			ToRelationsContainer(block.RelationshipInfo, info->RelationsInfo);
		}
	}

	static void SetBlockData(USaveGameCarrier* carrier, TArray<UBlockInfo*>& UsedBlocks)
	{
		carrier->usedBlocks.Empty();

		for (auto usedBlock : UsedBlocks)
		{
			FBlockInfo block;
			ToContainer(block, usedBlock);
			carrier->usedBlocks.Add(block);
		}
	}

	static TArray<UBlockInfo*> GetBlockData(USaveGameCarrier* carrier)
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

	static void SetOxygenInfo(USaveGameCarrier* carrier, UBlockWithOxygenInfo* OxygenInfo)
	{
		ToOxygenContainer(carrier->PlayerOxygenComponent, OxygenInfo);
	}

	static UBlockWithOxygenInfo* GetOxygenInfo(USaveGameCarrier* carrier)
	{
		auto result = NewObject<UBlockWithOxygenInfo>();
		FromOxygenContainer(result, carrier->PlayerOxygenComponent);
		return result;
	}

	static void SetElectricityInfo(USaveGameCarrier* carrier, UBlockWithElectricityInfo* ElectricityInfo)
	{
		ToElectricityContainer(carrier->PlayerElectricityComponent, ElectricityInfo);
	}
	static UBlockWithElectricityInfo* GetElectricityInfo(USaveGameCarrier* carrier)
	{
		auto result = NewObject<UBlockWithElectricityInfo>();
		FromElectricityContainer(result, carrier->PlayerElectricityComponent);
		return result;
	}
}