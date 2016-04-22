

#include "TauCetiF2.h"
#include "PatternDefinitionsHolder.h"




UPatternDefinitionsHolder::UPatternDefinitionsHolder(const FObjectInitializer& ObjectInitializer) :
	UObject(ObjectInitializer)
{

}

UPatternDefinitionsHolder* UPatternDefinitionsHolder::Instance()
{

	static UPatternDefinitionsHolder* instance = nullptr;

	if (instance == nullptr || !instance->IsValidLowLevel())
	{
		UClass *definitionsClass = UPatternDefinitionsHolder::StaticClass();
		instance = NewObject<UPatternDefinitionsHolder>();
		//instance = Cast<UPatternDefinitionsHolder>(ConstructObject<UObject>(definitionsClass));
	}

	return instance;

}