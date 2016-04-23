

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
	UMinMaxTree* Init(UMinMaxBox* box);


};
