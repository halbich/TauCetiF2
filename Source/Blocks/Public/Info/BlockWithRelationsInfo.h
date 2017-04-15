

#pragma once

#include "UObject/NoExportTypes.h"
#include "RelationshipInfo.h"
#include "BlockWithRelationsInfo.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BLOCKS_API UBlockWithRelationsInfo : public UObject
{
	GENERATED_BODY()
	
public:
	UBlockWithRelationsInfo();

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockWithRelationsInfo")
		FGuid ID;
	
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockWithRelationsInfo")
		TArray<URelationshipInfo*> Relationships;
	
	int32 RemoveRelationshipsByTargetID(const FGuid& targetID);
};
