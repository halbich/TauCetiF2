

#pragma once

#include "Object.h"
#include "Blocks/Public/Info/BlockInfo.h"
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

	UPROPERTY(Transient)
		TArray<UBlockInfo*> UsedBlocks;

	UPROPERTY(Transient)
		TArray<FRotator> AviableRotations;

	UPROPERTY(Transient)
		TMap<int32, UPatternGroup*> Patterns;
	
	UPROPERTY(Transient)
		bool InitDone;

	UPROPERTY(Transient)
		UMinMaxBox* objectDimensions;   //TODO used for debug purposes

	UPROPERTY(Transient)
		TArray<UMinMaxTree*> ResultObjectTrees;


	void InitData();

	void DEBUGSpawnPattern(const FVector& worldOffset, TArray<UBlockInfo*>& outArray);

	FORCEINLINE void SetRotationsFullYaw()
	{
		AviableRotations.Empty(4);
		AviableRotations.AddDefaulted(4);
		AviableRotations[1] = FRotator(0, 90, 0);
		AviableRotations[1] = FRotator(0, 180, 0);
		AviableRotations[1] = FRotator(0, 270, 0);
	}
	FORCEINLINE void SetRotationsFull()
	{
		AviableRotations.Empty(24);

		AviableRotations.AddDefaulted(24);

		AviableRotations[1] = FRotator(0, 0, 90);
		AviableRotations[2] = FRotator(0, 0, 180);
		AviableRotations[3] = FRotator(0, 0, 270);

		AviableRotations[4] = FRotator(0, 90, 0);
		AviableRotations[5] = FRotator(0, 90, 90);
		AviableRotations[6] = FRotator(0, 90, 180);
		AviableRotations[7] = FRotator(0, 90, 270);

		AviableRotations[8] = FRotator(0, 180, 0);
		AviableRotations[9] = FRotator(0, 180, 90);
		AviableRotations[10] = FRotator(0, 180, 180);
		AviableRotations[11] = FRotator(0, 180, 270);

		AviableRotations[12] = FRotator(0, 270, 0);
		AviableRotations[13] = FRotator(0, 270, 90);
		AviableRotations[14] = FRotator(0, 270, 180);
		AviableRotations[15] = FRotator(0, 270, 270);

		AviableRotations[16] = FRotator(90, 0, 0);
		AviableRotations[17] = FRotator(90, 0, 90);
		AviableRotations[18] = FRotator(90, 0, 180);
		AviableRotations[19] = FRotator(90, 0, 270);

		AviableRotations[20] = FRotator(270, 0, 0);
		AviableRotations[21] = FRotator(270, 0, 90);
		AviableRotations[22] = FRotator(270, 0, 180);
		AviableRotations[23] = FRotator(270, 0, 270);
	}

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
