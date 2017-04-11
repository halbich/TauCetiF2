

#pragma once


#include "UObject/NoExportTypes.h"
#include "Block.h"
#include "BlockHolder.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BLOCKS_API UBlockHolder : public UObject
{
	GENERATED_BODY()
	
public:

	// Holder for aviable building block. Key is block's UID
	UPROPERTY(BlueprintReadonly, VisibleAnywhere, Transient, Category = "TCF2 | BlockHolder")
		TMap<int32, TSubclassOf<ABlock>> AviableBlocks;

	// Cache for obtaining block behavior
	UPROPERTY(BlueprintReadonly, VisibleAnywhere, Transient, Category = "Blocks")
		TMap<int32, UBlockDefinition*> AviableBlocksDefinitionCache;
	

	UFUNCTION(BlueprintCallable, Category = "TCF2 | BlockHolder")
		static UBlockHolder* GetHolderInstance();

	void ReinitializeAviableBlocks();

	UBlockDefinition* GetDefinitionFor(int32 ID);

	// Returns all aviable UIDs of aviable blocks
	TArray<int32> GetAviableItems();



private:

	void tryAddBlockToAviables(UClass* blockClass);
	
};
