

#pragma once

#include "Engine/GameInstance.h"
#include "TCF2GameInstance.generated.h"

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
};
