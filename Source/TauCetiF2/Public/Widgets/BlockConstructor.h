﻿#pragma once

#include "Widgets/TerminalBaseWidget.h"
#include "Game/TauCetiF2Character.h"
#include "Blocks/Public/Info/BuildableBlockInfo.h"
#include "Blocks/Public/Info/BlockHolder.h"
#include "BlockConstructor.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class TAUCETIF2_API UBlockConstructor : public UTerminalBaseWidget
{
	GENERATED_BODY()

public:

	UBlockConstructor();

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockConstructorSelector")
		UBlockHolder* blockHolder;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | BlockConstructorSelector")
		bool AddItemToInventory(UBuildableBlockInfo* buildable, TArray<FText>& validationErrors);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | BlockConstructorSelector")
		TArray<UBuildableBlockInfo*> GetAllBuildableBlocks();

private:

	FORCEINLINE bool ensureHolder()
	{
		if (blockHolder && blockHolder->IsValidLowLevel())
			return true;

		auto inst = Cast<UTCF2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (!inst)
			return false;

		blockHolder = Cast<UBlockHolder>(inst->BlockHolder);
		return blockHolder != NULL;
	}

	FORCEINLINE void AddImplicitTags(UBuildableBlockInfo* block)
	{
		block->Tags.Add(FString::Printf(TEXT("kX_%d"), (int32)block->Scale.X));
		block->Tags.Add(FString::Printf(TEXT("kY_%d"), (int32)block->Scale.Y));
		block->Tags.Add(FString::Printf(TEXT("kZ_%d"), (int32)block->Scale.Z));
	}
};
