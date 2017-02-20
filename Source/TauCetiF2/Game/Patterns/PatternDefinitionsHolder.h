#pragma once

#include "Object.h"
#include "Helpers/BlockHelpers.h"
#include "Game/Patterns/PatternDefinition.h"
#include "Game/Patterns/PatternElement.h"
#include "Game/Patterns/PatternGroup.h"
#include "PatternDefinitionsHolder.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class TAUCETIF2_API UPatternDefinitionsHolder : public UObject
{
	GENERATED_BODY()

public:

	// TODO change singleton to gameinstance
	static UPatternDefinitionsHolder* Instance();

	UPROPERTY(Transient)
		TArray<UPatternDefinition*> UsedDefinitions;

	UPROPERTY(Transient)
		TMap<int32, UPatternGroup*> SearchPatterns;

	TArray<UBlockInfo*> DEBUGSpawnPatterns(const FVector& startingPoint);

	TArray<UBlockInfo*> DEBUGUsedPatternElements(const FVector& startingPoint);
private:

	UPatternDefinitionsHolder(const FObjectInitializer& ObjectInitializer);

	void Init();

	FORCEINLINE UBlockInfo* make(int32 id, FVector location, FVector blockScale, FRotator blockRotation)
	{
		auto ret = NewObject<UBlockInfo>(this);
		ret->ID = id;
		ret->Location = location;
		ret->Scale = blockScale;
		ret->Rotation = blockRotation;
		return ret;
	}

	/*FORCEINLINE UBlockInfo* make(EBlockName id, FVector location, FVector blockScale, FRotator blockRotation)
	{
		return make((int32)id, location, blockScale, blockRotation);
	}*/

	FORCEINLINE UPatternGroup* getGroup(int32 id)
	{
		UPatternGroup* group = nullptr;
		auto gf = SearchPatterns.Find(id);
		if (gf)
		{
			group = *gf;
			check(group && group->IsValidLowLevel());
		}

		if (!group)
		{
			group = SearchPatterns.Add(id, NewObject<UPatternGroup>()->Init(id));
		}

		return group;
	}

	// TODO FORCEINLINE
	void checkAllInit()
	{
		for (auto block : UsedDefinitions)
		{
			ensure(block->InitDone);
		}

		for (auto definition : UsedDefinitions)
		{
			for (auto blockElem : definition->UsedBlocks)
			{
				// TODO
				/*auto group = getGroup(blockElem->ID);
				if (FBlockDefinitionHolder::Instance().ShapeCanBeSubdivided(group->Definition->ShapeType))
				{
					UE_LOG(LogTemp, Warning, TEXT("INIT"));
				}
				else
				{
					group->Insert(blockElem);
				}*/
			}
		}
	}
};
