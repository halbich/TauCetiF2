#pragma once

#include "Blueprint/UserWidget.h"
#include "UsableBlockDefinition.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FUsableBlockDefinition
{
	GENERATED_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | UsableBlockDefinition")
		TSubclassOf<UUserWidget> ShowWidgetOnUse;

	FUsableBlockDefinition()
	{
	};
};
