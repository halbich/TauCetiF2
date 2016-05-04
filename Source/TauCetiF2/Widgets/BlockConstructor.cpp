

#include "TauCetiF2.h"
#include "BlockConstructor.h"


UBlockConstructor::UBlockConstructor() {

	FBlockDefinitionHolder::Instance().GetAllDefinitions(AllAviableBlocks);

}



