

#include "TauCetiF2.h"
#include "BlockConstructorSelector.h"



UBlockConstructorSelector::UBlockConstructorSelector()
{
	if (BlockName == EBlockName::Empty)
	{
		print(TEXT("s"));
	}
}


void UBlockConstructorSelector::SynchronizeProperties()
{
	if (BlockName == EBlockName::Empty)
	{
		print(TEXT("s"));
	}

	Super::SynchronizeProperties();
}