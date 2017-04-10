#pragma once

#include "Definitions/BlockDefinition.h"
#include "GenericBlock.generated.h"

class UMinMaxBox;
class ABlock;

DECLARE_DELEGATE_OneParam(FDeleteRequestDelegate, ABlock*);
DECLARE_EVENT_OneParam(IGenericBlock, FDeleteRequestEvent, ABlock*);


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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TCF2 | Block")
		UPrimitiveComponent* GetComponentForObjectOutline();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TCF2 | Block")
		void UpdateBlockOnConstruction(UBlockDefinition* BlockDef);

	UMinMaxBox* GetDefiningBox();

	
};