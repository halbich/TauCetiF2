

#include "Blocks.h"
#include "BlockHolder.h"




void UBlockHolder::ReinitializeAviableBlocks()
{
	AviableBlocks.Empty();

	UObjectLibrary * lib = UObjectLibrary::CreateLibrary(ABlock::StaticClass(), true, GIsEditor);
	lib->AddToRoot();
	int32 loaded = lib->LoadBlueprintAssetDataFromPath("/Game/Blocks");
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::FromInt(loaded) + TEXT(" objects loaded for lib - "));


	TArray<FAssetData> Assets;
	lib->GetAssetDataList(Assets);
	//for (int32 i = 0; i < Assets.Num(); ++i) {
	//	FAssetData& assetData = Assets[i];
	//	
	//}

	//FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);
	//TArray<FAssetData> AssetData;
	//FARFilter Filter;
	//Filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
	//Filter.ClassNames.Add(UBlueprintGeneratedClass::StaticClass()->GetFName());
	//Filter.PackagePaths.Add("/Game/Blocks");
	//Filter.bRecursiveClasses = true;
	//Filter.bRecursivePaths = true;
	//AssetRegistryModule.Get().GetAssets(Filter, AssetData);

	for (auto& a : Assets)
	{


		/*auto loadedAsset = MyGetAsset(a);

		if (!loadedAsset)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Get ship: Failed to load object by GetAsset - ") + a.AssetName.ToString());*/

		/*GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("INFO Class Name - ") + a.GetClass()->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("INFO FullName - ") + a.GetFullName());
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("INFO AssetClass - ") + a.AssetClass.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("INFO ObjectPath - ") + a.ObjectPath.ToString());*/
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("INFO IsAssetLoaded - ") + (a.IsAssetLoaded() ? FString("YES") : FString("NO")));
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("INFO IsValid - ") + (a.IsValid() ? FString("YES") : FString("NO")));


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
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT(" ---- >  AddedGet ship: check obj - ") + blockClass->GetName());
		AviableBlocks.Add(def->BlockID, b->GetClass());
	}
	else
	{
		// TODO better approach
		print(NSLOCTEXT("TCF2LocSpace", "LC.BlockHoldeComp.Error_Key_Exist", "Key already existed! Skipping block").ToString());
	}
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