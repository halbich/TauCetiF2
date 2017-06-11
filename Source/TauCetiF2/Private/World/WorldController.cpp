#include "TauCetiF2.h"
#include "EngineUtils.h"
#include "Blocks/Public/Block.h"
#include "WorldController.h"

AWorldController::AWorldController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	pickableDelegates = TMap<ABlock*, FDelegateHandle>();
	showableWidgetDelegates = TMap<ABlock*, FDelegateHandle>();
}

void AWorldController::loadBlocksArray(TArray<UBlockInfo*>& blocks) {
	auto world = GetWorld();
	if (!world)
		return;

	TArray<FText> validationErrors;

	UsedBlocks.Empty();
	UsedBlocks.Reserve(blocks.Num());
	for (auto block : blocks)
	{
		if (block->Health <= 0)
			continue;

		SpawnWorldObject(world, block, validationErrors, true);
	}

	if (validationErrors.Num() > 0)
	{
		auto inst = Cast<UTCF2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		ensure(inst);
		inst->AddWarningMessages(validationErrors);
	}
}

bool AWorldController::DestroyWorldObject(ABlock* object)
{
	if (!object || !object->IsValidLowLevel() || object->IsPendingKill())
		return false;

	object->OnDestroyRequestedEvent.RemoveDynamic(this, &AWorldController::DestroyRequestEventHandler);

	TQueue<ABlock*> patternRecompute;

	auto pi = object->WorldObjectComponent->PatternGroupInfo;
	if (object->Definition.GetDefaultObject()->UsingInPatterns && pi && pi->IsValidLowLevel())
	{
		pi->UnregisterBlock(object);

		for (auto pr : pi->BlocksInGroup)
		{
			auto pim = pr->GetPatternGroupImpl();
			pim->RegisterBlock(pr);
			pr->WorldObjectComponent->PatternGroupInfo = pim;
			patternRecompute.Enqueue(pr);
		}

		pi->BlocksInGroup.Empty();
		RootBox->TryUnregisterWatchingGroup(pi);
		pi->MarkPendingKill();
	}

	auto usableWithWidget = Cast<IBlockWithShowableWidget>(object);
	if (usableWithWidget)
	{
		auto showedWidget = Cast<UObjectWidget>(usableWithWidget->GetShownWidget());
		if (showedWidget && showedWidget->IsValidLowLevel() && showedWidget->InitedForBlock == object && showedWidget->IsVisible())
			showedWidget->RequestClose();

		auto ref = showableWidgetDelegates.Find(object);
		if (ref && ref->IsValid())
			usableWithWidget->RemoveShowWidgetForBlockEventListener(*ref);
	}

	auto pickable = Cast<IPickableBlock>(object);
	if (pickable)
	{
		auto ref = pickableDelegates.Find(object);
		if (ref && ref->IsValid())
			pickable->RemovePickupItemEventListener(*ref);
	}

	auto electricityComp = Cast<UElectricityComponent>(object->GetComponentByClass(UElectricityComponent::StaticClass()));
	if (electricityComp && electricityComp->Network && electricityComp->Network->IsValidLowLevel())
		electricityComponent->RemoveFromWorldNetwork(electricityComp);

	auto count = UsedBlocks.Remove(object->BlockInfo);
	check(count == 1 && "Failed to remove block info.");
	object->Destroy();

	if (debugBoxesShown) {
		DEBUGHideMinMaxBoxes();
		DEBUGShowMinMaxBoxes();
	}

	TArray<UPatternGroupInfo*> refreshPatterns;
	ABlock* toRenew;
	while (patternRecompute.Dequeue(toRenew))
	{
		toRenew->RenewPatternInfo();
		if (toRenew->WorldObjectComponent->PatternGroupInfo)
		{
			RootBox->RegisterWatchingGroup(toRenew->WorldObjectComponent->PatternGroupInfo);
			refreshPatterns.AddUnique(toRenew->WorldObjectComponent->PatternGroupInfo);
		}
	}

	for (auto rp : refreshPatterns)
	{
		if (rp && rp->IsValidLowLevel() && !rp->IsPendingKill())
			rp->WatchingRegionChanged();
	}

	refreshPatterns.Empty();

	DEBUG_observable();

	weatherComponent->ObjectsChanged();
	check(RootBox);
	check(RootBox->IsValidLowLevel());
	check(!RootBox->IsPendingKill());

	return true;
}

