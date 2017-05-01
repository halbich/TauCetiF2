#include "TauCetiF2.h"
#include "PlatformFeatures.h"
#include "GameFramework/SaveGame.h"
#include "FFileVisitor.h"
#include "Helpers.h"

bool UHelpers::ChangeLocalization(FString target)
{
	return FInternationalization::Get().SetCurrentCulture(target);
}

void UHelpers::GetCurrentLocalizations(UPARAM(ref)TArray<FString>& DisplayNames, UPARAM(ref)TArray<FString>& IsoNames)
{
	TArray<FCultureRef> LocalizedCulturesForGame;

	FInternationalization::Get().GetCulturesWithAvailableLocalization(FPaths::GetGameLocalizationPaths(), LocalizedCulturesForGame, false);

	for (auto c : LocalizedCulturesForGame)
	{
		DisplayNames.Add(c->GetNativeName());
		IsoNames.Add(c->GetTwoLetterISOLanguageName());
	}
}

FString UHelpers::GetCurrentCultureIsoName()
{
	return  FInternationalization::Get().GetCurrentCulture()->GetTwoLetterISOLanguageName();
}

void UHelpers::FatalError(const FName text)
{
	UE_LOG(LogTemp, Fatal, TEXT("%s"), *text.ToString());
}

int32 UHelpers::GetCubeMinSize()
{
	return GameDefinitions::CubeMinSize;
}

int32 UHelpers::GetCubeMinSizeHalf()
{
	return GameDefinitions::CubeMinSizeHalf;
}

void UHelpers::ObjectApplyLocalTrans(UStaticMeshComponent* comp, FVector loc, FRotator rot, FVector scal) {
	comp->ResetRelativeTransform();

	auto rc = comp->GetRelativeTransform();

	rc.SetLocation(loc);
	rc.SetRotation(FQuat(rot));
	rc.SetScale3D(scal);

	comp->SetRelativeTransform(rc);
}

TArray<UInventoryFlagItem*> UHelpers::GetBlockFlags(UObject* WorldContextObject, UBlockBaseInfo* blockBaseInfo)
{
	TArray<UInventoryFlagItem*> result;

	if (!blockBaseInfo || blockBaseInfo->ID < 0)
		return result; // system actions

	auto inst = Cast<UTCF2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	ensure(inst);
	auto blockHolder = Cast<UBlockHolder>(inst->BlockHolder);
	ensure(blockHolder);

	auto def = blockHolder->GetDefinitionFor(blockBaseInfo->ID);
	ensure(def);

	for (auto fl : def->AdditionalFlags)
	{
		auto invItem = NewObject<UInventoryFlagItem>();
		invItem->TagName = fl.TagID;
		invItem->DisplayText = fl.DisplayText;

		for (auto it : fl.PossibleValues)
		{
			auto cmbIt = NewObject<UCmbItem>();
			cmbIt->Value = it.Value;
			cmbIt->Text = it.DisplayText;
			invItem->AviableValues.Add(cmbIt);
		}

		result.Add(invItem);
		if (blockBaseInfo->AdditionalFlags.Contains(fl.TagID))
		{
			invItem->TagValue = blockBaseInfo->AdditionalFlags[fl.TagID];
			invItem->TagReadOnly = true;
		}
	}
	return result;
}

void UHelpers::GetSliderValuesForObject(UBuildableBlockInfo* buildableInfo, FVector Limit, bool IsSaveEnabled, FVector& Min, FVector& Max)
{
	auto bd = buildableInfo->BlockDefinition;
	if (!IsSaveEnabled)
	{
		auto actualValue = bd->GetObjectScale(buildableInfo->Scale);
		Min = actualValue;
		Max = actualValue;
		return;
	}

	auto sM = bd->HasCustomScaling ? bd->CustomBlockScale : bd->MaxBlockScale;

	Max = FVector(FMath::Min(sM.X, Limit.X), FMath::Min(sM.Y, Limit.Y), FMath::Min(sM.Z, Limit.Z));

	auto sm = bd->HasCustomScaling ? bd->CustomBlockScale : bd->MinBlockScale;
	Min = FVector(FMath::Min(sm.X, Max.X), FMath::Min(sm.Y, Max.Y), FMath::Min(sm.Z, Max.Z));
}

FDateTime UHelpers::GetTimeFromSeconds(float seconds)
{
	int32 sec = FMath::RoundToInt(seconds);

	auto days = sec / 86400;
	auto hoursRest = sec % 86400;

	auto hours = hoursRest / 3600;
	auto minutesRest = hoursRest % 3600;

	auto minutes = minutesRest / 60;
	auto secondsRest = minutesRest % 60;

	check(FDateTime::Validate(1, 1, 1, hours, minutes, secondsRest, 0));

	return FDateTime(1, 1, 1, hours, minutes, secondsRest, 0);
}

bool UHelpers::GetSoundClassVolume(USoundClass* TargetClass, float& resultValue)
{
	resultValue = -1;

	if (!TargetClass || !TargetClass->IsValidLowLevel())
		return false;

	resultValue = TargetClass->Properties.Volume;
	return true;
}

bool UHelpers::SetSoundClassVolume(USoundClass* TargetClass, float targetValue)
{
	if (!TargetClass || !TargetClass->IsValidLowLevel())
		return false;

	TargetClass->Properties.Volume = targetValue;
	return true;
}

FWeatherDefinition UHelpers::GetRandomDefinition(UPARAM(ref)TArray<FWeatherDefinition>& aviableDefinitions, int32& outIndex)
{
	check(aviableDefinitions.Num() > 0);

	outIndex = FMath::RandHelper(aviableDefinitions.Num());

	return aviableDefinitions[outIndex];
}

float UHelpers::GetGameDayLength()
{
	return GameDefinitions::GameDayLength;
}

float UHelpers::GetGameDayMultiplier()
{
	return GameDefinitions::GameDayMultiplier;
}

TArray<FControlsHelperStructGroup> UHelpers::GetControlsList()
{
	TArray<FControlsHelperStructGroup> res;

	FControlsHelperStructGroup main;
	main.SectionName = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.Desc", "Ovládání");
	
	FControlsHelperStruct mainMove;
	mainMove.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.MoveP", "W S A D");
	mainMove.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.MoveS", "Nahoru dolů vlevo vpravo");
	mainMove.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.MoveD", "Pohyb postavy");
	main.Controls.Add(mainMove);


	res.Add(main);

	FControlsHelperStructGroup building;
	building.SectionName = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.Desc", "Stavění");

	FControlsHelperStruct buildingChange;
	buildingChange.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.BuildingChangeP", "Kolečko myši");
	buildingChange.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.BuildingChangeS", "");
	buildingChange.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.BuildingChangeD", "Změnit stavitelný / umístitelný objekt");
	building.Controls.Add(buildingChange);

	res.Add(building);

	return res;
}