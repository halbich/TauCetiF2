

#pragma once

#include "World/MinMaxBox.h"
#include "MinMaxTree.generated.h"

/**
 *
 */
UCLASS()
class TAUCETIF2_API UMinMaxTree : public UMinMaxBox
{
	GENERATED_BODY()

public:

	UMinMaxTree* Init(FVector min, FVector max);
	UMinMaxTree* Init(UMinMaxBox* box);

	UPROPERTY()
		TArray<UMinMaxTree*> Children;

	FORCEINLINE UMinMaxTree* GetRoot() {
		return GetRootNode<UMinMaxTree>(true);
	}


	void Insert(UMinMaxTree* other);

	FORCEINLINE UMinMaxTree* GetParent()
	{
		return GetParentNodeTemplate<UMinMaxTree>();
	}

	void ChildrenDeleted();

private:


	FORCEINLINE void AppendChildren(UMinMaxTree* child)
	{
		ensure(child);
		child->SetParent(this);
		Children.Add(child);
	}

};
