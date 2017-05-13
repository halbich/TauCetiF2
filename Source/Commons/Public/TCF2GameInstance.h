#pragma once

#include "Engine/GameInstance.h"
#include "TCF2GameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDaytimeChangedDelegate, bool, isNight, bool, calledByLevelLoad);

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
		void DaytimeChanged(bool isNight, bool calledByLevelLoad);

	UPROPERTY(BlueprintReadOnly, Category = "TCF2 | GameInstance")
		bool IsNightInGame;



	UPROPERTY(Transient)
		TArray<FText> LoadingErrorMessages;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | GameInstance")
		void AddErrorMessages(TArray<FText> messages);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | GameInstance")
		bool TakeErrorMessages(TArray<FText>& messages);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | GameInstance")
		void ClearErrorMessages();

	UPROPERTY(Transient)
		TArray<FText> LoadingWarningMessages;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | GameInstance")
		void AddWarningMessages(TArray<FText> messages);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | GameInstance")
		bool TakeWarningMessages(TArray<FText>& messages);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | GameInstance")
		void ClearWarningMessages();


};
