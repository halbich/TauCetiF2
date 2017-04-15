

#pragma once

#include "UObject/NoExportTypes.h"
#include "RelationshipInfo.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKS_API URelationshipInfo : public UObject
{
	GENERATED_BODY()
	
public:

	// Primary key of object which is referenced by this Foreign key
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | RelationshipInfo")
		FGuid TargetID;

	// Custom type of relationship
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | RelationshipInfo")
		uint8 RelationshipType;
	
	
};
