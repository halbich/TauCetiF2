

#pragma once

#include "Object.h"
#include "Game/Inventory/Saving/FTagGroup.h"
#include "TagGroup.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class TAUCETIF2_API UTagGroup : public UObject
{
	GENERATED_BODY()
public:
	UTagGroup();

	UPROPERTY(BlueprintReadWrite, Category = Tags)
		TArray<FString> Tags;

	UPROPERTY(BlueprintReadWrite, Category = Tags)
		FString GroupName;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Tags)
		static UTagGroup* GetEmpty();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Tags)
		static UTagGroup* GetFromTags(const TArray<FString>& tagList);


	FORCEINLINE FTagGroup ToContainer() {
		FTagGroup result;
		result.GroupName = GroupName;
		result.Tags = TArray<FString>(Tags);
		return result;
	}

	FORCEINLINE void FromContainer(const FTagGroup& group) {
		GroupName = group.GroupName;
		Tags = TArray<FString>(group.Tags);
	}
};
