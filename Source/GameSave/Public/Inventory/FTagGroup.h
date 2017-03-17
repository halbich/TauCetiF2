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

	void Empty();
	
};
