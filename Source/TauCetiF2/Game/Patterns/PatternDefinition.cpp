

#include "TauCetiF2.h"
#include "Helpers/BlockHelpers.h"
#include "PatternDefinition.h"

void UPatternDefinition::InitData()
{
	FVector currentMin = FVector::ZeroVector;
	FVector currentMax = FVector::ZeroVector;

	TArray<UMinMaxBox*> MinMaxBoxes;

	for (auto block : UsedBlocks)
	{
		auto definition = FBlockDefinitionHolder::Instance().GetDefinition(block->ID);
		auto spawnBox = BlockHelpers::GetSpawnBox(definition, block);
		MinMaxBoxes.Add(spawnBox);
		if (currentMin == currentMax && currentMin == FVector::ZeroVector)
		{
			currentMin = spawnBox->Min;
			currentMax = spawnBox->Max;
			continue;
		}

		currentMin = FVector(FMath::Min(currentMin.X, spawnBox->Min.X), FMath::Min(currentMin.Y, spawnBox->Min.Y), FMath::Min(currentMin.Z, spawnBox->Min.Z));
		currentMax = FVector(FMath::Max(currentMax.X, spawnBox->Max.X), FMath::Max(currentMax.Y, spawnBox->Max.Y), FMath::Max(currentMax.Z, spawnBox->Max.Z));


	}

	ensure(currentMin != currentMax && currentMin != FVector::ZeroVector);

	objectDimensions = NewObject<UMinMaxBox>()->InitBox(currentMin, currentMax);


	for (int32 i = 0; i < UsedBlocks.Num(); i++)
	{
		auto block = UsedBlocks[i];
		auto box = MinMaxBoxes[i];
		auto minMaxTree = NewObject<UMinMaxTree>()->Init(box);
	}

	InitDone = true;
}

void UPatternDefinition::DEBUGSpawnPattern(const FVector& worldOffset, TArray<UBlockInfo*>& outArray)
{
	for (auto block : UsedBlocks)
	{
		auto NewBlock = NewObject<UBlockInfo>(this, NAME_None, RF_NoFlags, block);
		NewBlock->Location += worldOffset;
		outArray.Add(NewBlock);
	}
}


