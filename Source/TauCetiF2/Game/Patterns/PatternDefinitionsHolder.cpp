

#include "TauCetiF2.h"
#include "PatternDefinitionsHolder.h"




UPatternDefinitionsHolder::UPatternDefinitionsHolder(const FObjectInitializer& ObjectInitializer) :
	UObject(ObjectInitializer)
{

}

UPatternDefinitionsHolder* UPatternDefinitionsHolder::Instance()
{

	static UPatternDefinitionsHolder* instance = NewObject<UPatternDefinitionsHolder>();

	return instance;

}