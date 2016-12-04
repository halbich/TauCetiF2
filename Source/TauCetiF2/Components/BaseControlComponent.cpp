

#include "TauCetiF2.h"
#include "BaseControlComponent.h"


UBaseControlComponent::UBaseControlComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;


	if (BaseControl)
	{
	//	auto w = CreateWidget<UObjectWidget>(this, BaseControl);

	}

	//static ConstructorHelpers::FObjectFinder<UUserWidget> widget(TEXT("WidgetBlueprint'/Game/UMG/BaseControl/BaseControl.BaseControl'"));

	//if (widget.Succeeded())
	//{
	//	//BaseControlWidget =
	//	print(TEXT("asdasd"));
	//}

}




