#include "TauCetiF2.h"
#include "EngineUtils.h"
#include "Blocks/Public/Block.h"

#include "WorldController.h"

#pragma optimize("", off)

AWorldController::AWorldController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BlockHolder = ObjectInitializer.CreateDefaultSubobject<UBlockHolderComponent>(this, TEXT("Block Holder"));

	BaseControl = ObjectInitializer.CreateDefaultSubobject<UBaseControlComponent>(this, TEXT("Base Control"));

	pickableDelegates = TMap<ABlock*, FDelegateHandle>();
}

void AWorldController::loadBlocksArray(TArray<UBlockInfo*>& blocks) {
	auto world = GetWorld();
	if (!world)
		return;

	BlockHolder->instance = BlockHolder;

	BlockHolder->ReinitializeAviableBlocks();

	UsedBlocks.Empty();
	UsedBlocks.Reserve(blocks.Num());
	for (auto block : blocks)
	{
		SpawnWorldObject(world, block, true);
	}
}

bool AWorldController::DestroyWorldObject(ABlock* object)
{
	if (!object || !object->IsValidLowLevel() || object->IsPendingKill())
		return false;

	auto pickable = Cast<IPickableBlock>(object);
	if (pickable)
	{
		auto ref = pickableDelegates.Find(object);
		if(ref && ref->IsValid())
				pickable->RemovePickupItemEventListener(*ref);

	}


	auto count = UsedBlocks.Remove(object->BlockInfo);
	check(count == 1 && "Failed to remove block info.");
	object->Destroy();

	if (debugBoxesShown) {
		DEBUGHideMinMaxBoxes();
		DEBUGShowMinMaxBoxes();
	}

	check(RootBox && RootBox->IsValidLowLevel() && !RootBox->IsPendingKill());

	return true;
}

