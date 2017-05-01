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

	FControlsHelperStructGroup Main;
	Main.SectionName = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.Desc", "Ovládání");

	/*FControlsHelperStruct mainMove;
	mainMove.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.MoveP", "W S A D");
	mainMove.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.MoveS", "Nahoru dolů vlevo vpravo");
	mainMove.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.MoveD", "Pohyb postavy");
	Main.Controls.Add(mainMove);*/


	FControlsHelperStruct MainMove; MainMove.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.Move.P", "W S A D"); MainMove.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.Move.S", "Šipky klávesnice"); MainMove.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.Move.D", "Pohyb postavy"); Main.Controls.Add(MainMove);
	FControlsHelperStruct MainJump; MainJump.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.Jump.P", "Mezerník"); MainJump.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.Jump.S", ""); MainJump.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.Jump.D", "Výskok"); Main.Controls.Add(MainJump);
	FControlsHelperStruct MainLooking; MainLooking.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.Looking.P", "Myš"); MainLooking.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.Looking.S", ""); MainLooking.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.Looking.D", "Rozhlížení se"); Main.Controls.Add(MainLooking);
	FControlsHelperStruct MainSwitch; MainSwitch.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.Switch.P", "Tabulátor"); MainSwitch.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.Switch.S", ""); MainSwitch.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.Switch.D", "Přepnout pohled"); Main.Controls.Add(MainSwitch);
	FControlsHelperStruct MainFastUse; MainFastUse.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.FastUse.P", "Levé tlačítko myši"); MainFastUse.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.FastUse.S", ""); MainFastUse.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.FastUse.D", "Rychlé použití"); Main.Controls.Add(MainFastUse);
	FControlsHelperStruct MainEditPickup; MainEditPickup.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.EditPickup.P", "Pravé tlačítko myši"); MainEditPickup.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.EditPickup.S", "F"); MainEditPickup.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.EditPickup.D", "Sebrat / otevřít konfigurační terminál"); Main.Controls.Add(MainEditPickup);
	FControlsHelperStruct MainInventory; MainInventory.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.Inventory.P", "E"); MainInventory.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.Inventory.S", ""); MainInventory.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Main.Inventory.D", "Otevřít inventář"); Main.Controls.Add(MainInventory);

	res.Add(Main);



	FControlsHelperStructGroup Building;
	Building.SectionName = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.Desc", "Stavění");

	FControlsHelperStruct BuildingBuildItem; BuildingBuildItem.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.BuildItem.P", "Levé tlačítko myši"); BuildingBuildItem.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.BuildItem.S", ""); BuildingBuildItem.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.BuildItem.D", "Postavit / umístit blok"); Building.Controls.Add(BuildingBuildItem);
	FControlsHelperStruct BuildingEmptyHand; BuildingEmptyHand.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.EmptyHand.P", "H"); BuildingEmptyHand.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.EmptyHand.S", ""); BuildingEmptyHand.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.EmptyHand.D", "Prázdné ruce"); Building.Controls.Add(BuildingEmptyHand);
	FControlsHelperStruct BuildingNextItem; BuildingNextItem.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.NextItem.P", "Kolečko myši dolů"); BuildingNextItem.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.NextItem.S", ""); BuildingNextItem.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.NextItem.D", "Následující stavitelný / umístitelný blok"); Building.Controls.Add(BuildingNextItem);
	FControlsHelperStruct BuildingPrevItem; BuildingPrevItem.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.PrevItem.P", "Kolečko myši nahoru"); BuildingPrevItem.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.PrevItem.S", ""); BuildingPrevItem.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.PrevItem.D", "Předchozí stavitelný / umístitelný blok"); Building.Controls.Add(BuildingPrevItem);
	FControlsHelperStruct BuildingNextBank; BuildingNextBank.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.NextBank.P", ")"); BuildingNextBank.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.NextBank.S", ""); BuildingNextBank.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.NextBank.D", "Následující stavební bank"); Building.Controls.Add(BuildingNextBank);
	FControlsHelperStruct BuildingPrevBank; BuildingPrevBank.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.PrevBank.P", "ú"); BuildingPrevBank.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.PrevBank.S", ""); BuildingPrevBank.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.PrevBank.D", "Předchozí stavební bank"); Building.Controls.Add(BuildingPrevBank);
	FControlsHelperStruct BuildingRotYaw; BuildingRotYaw.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.RotYaw.P", "Delete, Page Down"); BuildingRotYaw.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.RotYaw.S", "4, 6"); BuildingRotYaw.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.RotYaw.D", "Rotovat blok (yaw)"); Building.Controls.Add(BuildingRotYaw);
	FControlsHelperStruct BuildingRotPitch; BuildingRotPitch.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.RotPitch.P", " Home, End"); BuildingRotPitch.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.RotPitch.S", "8, 5"); BuildingRotPitch.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.RotPitch.D", "Rotovat blok (pitch)"); Building.Controls.Add(BuildingRotPitch);
	FControlsHelperStruct BuildingRotRoll; BuildingRotRoll.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.RotRoll.P", "Insert, Page Up"); BuildingRotRoll.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.RotRoll.S", "7, 9"); BuildingRotRoll.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.RotRoll.D", "Rotovat blok (roll)"); Building.Controls.Add(BuildingRotRoll);
	FControlsHelperStruct BuildingShift; BuildingShift.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.Shift.P", "Shift"); BuildingShift.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.Shift.S", ""); BuildingShift.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Building.Shift.D", "Přepnout zobrazení pomocné konstrukční roviny"); Building.Controls.Add(BuildingShift);
	res.Add(Building);


	FControlsHelperStructGroup Other;
	Other.SectionName = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Other.Desc", "Ostatní");

	FControlsHelperStruct OtherCreative; OtherCreative.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Other.Creative.P", "C"); OtherCreative.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Other.Creative.S", ""); OtherCreative.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Other.Creative.D", "Přepnout kreativní mód"); Other.Controls.Add(OtherCreative);
	FControlsHelperStruct OtherQuickSave; OtherQuickSave.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Other.QuickSave.P", "F8"); OtherQuickSave.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Other.QuickSave.S", "K"); OtherQuickSave.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Other.QuickSave.D", "Rychlé uložení hry"); Other.Controls.Add(OtherQuickSave);
	FControlsHelperStruct OtherQuickLoad; OtherQuickLoad.PrimaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Other.QuickLoad.P", "F9"); OtherQuickLoad.SecondaryControl = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Other.QuickLoad.S", "L"); OtherQuickLoad.Description = NSLOCTEXT("TCF2LocSpace", "LC.Controls.Other.QuickLoad.D", "Načíst poslední rychlé uložení"); Other.Controls.Add(OtherQuickLoad);

	res.Add(Other);


	return res;
}