

#pragma once

#include "Object.h"
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

};
