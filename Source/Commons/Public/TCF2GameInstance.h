#pragma once

#include "Engine/GameInstance.h"
#include "TCF2GameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDaytimeChangedDelegate, bool, isNight);

/**
 *
 */
UCLASS()
class COMMONS_API UTCF2GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient)
		UObject* BlockHolder;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | GameInstance")
		void SetHolderInstance(UObject* holder);

	UPROPERTY(BlueprintAssignable, Category = "TCF2 | GameInstance")
		FDaytimeChangedDelegate OnDaytimeChangedEvent;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | GameInstance")
		void DaytimeChanged(bool isNight);

	UPROPERTY(BlueprintReadOnly, Category = "TCF2 | GameInstance")
		bool IsNightInGame;
};
