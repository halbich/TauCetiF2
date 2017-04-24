#pragma once

#include "FBlockWithRelationshipInfo.h"
#include "FRelationshipInfo.h"

FORCEINLINE FArchive& operator<<(FArchive &Ar, FRelationshipInfo& relationInfo)
{
	Ar << relationInfo.TargetID;
	Ar << relationInfo.RelationshipType;
	return Ar;
}

FORCEINLINE FArchive& operator<<(FArchive &Ar, FBlockWithRelationshipInfo& blockRelationInfo)
{
	Ar << blockRelationInfo.ID;
	Ar << blockRelationInfo.Relationships;
	return Ar;
}
