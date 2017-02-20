#include "TauCetiF2.h"
#include "DeleteButton.h"

UDeleteButton::UDeleteButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset> deleteBtnStyle(TEXT("SlateWidgetStyleAsset'/Game/UMG/Styles/ButtonDeleteStyle.ButtonDeleteStyle'"));

	ensure(deleteBtnStyle.Succeeded());

	auto btnStyle = Cast<UButtonWidgetStyle>(deleteBtnStyle.Object->CustomStyle);
	if (btnStyle)
		SetStyle(btnStyle->ButtonStyle);
}