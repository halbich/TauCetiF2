#pragma once

/**
 *
 */
struct FTagGroup
{

	FTagGroup();

	FString GroupName;
	TArray<FString> Tags;
	bool LetVisibleAll;

	FORCEINLINE void Empty() {
		GroupName.Empty();
		Tags.Empty();
	}
};
