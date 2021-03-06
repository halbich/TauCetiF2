﻿#pragma once

#include "Blocks/Public/Block.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/SynchronizeWidget.h"
#include "ObjectWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWidgetCloseRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWidgetShown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWidgetRemoved, UObjectWidget*, RemovedWidget);

/**
 *
 */
UCLASS(Blueprintable, ClassGroup = (Custom))
class TAUCETIF2_API UObjectWidget : public USynchronizeWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Transient, Category = "TCF2 | CustomWidgets | ObjectWidget")
		FWidgetCloseRequest OnWidgetCloseRequest;

	UPROPERTY(BlueprintAssignable, Transient, Category = "TCF2 | CustomWidgets | ObjectWidget")
		FWidgetShown OnWidgetShown;

	UPROPERTY(BlueprintAssignable, Transient, Category = "TCF2 | CustomWidgets | ObjectWidget")
		FWidgetRemoved OnWidgetRemoved;

	UPROPERTY(Transient)
		TArray<UObjectWidget*> ItemsStack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | CustomWidgets | ObjectWidget")
		FString WidgetID;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "TCF2 | CustomWidgets | ObjectWidget")
		ABlock* InitedForBlock;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | CustomWidgets | ObjectWidget")
		void AddToStack(UObjectWidget* widget);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | CustomWidgets | ObjectWidget")
		virtual void SwapWithTop(UObjectWidget* widget);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | CustomWidgets | ObjectWidget")
		void AddToStackAndScreen(UObjectWidget* widget, int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | CustomWidgets | ObjectWidget")
		void RemoveTopWidgetFromStack();

	UFUNCTION(BlueprintCallable, Category = "TCF2 | CustomWidgets | ObjectWidget")
		void RequestClose();

	virtual void OnEnterKey();

	// Returns if caller should be hidden from screen
	virtual bool OnEscapeKey();

	void WidgetShown();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TCF2 | CustomWidgets | ObjectWidget")
		void InitForBlock(ABlock* block);

	virtual void InitForBlock_Implementation(ABlock* block);

private:

	FORCEINLINE void notifyWidgetRemoved(UObjectWidget* widget)
	{
		OnWidgetRemoved.Broadcast(widget);
	}

	FORCEINLINE void popAll() {
		while (ItemsStack.Num() > 0)
			pop();
	}

	FORCEINLINE void pop()
	{
		auto widget = ItemsStack.Pop();
		widget->popAll();
		notifyWidgetRemoved(widget);
		widget->notifyWidgetRemoved(widget);
	}
};
