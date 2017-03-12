#include "TauCetiF2.h"
#include "CmbItem.h"

UCmbItem* UCmbItem::MakeCmbItem(FText displayText, int32 itemValue)
{
	auto result = NewObject<UCmbItem>();
	result->Text = displayText;
	result->Value = itemValue;
	return result;
}