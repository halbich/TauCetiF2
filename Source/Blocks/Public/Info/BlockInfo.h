#pragma once

#include "BlockBaseInfo.h"
#include "BlockWithRelationsInfo.h"
#include "Commons/Public/Enums.h"
#include "BlockInfo.generated.h"

/*
 *	Block info with Location and Scale, used in game. For saving see FBlockInfo
 */
UCLASS(BlueprintType)
class BLOCKS_API UBlockInfo : public UBlockBaseInfo
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockInfo")
		FVector Location;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockInfo")
		FRotator Rotation;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockInfo")
		bool UnderConstruction;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockInfo")
		float Health;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockInfo")
		float MaxHealth;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockInfo")
		TMap<FString, FString> BlockSpecificData;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockInfo")
		UBlockWithRelationsInfo* RelationsInfo;

	UPROPERTY(Transient)
		EHealthSeverity HealthSeverity;


	// updates severity of this block. Returns if severity has changed.
	FORCEINLINE bool UpdateHealthSeverity()
	{
		auto old = HealthSeverity;

		EHealthSeverity newSev;
		ensure(MaxHealth > 0);
		ensure(Health <= MaxHealth);

		auto ratio = Health / MaxHealth;

		if (ratio == 1.0f)
			newSev = EHealthSeverity::OK;
		else if (ratio >= 0.5f)
			newSev = EHealthSeverity::ToRepair;
		else if (ratio >= 0.25f)
			newSev = EHealthSeverity::Important;
		else if (ratio > 0)
			newSev = EHealthSeverity::Critical;
		else
			newSev = EHealthSeverity::Dead;


		if (old != newSev)
		{
			HealthSeverity = newSev;
			return true;
		}

		return false;
	}
};
