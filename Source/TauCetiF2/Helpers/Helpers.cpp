﻿#include "TauCetiF2.h"
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

TArray<UInventoryFlagItem*> UHelpers::GetBlockFlags(UBlockBaseInfo* blockBaseInfo)
{
	TArray<UInventoryFlagItem*> result;

	if (!blockBaseInfo || blockBaseInfo->ID < 0)
		return result; // system actions

	auto def = UBlockHolderComponent::GetInstance()->GetDefinitionFor(blockBaseInfo->ID);
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

void UHelpers::GetSliderValuesForObject(UBuildableBlockInfo* buildableInfo, bool IsSaveEnabled, FVector& Min, FVector& Max)
{
	auto bd = buildableInfo->BlockDefinition;
	if (!IsSaveEnabled)
	{
		auto actualValue = bd->GetObjectScale(buildableInfo->Scale);
		Min = actualValue;
		Max = actualValue;
		return;
	}

	Min = bd->HasCustomScaling ? bd->CustomBlockScale : bd->MinBlockScale;
	Max = bd->HasCustomScaling ? bd->CustomBlockScale : bd->MaxBlockScale;
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