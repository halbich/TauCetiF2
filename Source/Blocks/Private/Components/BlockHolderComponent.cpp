#include "Blocks.h"
#include "ARFilter.h"
#include "AssetRegistryModule.h"
#include "BlockHolderComponent.h"

#pragma optimize("", off)

UBlockHolderComponent* UBlockHolderComponent::instance = NULL;

// Sets default values for this component's properties
UBlockHolderComponent::UBlockHolderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ComponentTags.Add(TEXT("UBlockHolderComponent"));
}

// Called when the game starts
void UBlockHolderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UBlockHolderComponent::ReinitializeAviableBlocks()
{
	AviableBlocks.Empty();

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);
	TArray<FAssetData> AssetData;
	FARFilter Filter;
	Filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
	Filter.ClassNames.Add(UBlueprintGeneratedClass::StaticClass()->GetFName());
	Filter.PackagePaths.Add("/Game/Blocks");
	Filter.bRecursiveClasses = true;
	Filter.bRecursivePaths = true;
	AssetRegistryModule.Get().GetAssets(Filter, AssetData);

	for (auto& a : AssetData)
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

void UBlockHolderComponent::tryAddBlockToAviables(UClass* blockClass)
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
		AviableBlocks.Add(def->BlockID, b->GetClass());
	}
	else
	{
		// TODO better approach
		print(NSLOCTEXT("TCF2LocSpace", "LC.BlockHoldeComp.Error_Key_Exist", "Key already existed! Skipping block").ToString());
	}
}

TArray<int32> UBlockHolderComponent::GetAviableItems() {
	TArray<int32> keys;
	AviableBlocks.GenerateKeyArray(keys);
	return keys;
}

UBlockDefinition* UBlockHolderComponent::GetDefinitionFor(int32 ID)
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

UBlockHolderComponent* UBlockHolderComponent::GetInstance()
{
	if (instance && instance->IsValidLowLevel())
		return instance;

	return NULL;
}

#pragma optimize("", on)