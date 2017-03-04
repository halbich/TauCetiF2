#pragma once

/**
 *
 */
class GAMESAVE_API FTagGroup
{
public:
	FTagGroup();

	FString GroupName;
	TArray<FString> Tags;
	bool LetVisibleAll;

	FORCEINLINE void Empty() {
		GroupName.Empty();
		Tags.Empty();
	}
};
