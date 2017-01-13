

#pragma once

#include "Components/ActorComponent.h"
#include "Block.h"

#include "BlockHolderComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLOCKS_API UBlockHolderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBlockHolderComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	void ReinitializeAviableBlocks();

	// Holder for aviable building block. Key is block's UID
	UPROPERTY(BlueprintReadonly, VisibleAnywhere, Transient, Category = "Blocks")
		TMap<int32, TSubclassOf<ABlock>> AviableBlocks;

	// Cache for obtaining block behavior
	UPROPERTY(BlueprintReadonly, VisibleAnywhere, Transient, Category = "Blocks")
		TMap<int32, UBlockDefinition*> AviableBlocksDefinitionCache;

	UBlockDefinition* GetDefinitionFor(int32 ID);

	// Returns all aviable UIDs of aviable blocks
	TArray<int32> GetAviableItems();

	static UBlockHolderComponent* instance;

	// Returns checked pointer or NULL
	static UBlockHolderComponent* GetInstance();
private:

	void tryAddBlockToAviables(UClass* blockClass);


};