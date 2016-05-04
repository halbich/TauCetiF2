

#pragma once

#include "Widgets/ObjectWidget.h"
#include "Widgets/TerminalBaseWidget.h"
#include "Widgets/BlockConstructor.h"
#include "BaseControlWidget.generated.h"

/**
 * 
 */
UCLASS()
class TAUCETIF2_API UBaseControlWidget : public UObjectWidget
{
	GENERATED_UCLASS_BODY()
public:
	UBaseControlWidget();

	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = BaseControlWidget)
		TArray<UTerminalBaseWidget*> ContainingTerminalWidgets;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UTerminalBaseWidget> wTerminalBase;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		UTerminalBaseWidget* TerminalBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UBlockConstructor> wBlockConstructor;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		UBlockConstructor* BlockConstructor;

};
