#pragma once

#include "Object.h"
#include "Game/Patterns/PatternElement.h"
#include "PatternGroup.generated.h"

/**
 *
 */
UCLASS()
class TAUCETIF2_API UPatternGroup : public UObject
{
	GENERATED_BODY()

public:

	UPatternGroup();

	UPROPERTY(Transient)
		int32 GroupID;

	UPROPERTY(Transient)
		TArray<UPatternElement*> Patterns;

	void Insert(UBlockInfo* info);

	UBlockDefinition* Definition;

	FORCEINLINE UPatternGroup* Init(int32 groupID)
	{
		GroupID = groupID;
		//Definition = UBlockHolderComponent::Instance().GetDefinition(GroupID);

		ensure(Definition != nullptr);
		return this;
	}
};