// TODO inline!
ABlock* AWorldController::SpawnWorldObject(UWorld* world, UBlockInfo* block, bool addToRoot)
{
	if (!block)
		return nullptr;

	check(RootBox != nullptr);

	auto definition = UBlockHolderComponent::GetInstance()->GetDefinitionFor(block->ID);

	if (!definition)
	{
		// TODO Localization!
		UE_LOG(LogTemp, Error, TEXT("Nezn�m� ID (%d) objektu. Vynech�v�m"), block->ID);
		return nullptr;
	}

	UMinMaxBox* box = BlockHelpers1::GetSpawnBox(definition, block);
	ensure(box != nullptr);
	if (!IsValidSpawnPoint(box))
	{
		// TODO Localization!
		UE_LOG(LogTemp, Error, TEXT("Objekt nelze korektn� p�idat do stromu. Vynech�v�m."));
		return nullptr;
	}

	FString invalidReason;
	bool isValid = BlockHelpers::CheckBlockValidity(definition, block, invalidReason);
	if (!isValid)
	{
		// TODO Localization!
		UE_LOG(LogTemp, Error, TEXT("Blok nen� validn�. D�vod: %s. nech�v�m"), *invalidReason);
		return nullptr;
	}

	auto classBP = UBlockHolderComponent::GetInstance()->AviableBlocks[block->ID];

	ensure(classBP != nullptr);

	auto trans = BlockHelpers::GetSpawnTransform(definition, block);
	auto actor = world->SpawnActorDeferred<ABlock>(classBP, trans, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	if (!actor)
	{
		// TODO Localization!
		UE_LOG(LogTemp, Error, TEXT("Nezn�m� ID (%d) objektu. Vynech�v�m"), block->ID);
		ensure(actor != nullptr);			// chceme vyhodit chybu
		return nullptr;
	}

	actor->SetBlockInfo(block);

	if (addToRoot) {
		auto MinMax = NewObject<UKDTree>()->Init(box);
		MinMax->ContainingObject = actor;
		UE_LOG(LogTemp, Log, TEXT("---   P�id�v�m do sv�ta objekt  %s"), *actor->GetName());


		auto woc = NewObject<UWorldObjectComponent>(actor);
		woc->Element = actor;
		woc->UpdateDefiningBox(MinMax);
		woc->RegisterComponent();

		RootBox->AddToTree(MinMax);
		for (auto usedBox : woc->TreeElements)
		{
			check(usedBox->GetRootNode<UKDTree>() == RootBox && TEXT("Used box don't have RootBox as ROOT !"));
			check(usedBox->ContainingObject == actor && TEXT("Used box has another ContainingObject than it should have!"));
		}


		auto pickable = Cast<IPickableBlock>(actor);
		if (pickable)
		{
			FPickupItemDelegate Subscriber;
			Subscriber.BindUObject(this, &AWorldController::onPickupItem);
			auto ListeningHandle = pickable->AddPickupItemEventListener(Subscriber);

			pickableDelegates.Add(actor, ListeningHandle);

		}

		//MinMax->DEBUGDrawContainingBox(GetWorld());
	}

	UGameplayStatics::FinishSpawningActor(actor, trans);

	if (addToRoot)
	{
		UsedBlocks.Add(block);
		if (debugBoxesShown) {
			DEBUGHideMinMaxBoxes();
			DEBUGShowMinMaxBoxes();
		}
	}

	return actor;
}

void AWorldController::DEBUGShowMinMaxBoxes() {
	if (RootBox)
	{
		debugBoxesShown = true;
		RootBox->DEBUGDrawContainingBox(GetWorld());
	}
	else
		print(TEXT("NO Root!"));
}

void AWorldController::DEBUGHideMinMaxBoxes() {
	if (RootBox)
	{
		FlushPersistentDebugLines(GetWorld());
		debugBoxesShown = false;
		//
	}
	else
		print(TEXT("NO Root!"));
}

void AWorldController::DEBUGSpawnPatterns(const FVector & startingPoint)
{
	print(TEXT("DEBUGSpawnPatterns"));

	auto blocks = UPatternDefinitionsHolder::Instance()->DEBUGSpawnPatterns(startingPoint);
	for (auto block : blocks)
	{
		SpawnWorldObject(GetWorld(), block, true);
	}
}

void AWorldController::DEBUGUsedPatternElements(const FVector & startingPoint)
{
	print(TEXT("DEBUGUsedPatternElements"));

	auto blocks = UPatternDefinitionsHolder::Instance()->DEBUGUsedPatternElements(startingPoint);
	for (auto block : blocks)
	{
		SpawnWorldObject(GetWorld(), block, true);
	}
}

void AWorldController::BeginPlay() {
	RootBox = NewObject<UKDTree>(GetTransientPackage(), TEXT("RootBox"));

	auto minCube = WorldHelpers::BorderToWorld(FVector(0, 0, 0));
	auto maxCube = WorldHelpers::BorderToWorld(GameDefinitions::WorldBorders);

	FVector min((minCube - 0.5 * FVector(1, 1, 1))* GameDefinitions::CubeMinSize);
	FVector max((maxCube + 0.5 * FVector(1, 1, 1))* GameDefinitions::CubeMinSize);

	RootBox->Init(min, max, 0);

	UPatternDefinitionsHolder::Instance();

	Super::BeginPlay();
}

void AWorldController::EndPlay(const EEndPlayReason::Type EndPlayReasonType)
{
	for (auto del : pickableDelegates)
	{
		if (del.Key && del.Key->IsValidLowLevel() && del.Value.IsValid())
		{
			auto pickable = Cast<IPickableBlock>(del.Key);
			if (pickable)
				pickable->RemovePickupItemEventListener(del.Value);
		}

	}

	for (auto block : UsedBlocks)
	{
	}



	Super::EndPlay(EndPlayReasonType);
}

void AWorldController::LoadDataFromCarrier(USaveGameCarrier* carrier)
{
	check(carrier != nullptr);

	auto usedBlocks = carrier->GetBlockData();

	loadBlocksArray(usedBlocks);
}

void AWorldController::SaveDataToCarrier(USaveGameCarrier* carrier)
{
	check(carrier != nullptr);

	carrier->FillData(UsedBlocks);
}

void AWorldController::onPickupItem(ABlock* pickingItem)
{


}

#pragma optimize("", on)