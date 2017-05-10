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

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | Tags")
		TArray<FString> Tags;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | Tags")
		FString GroupName;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | Tags")
		bool LetVisibleAll;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | Tags")
		static UTagGroup* GetEmpty();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | Tags")
		static UTagGroup* GetFromTags(const TArray<FString>& tagList);

	bool IsSatisfied(TArray<FString>& tags, TArray<FString>& implicitTags)
	{
		for (auto groupTag : Tags)
		{
			for (auto itag : implicitTags)
			{
				if (itag.Contains(groupTag))
					return true;
			}

			for (auto tag : tags)
			{
				if (tag.Contains(groupTag))
					return true;
			}
		}

		return false;
	}
};
