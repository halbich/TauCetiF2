#pragma once

#include "Blueprint/UserWidget.h"
#include "SynchronizeWidget.generated.h"

/**
 *
 */
UCLASS()
class TAUCETIF2_API USynchronizeWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	/**
	* This is called after a widget is constructed and properties are synchronized.
	* It can also be called by the editor to update modified state.
	* Override this event in blueprint to update the widget after a default property is modified.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "TCF2 | User Interface")
		void OnSynchronizeProperties();

	virtual void SynchronizeProperties() override;



	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TCF2 | User Interface")
		void TutorialHighlight(float showForTime = -1.0f);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TCF2 | User Interface")
		void TutorialUnHighlight();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TCF2 | User Interface")
		void TutorialHide();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TCF2 | User Interface")
		void TutorialShow();
	

	UPROPERTY(Transient)
		TArray<USynchronizeWidget*> highlighted;

	UPROPERTY(Transient)
		TArray<USynchronizeWidget*> hidden;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | User Interface")
		TArray<USynchronizeWidget*> AllTutorialScreenWidgets;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | User Interface")
		void MakeHighlightScreen(TArray<USynchronizeWidget*> ToHighlight);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | User Interface")
		void MakeHighlightScreenSingle(USynchronizeWidget* ToHighlight);



};
