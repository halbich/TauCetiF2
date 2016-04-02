

#include "TauCetiF2.h"
#include "FFileVisitor.h"





bool FFileVisitor::Visit(const TCHAR * FilenameOrDirectory, bool bIsDirectory)
{
	// Pass back the file name
	new(Result)FString(FPaths::GetCleanFilename(FilenameOrDirectory));

	return true;
}
