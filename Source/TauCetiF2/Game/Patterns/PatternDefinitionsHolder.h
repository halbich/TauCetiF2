

#pragma once

#include "Object.h"
#include "Blocks/Definitions/FBlockDefinition.h"
#include "Blocks/Definitions/FBlockDefinitionHolder.h"
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

	TArray<UBlockInfo*> DEBUGSpawnPatterns(const FVector& startingPoint);

	TArray<UBlockInfo*> DEBUGUsedPatternElements(const FVector& startingPoint);
private:


	UPatternDefinitionsHolder(const FObjectInitializer& ObjectInitializer);

	void Init();

	FORCEINLINE UBlockInfo* make(uint32 id, FVector location, FVector blockScale, FRotator blockRotation)
	{
		auto ret = NewObject<UBlockInfo>(this);
		ret->ID = id;
		ret->Location = location;
		ret->Scale = blockScale;
		ret->Rotation = blockRotation;
		return ret;

	}

	FORCEINLINE UBlockInfo* make(EBlockName id, FVector location, FVector blockScale, FRotator blockRotation)
	{
		return make((uint32)id, location, blockScale, blockRotation);
	}

};
