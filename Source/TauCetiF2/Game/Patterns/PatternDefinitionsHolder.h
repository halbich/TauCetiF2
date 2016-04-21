

#pragma once

#include "Object.h"
#include "Game/Patterns/PatternDefinition.h"
#include "PatternDefinitionsHolder.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class TAUCETIF2_API UPatternDefinitionsHolder : public UObject
{
	GENERATED_BODY()

public:


	static UPatternDefinitionsHolder* Instance();

	UPROPERTY()
	TArray<UPatternDefinition*> UsedDefinitions;
private:


	UPatternDefinitionsHolder(const FObjectInitializer& ObjectInitializer);

};
