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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient, Category = "TCF2 | BaseControlWidget")
		TArray<UTerminalBaseWidget*> ContainingTerminalWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCF2 | BaseControlWidget")
		TSubclassOf<class UTerminalBaseWidget> wTerminalBase;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BaseControlWidget")
		UTerminalBaseWidget* TerminalBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCF2 | BaseControlWidget")
		TSubclassOf<class UBlockConstructor> wBlockConstructor;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BaseControlWidget")
		UBlockConstructor* BlockConstructor;

	virtual void InitForBlock_Implementation(ABlock* block) override;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | BaseControlWidget")
		void LateInit();
};
