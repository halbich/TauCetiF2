#pragma once

#include "Widgets/TerminalBaseWidget.h"
#include "Game/TauCetiF2Character.h"
#include "Blocks/Public/Info/BuildableBlockInfo.h"
#include "Blocks/Public/Info/BlockHolder.h"
#include "Blocks/Public/Info/PatternImplementation/CreatorPatternGroupInfo.h"
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

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockConstructorSelector")
		FVector ScaleLimit;

	UPROPERTY(Transient)
		UCreatorPatternGroupInfo* RelatedToPatternGroup;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | BlockConstructorSelector")
		bool AddItemToInventory(UBuildableBlockInfo* buildable, TArray<FText>& validationErrors);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | BlockConstructorSelector")
		TArray<UBuildableBlockInfo*> GetAllBuildableBlocks();

	virtual void InitForBlock_Implementation(ABlock* block) override;


	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "TCF2 | BlockConstructorSelector")
		FText GetSizeText();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | BlockConstructorSelector")
		bool GetIsValidBuilder();

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
};
