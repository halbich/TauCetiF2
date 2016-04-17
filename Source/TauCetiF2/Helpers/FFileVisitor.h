

#pragma once

/**
 * 
 */
class TAUCETIF2_API FFileVisitor : public IPlatformFile::FDirectoryVisitor
{
public:


	TArray<FString>& Result;
	FString WildCard;
	bool bFiles;
	bool bDirectories;

	FFileVisitor(TArray<FString>& InResult, const FString& InWildCard, bool bInFiles, bool bInDirectories)
		: Result(InResult)
		, WildCard(InWildCard)
		, bFiles(bInFiles)
		, bDirectories(bInDirectories)
	{
	}

	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override;
};