ABlock* AWorldController::SpawnWorldObject(UWorld* world, UBlockInfo* block, TArray<FText>& validationErrors, bool addToRoot)
{
	if (!block)
		return nullptr;

	check(RootBox != nullptr);

	auto inst = Cast<UTCF2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ensure(inst);
	auto blockHolder = Cast<UBlockHolder>(inst->BlockHolder);
	ensure(blockHolder);

	auto definition = blockHolder->GetDefinitionFor(block->ID);

	if (!definition)
	{
		validationErrors.Add(FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.WorldController.UnknownID", "Neznámé ID ({0}). Blok nebude ve hře použit."), block->ID));
		return nullptr;
	}

	UMinMaxBox* box = WorldHelpers::GetSpawnBox(definition, block);
	ensure(box != nullptr);
	if (!IsValidSpawnPoint(box))
	{
		validationErrors.Add(FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.WorldController.InvalidSpawnPoint", "Blok s ID {0} nelze korektně přidat. Blok nebude ve hře použit."), block->ID));
		return nullptr;
	}

	FString invalidReason;
	bool isValid = BlockHelpers::CheckBlockValidity(definition, block, invalidReason);
	if (!isValid)
	{
		validationErrors.Add(FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.WorldController.InvalidBlock", "Blok s ID {0} není validní. Důvod: {1}. Blok nebude ve hře použit."), FText::FromString(FString::FormatAsNumber(block->ID)), FText::FromString(invalidReason)));
		return nullptr;
	}

	auto classBP = blockHolder->AviableBlocks[block->ID];

	ensure(classBP != nullptr);

	auto trans = BlockHelpers::GetSpawnTransform(definition, block);
	auto actor = world->SpawnActorDeferred<ABlock>(classBP, trans, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	if (!actor)
	{
		validationErrors.Add(FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.WorldController.InvalidActor", "Blok s ID {0} nemohl být korektně vytvořen. Blok nebude ve hře použit."), block->ID));
		return nullptr;
	}

	actor->SetBlockInfo(block);

	if (addToRoot) {
		auto MinMax = NewObject<UKDTree>()->Init(box);
		MinMax->ContainingObject = actor;

		auto woc = actor->WorldObjectComponent;
		woc->UpdateDefiningBox(MinMax);
		RootBox->AddToTree(MinMax);
		for (auto usedBox : woc->TreeElements)
		{
			check(usedBox->GetRootNode<UKDTree>() == RootBox && TEXT("Used box don't have RootBox as ROOT !"));
			check(usedBox->ContainingObject == actor && TEXT("Used box has another ContainingObject than it should have!"));
		}

		auto weatherMinMax = NewObject<UWeatherTargetsKDTree>()->Init(box);
		weatherMinMax->ContainingObject = actor;
		weatherComponent->WeatherRootTree->AddToTree(weatherMinMax);

		actor->InitWorldObjectComponent();

		auto pickable = Cast<IPickableBlock>(actor);
		if (pickable)
		{
			FPickupItemDelegate Subscriber;
			Subscriber.BindUObject(this, &AWorldController::onPickupItem);
			auto ListeningHandle = pickable->AddPickupItemEventListener(Subscriber);

			pickableDelegates.Add(actor, ListeningHandle);
		}

		auto usableWithWidget = Cast<IBlockWithShowableWidget>(actor);
		if (usableWithWidget)
		{
			FShowWidgetForBlockDelegate Subscriber;
			Subscriber.BindUObject(this, &AWorldController::onShowWidgetRequest);
			auto ListeningHandle = usableWithWidget->AddShowWidgetForBlockEventListener(Subscriber);

			showableWidgetDelegates.Add(actor, ListeningHandle);
		}

		actor->OnDestroyRequestedEvent.AddDynamic(this, &AWorldController::DestroyRequestEventHandler);

		//MinMax->DEBUGDrawContainingBox(GetWorld());
	}

	UGameplayStatics::FinishSpawningActor(actor, trans);

	if (addToRoot)
	{
		if (actor->BlockInfo->HealthSeverity == EHealthSeverity::Dead)
		{
			actor->Destroy();
			return NULL;
		}

		UsedBlocks.Add(block);

		if (debugBoxesShown) {
			DEBUGHideMinMaxBoxes();
			DEBUGShowMinMaxBoxes();
		}

		weatherComponent->ObjectsChanged();

		auto pi = actor->WorldObjectComponent->PatternGroupInfo;
		if (definition->UsingInPatterns && pi)
		{
			RootBox->RegisterWatchingGroup(pi);
			pi->WatchingRegionChanged();

			DEBUG_observable();
		}

		if (definition->HasElectricityComponent) {
			auto electricityComp = Cast<UElectricityComponent>(actor->GetComponentByClass(UElectricityComponent::StaticClass()));
			if (electricityComp)
				electricityComponent->AddToWorldNetwork(electricityComp);
		}
	}

	return actor;
}

void AWorldController::DEBUGShowMinMaxBoxes() {
	if (RootBox && !debugBoxesShown)
	{
		debugBoxesShown = true;
		RootBox->DEBUGDrawContainingBox(GetWorld());
	}
}

void AWorldController::DEBUGHideMinMaxBoxes() {
	if (RootBox && debugBoxesShown)
	{
		FlushPersistentDebugLines(GetWorld());
		debugBoxesShown = false;
	}

	DEBUG_observable();
}

void AWorldController::BeginPlay() {
	RootBox = NewObject<UKDTree>(GetTransientPackage(), TEXT("RootBox"));

	auto minCube = WorldHelpers::BorderToWorld(FVector(0, 0, 0));
	auto maxCube = WorldHelpers::BorderToWorld(GameDefinitions::WorldBorders);

	FVector min((minCube - 0.5 * FVector(1, 1, 1))* GameDefinitions::CubeMinSize);
	FVector max((maxCube + 0.5 * FVector(1, 1, 1))* GameDefinitions::CubeMinSize);

	RootBox->Init(min, max, 0);

	for (TActorIterator<APawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (!weatherComponent)
			weatherComponent = Cast<UGameWeatherComponent>((*ActorItr)->GetComponentByClass(UGameWeatherComponent::StaticClass()));

		if (!electricityComponent)
			electricityComponent = Cast<UGameElectricityComponent>((*ActorItr)->GetComponentByClass(UGameElectricityComponent::StaticClass()));

		if (weatherComponent && electricityComponent)
			break;
	}

	check(weatherComponent);
	check(electricityComponent);

	weatherComponent->WeatherRootTree->Init(min, max, 0);
	weatherComponent->InitComp();

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

	for (auto del : showableWidgetDelegates)
	{
		if (del.Key && del.Key->IsValidLowLevel() && del.Value.IsValid())
		{
			auto showable = Cast<IBlockWithShowableWidget>(del.Key);
			if (showable)
				showable->RemoveShowWidgetForBlockEventListener(del.Value);
		}
	}

	Super::EndPlay(EndPlayReasonType);
}

void AWorldController::LoadDataFromCarrier(USaveGameCarrier* carrier)
{
	check(carrier != nullptr);

	auto usedBlocks = BlockSavingHelpers::GetBlockData(carrier);

	loadBlocksArray(usedBlocks);
}

void AWorldController::SaveDataToCarrier(USaveGameCarrier* carrier)
{
	check(carrier != nullptr);

	TArray<UBlockInfo*> toSave;

	for (auto bl : UsedBlocks)
	{
		if (bl->Health <= 0 || bl->HealthSeverity == EHealthSeverity::Dead)
			continue;

		toSave.Add(bl);
	}

	BlockSavingHelpers::SetBlockData(carrier, toSave);
}

void AWorldController::onPickupItem(ABlock* pickingItem)
{
	auto playerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!playerChar || !playerChar->IsValidLowLevel())
		return;

	auto invComp = Cast<UInventoryComponent>(playerChar->GetComponentByClass(UInventoryComponent::StaticClass()));
	if (!invComp || !invComp->IsValidLowLevel())
		return;

	auto invBuildable = UInventoryBuildableBlockInfo::GetInventoryBuildable(pickingItem->BlockInfo, pickingItem->Definition.GetDefaultObject());

	invBuildable->SetTagsFlatlined(pickingItem->GetInventoryBuildableTags());

	if (DestroyWorldObject(pickingItem))
		invComp->AddItem(invBuildable);
}

void AWorldController::onShowWidgetRequest(ABlock* block, TSubclassOf<UUserWidget> widget)
{
	if (!widget || !block)
		return;

	auto pc = Cast<ATauCetiF2PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	ensure(pc);

	if (widget->IsChildOf(UObjectWidget::StaticClass()))
	{
		TSubclassOf<UObjectWidget> objW = *widget;
		auto id = pc->EnsureRegisterWidget(objW);
		pc->ShowRegisteredWidget(id, block);

		return;
	}

	auto defW = widget.GetDefaultObject();

	defW->AddToPlayerScreen();
}

void AWorldController::DestroyRequestEventHandler(ABlock* block)
{
	DestroyWorldObject(block);
}