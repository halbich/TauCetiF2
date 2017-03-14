#pragma once

#include "Block.h"
#include "Components/ActorComponent.h"
#include "BlockHolderComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLOCKS_API UBlockHolderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBlockHolderComponent();

	// Holder for aviable building block. Key is block's UID
	UPROPERTY(BlueprintReadonly, VisibleAnywhere, Transient, Category = "Blocks")
		TMap<int32, TSubclassOf<ABlock>> AviableBlocks;

	// Cache for obtaining block behavior
	UPROPERTY(BlueprintReadonly, VisibleAnywhere, Transient, Category = "Blocks")
		TMap<int32, UBlockDefinition*> AviableBlocksDefinitionCache;

	// Called when the game starts
	virtual void BeginPlay() override;

	void ReinitializeAviableBlocks();

	UBlockDefinition* GetDefinitionFor(int32 ID);

	// Returns all aviable UIDs of aviable blocks
	TArray<int32> GetAviableItems();

	static UBlockHolderComponent* instance;

	// Returns checked pointer or NULL
	static UBlockHolderComponent* GetInstance();
private:

	void tryAddBlockToAviables(UClass* blockClass);
};
