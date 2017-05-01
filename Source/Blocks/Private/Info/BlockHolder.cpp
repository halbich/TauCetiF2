#include "Blocks.h"
#include "BlockHolder.h"

void UBlockHolder::ReinitializeAviableBlocks()
{
	AviableBlocks.Empty();

	UObjectLibrary * lib = UObjectLibrary::CreateLibrary(ABlock::StaticClass(), true, GIsEditor);
	lib->AddToRoot();
	int32 loaded = lib->LoadBlueprintAssetDataFromPath("/Game/Blocks");

	TArray<FAssetData> Assets;
	lib->GetAssetDataList(Assets);

	for (auto& a : Assets)
	{
		auto bp = Cast<UBlueprint>(a.GetAsset());
		if (bp)
		{
			auto gen = bp->GeneratedClass;
			if (gen && gen->IsChildOf(ABlock::StaticClass()))
			{
				tryAddBlockToAviables(gen);
				continue;
			}
		}

		auto generatedClassName = (a.AssetName.ToString() + "_C");
		UClass* genClass = FindObject<UClass>(a.GetPackage(), *generatedClassName);
		if (genClass && genClass->IsChildOf(ABlock::StaticClass()))
		{
			tryAddBlockToAviables(genClass);
			continue;
		}

		auto renamedClass = FindObject<UObjectRedirector>(a.GetPackage(), *generatedClassName);
		if (renamedClass)
		{
			auto renamedTarget = CastChecked<UClass>(renamedClass->DestinationObject);
			if (renamedTarget && renamedTarget->IsChildOf(ABlock::StaticClass()))
			{
				tryAddBlockToAviables(renamedTarget);
			}
		}
	}

	AviableBlocks.KeySort([](int32 A, int32 B) {
		return A < B; // sort keys in reverse
	});

}

void UBlockHolder::tryAddBlockToAviables(UClass* blockClass)
{
	auto b = NewObject<ABlock>(this, blockClass);
	check(b);

	if (!b->Definition)
		return;

	auto def = b->Definition->GetDefaultObject<UBlockDefinition>();

	if (!def)
		return;

	auto contains = AviableBlocks.Find(def->BlockID);
	if (!contains)
		AviableBlocks.Add(def->BlockID, b->GetClass());
	
}

TArray<int32> UBlockHolder::GetAviableItems() {
	TArray<int32> keys;
	AviableBlocks.GenerateKeyArray(keys);
	return keys;
}

UBlockDefinition* UBlockHolder::GetDefinitionFor(int32 ID)
{
	auto bl = AviableBlocksDefinitionCache.Find(ID);

	if (bl && (*bl)->IsValidLowLevel())
		return *bl;

	auto orig = AviableBlocks.Find(ID);

	if (!orig)
		return NULL;

	auto def = Cast<ABlock>(orig->GetDefaultObject());
	if (def && def->Definition)
	{
		auto toInsert = Cast<UBlockDefinition>(def->Definition->GetDefaultObject());

		AviableBlocksDefinitionCache.Add(ID, toInsert);
		return toInsert;
	}

	return NULL;
}

UBlockHolder* UBlockHolder::GetHolderInstance()
{
	auto result = NewObject<UBlockHolder>();
	result->ReinitializeAviableBlocks();
	return result;
}