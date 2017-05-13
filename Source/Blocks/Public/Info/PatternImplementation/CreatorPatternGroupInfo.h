#pragma once

#include "Info/PatternGroupInfo.h"
#include "Commons/Public/GameDefinitions.h"
#include "CreatorPatternGroupInfo.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API UCreatorPatternGroupInfo : public UPatternGroupInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | CreatorPatternGroup")
		bool IsValidCreator;

	virtual void UpdateValidForObserve() override;

	virtual void WatchingRegionChanged() override;

	friend void ComputeCreator(ABlock* block);
};
