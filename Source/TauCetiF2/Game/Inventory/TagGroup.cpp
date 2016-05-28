

#include "TauCetiF2.h"
#include "TagGroup.h"




UTagGroup::UTagGroup()
{
	GroupName = TEXT("Nová skupina");
}

UTagGroup* UTagGroup::GetEmpty()
{
	return NewObject<UTagGroup>();
}

UTagGroup* UTagGroup::GetFromTags(const TArray<FString>& tagList)
{
	auto obj = NewObject<UTagGroup>();

	obj->Tags.Reserve(tagList.Num());
	for (auto t : tagList) {
		auto trimmed = t.Trim();
		if (!obj->Tags.Contains<FString>(trimmed))
			obj->Tags.Add(trimmed);
	}

	return obj;
}
