

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
	for (auto t : tagList)
		obj->Tags.Add(t);

	return obj;
}
