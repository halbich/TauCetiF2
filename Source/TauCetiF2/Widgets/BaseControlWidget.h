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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient, Category = BaseControlWidget)
		TArray<UTerminalBaseWidget*> ContainingTerminalWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UTerminalBaseWidget> wTerminalBase;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Widgets")
		UTerminalBaseWidget* TerminalBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UBlockConstructor> wBlockConstructor;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Widgets")
		UBlockConstructor* BlockConstructor;
};
