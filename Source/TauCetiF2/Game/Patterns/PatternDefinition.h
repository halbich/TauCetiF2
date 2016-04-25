

#pragma once

#include "Object.h"
#include "Blocks/Info/BlockInfo.h"
#include "Blocks/Definitions/FBlockDefinition.h"
#include "Blocks/Definitions/FBlockDefinitionHolder.h"
#include "World/MinMaxTree.h"
#include "Game/Patterns/PatternElement.h"
#include "Game/Patterns/PatternGroup.h"
#include "PatternDefinition.generated.h"

/**
 *
 */
UCLASS()
class TAUCETIF2_API UPatternDefinition : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY()
		TArray<UBlockInfo*> UsedBlocks;

	

	UPROPERTY()
		TMap<int32, UPatternGroup*> Patterns;

	void InitData();

	UPROPERTY()
		bool InitDone;

	UPROPERTY()
		UMinMaxBox* objectDimensions;   //TODO used for debug purposes

	void DEBUGSpawnPattern(const FVector& worldOffset, TArray<UBlockInfo*>& outArray);

private:

	FORCEINLINE UPatternGroup* getGroup(int32 id)
	{
		UPatternGroup* group = nullptr;
		auto gf = Patterns.Find(id);
		if (gf)
		{
			group = *gf;
			check(group && group->IsValidLowLevel());
		}

		if (!group)
		{
			group = Patterns.Add(id, NewObject<UPatternGroup>()->Init(id));
		}

		return group;
	}
};
