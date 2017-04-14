#pragma once

/**
 *
 */
struct GAMESAVE_API FTagGroup
{
	FTagGroup();

	FString GroupName;
	TArray<FString> Tags;
	bool LetVisibleAll;

	void Empty();
};
