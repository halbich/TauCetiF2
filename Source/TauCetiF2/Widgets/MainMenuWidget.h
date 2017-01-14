

#pragma once

#include "Widgets/ObjectWidget.h"
#include "Widgets/MenuWidget.h"
#include "MainMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMainMenuCloseRequest);

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom))
class TAUCETIF2_API UMainMenuWidget : public UMenuWidget
{
	GENERATED_BODY()
	
	
public:

	UPROPERTY(BlueprintAssignable, Transient, Category = "CustomWidgets|ObjectWidget")
		FMainMenuCloseRequest OnMainMenuCloseRequest;
	
};
