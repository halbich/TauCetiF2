#include "Commons.h"
#include "FFileVisitor.h"

bool FFileVisitor::Visit(const TCHAR * FilenameOrDirectory, bool bIsDirectory)
{
	auto res = FString(FilenameOrDirectory);
	// Pass back the file name
	FPaths::NormalizeFilename(res);

	new(Result)FString(res);

	return true;
}