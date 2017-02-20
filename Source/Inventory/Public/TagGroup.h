#pragma once

#include "Object.h"
#include "TagGroup.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class INVENTORY_API UTagGroup : public UObject
{
	GENERATED_BODY()
public:
	UTagGroup();

	UPROPERTY(BlueprintReadWrite, Transient, Category = Tags)
		TArray<FString> Tags;

	UPROPERTY(BlueprintReadWrite, Transient, Category = Tags)
		FString GroupName;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Tags)
		static UTagGroup* GetEmpty();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Tags)
		static UTagGroup* GetFromTags(const TArray<FString>& tagList);

	bool IsSatisfied(TArray<FString>& tags)
	{
		for (auto groupTag : Tags)
		{
			for (auto tag : tags)
			{
				if (groupTag == tag)
					return true;
			}
		}

		return false;
	}
};