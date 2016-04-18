

#pragma once

#include "GameFramework/GameUserSettings.h"
#include "QualityUserSettings.generated.h"

/**
 *
 */
UCLASS(BlueprintType, config = CustomQualitySettings)
class TAUCETIF2_API UQualityUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, BlueprintReadWrite, Category = QualitySettings)
		int32 test_number;
	UPROPERTY(Config, BlueprintReadWrite, Category = QualitySettings)
		FString test_string;

	UFUNCTION(BlueprintCallable, Category = QualitySettings)
		void TestSettings();
};
