

#pragma once

#include "GenericBlock.generated.h"

UINTERFACE(BlueprintType)
class BLOCKS_API UGenericBlock : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};


class BLOCKS_API IGenericBlock
{
	GENERATED_IINTERFACE_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TCF2 | Block")
		UStaticMeshComponent* GetMeshStructureComponent(int32 BlockMeshStructureDefIndex); 

};