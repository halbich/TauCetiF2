#pragma once

#include "MinMaxBox.h"
#include "MinMaxTree.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API UMinMaxTree : public UMinMaxBox
{
	GENERATED_BODY()

public:

	UMinMaxTree* Init(FVector min, FVector max);
	UMinMaxTree* Init(UMinMaxBox* box);

	UPROPERTY(Transient)
		TArray<UMinMaxTree*> Children;

	void Insert(UMinMaxTree* other);

	void ChildrenDeleted();

	FORCEINLINE UMinMaxTree* GetRoot() {
		return GetRootNode<UMinMaxTree>(true);
	}

	FORCEINLINE UMinMaxTree* GetParent()
	{
		return GetParentNodeTemplate<UMinMaxTree>();
	}

private:

	FORCEINLINE void AppendChildren(UMinMaxTree* child)
	{
		ensure(child);
		child->SetParent(this);
		Children.Add(child);
	}
};
