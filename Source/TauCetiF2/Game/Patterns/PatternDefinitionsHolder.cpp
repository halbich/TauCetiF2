

#include "TauCetiF2.h"
#include "PatternDefinitionsHolder.h"

#pragma optimize("", off)


TArray<UBlockInfo*> UPatternDefinitionsHolder::DEBUGSpawnPatterns(const FVector & startingPoint)
{
	return TArray<UBlockInfo*>();
}

TArray<UBlockInfo*> UPatternDefinitionsHolder::DEBUGUsedPatternElements(const FVector & startingPoint)
{
	return TArray<UBlockInfo*>();
}

UPatternDefinitionsHolder::UPatternDefinitionsHolder(const FObjectInitializer& ObjectInitializer) :
	UObject(ObjectInitializer)
{
	
}

void UPatternDefinitionsHolder::Init()
{
	auto blockDefinitions = FBlockDefinitionHolder::Instance().GetDefinition((uint32)EBlockName::ConstructCubeSide);

	auto test1 = NewObject<UPatternDefinition>(this);

	test1->UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(0, 0, 0), FVector(4, 4, 1), FRotator(0, 0, 0)));

	UsedDefinitions.Add(test1);
}

UPatternDefinitionsHolder* UPatternDefinitionsHolder::Instance()
{

	static UPatternDefinitionsHolder* instance = nullptr;

	if (instance == nullptr || !instance->IsValidLowLevel())
	{
		UClass *definitionsClass = UPatternDefinitionsHolder::StaticClass();
		instance = NewObject<UPatternDefinitionsHolder>();
		instance->Init();
	}

	return instance;

}


#pragma optimize("", on)