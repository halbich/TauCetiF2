#include "TauCetiF2.h"
#include "FBlockAdditionalFlagsDefinition.h"

//FBlockAdditionalFlagsDefinition::FBlockAdditionalFlagsDefinition(FString tagID, FText displayText) : PossibleValues()
//{
//	TagID = tagID;
//	DisplayText = displayText;
//}
//
//FBlockAdditionalFlagsDefinition::FBlockAdditionalFlagsDefinition(ENamedTag namedTag, FText displayText) : FBlockAdditionalFlagsDefinition(GetNameForTag(namedTag), displayText)
//{
//}
//
//void FBlockAdditionalFlagsDefinition::SetValues(int8 n, ...)
//{
//
//	PossibleValues.Reserve(n);
//	va_list arguments;
//
//	va_start(arguments, n);
//	for (int32 x = 0; x < n; x++)
//	{
//		auto mat = va_arg(arguments, FBlockFlagValue);
//		PossibleValues.Add(mat);
//	}
//
//	va_end(arguments);
//
//}