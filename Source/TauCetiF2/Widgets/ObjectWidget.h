

#pragma once

#include "Blueprint/UserWidget.h"
#include "ObjectWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWidgetCloseRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWidgetShown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWidgetRemoved, UObjectWidget*, RemovedWidget);

/**
 * 
 */
UCLASS(Blueprintable)
class TAUCETIF2_API UObjectWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "CustomWidgets|ObjectWidget")
		FWidgetCloseRequest OnWidgetCloseRequest;
	
	UFUNCTION(BlueprintCallable, Category = "CustomWidgets|ObjectWidget")
		void RequestClose();

	UPROPERTY(BlueprintAssignable, Category = "CustomWidgets|ObjectWidget")
		FWidgetShown OnWidgetShown;

	UPROPERTY(BlueprintAssignable, Category = "CustomWidgets|ObjectWidget")
		FWidgetRemoved OnWidgetRemoved;
	
	void OnEnterKey();
	
	// Returns if caller should be hidden from screen
	bool OnEscapeKey();

	void WidgetShown();

	UPROPERTY()
		TArray<UObjectWidget*> ItemsStack;

	UFUNCTION(BlueprintCallable, Category = "CustomWidgets|ObjectWidget")
		void AddToStack(UObjectWidget* widget);

	UFUNCTION(BlueprintCallable, Category = "CustomWidgets|ObjectWidget")
		void SwapWithTop(UObjectWidget* widget);

	UFUNCTION(BlueprintCallable, Category = "CustomWidgets|ObjectWidget")
		void AddToScreen(UObjectWidget* widget, int32 ZOrder = 0);

private:
	

	FORCEINLINE void notifyWidgetRemoved(UObjectWidget* widget)
	{
		OnWidgetRemoved.Broadcast(widget);
	}

	FORCEINLINE void pop()
	{
		auto widget = ItemsStack.Pop();
		notifyWidgetRemoved(widget);
	}
};
