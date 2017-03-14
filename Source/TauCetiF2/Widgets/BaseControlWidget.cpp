#include "TauCetiF2.h"
#include "BaseControlWidget.h"

UBaseControlWidget::UBaseControlWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (wTerminalBase)
	{
		/*TerminalBase = CreateWidget<UTerminalBaseWidget>(this, wTerminalBase);
		ContainingTerminalWidgets.Add(TerminalBase);*/
	}

	if (wBlockConstructor)
	{
		/*BlockConstructor = CreateWidget<UBlockConstructor>(this, wBlockConstructor);
		ContainingTerminalWidgets.Add(BlockConstructor);*/
	}
}