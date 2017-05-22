#include "GameSave.h"
#include "NewGameSaveHolder.h"

UNewGameSaveHolder::UNewGameSaveHolder()
{
	systemSaves = TMap<ENamedHardcodedLevel, FunctionPtrType>();

	systemSaves.Add(ENamedHardcodedLevel::MainMenu, &UNewGameSaveHolder::getMainMenuSave);
	systemSaves.Add(ENamedHardcodedLevel::TutorialLevel, &UNewGameSaveHolder::getTutorialGameSave);
	systemSaves.Add(ENamedHardcodedLevel::DefaultLevel, &UNewGameSaveHolder::getDefaultGameSave);
	systemSaves.Add(ENamedHardcodedLevel::MiniLevel, &UNewGameSaveHolder::getMiniLevelGameSave);
	systemSaves.Add(ENamedHardcodedLevel::SuperSize, &UNewGameSaveHolder::getSuperSizeGameSave);
	systemSaves.Add(ENamedHardcodedLevel::EmptyLevel, &UNewGameSaveHolder::getEmptyGameSave);
}

UNewGameSaveHolder::~UNewGameSaveHolder()
{
	systemSaves.Empty();
}

UNewGameSaveHolder* UNewGameSaveHolder::Instance()
{
	return NewObject<UNewGameSaveHolder>();
}

TArray<USaveGameCarrier*> UNewGameSaveHolder::GetNewSaveGamesList()
{
	TArray<USaveGameCarrier*> result;
	auto i = Instance();

	for (auto level : systemSaves)
	{
		if (level.Key != ENamedHardcodedLevel::MainMenu)
			result.Add((i->* (level.Value))(false));
	}

	return result;
}

USaveGameCarrier* UNewGameSaveHolder::GetSaveForMainMenu()
{
	auto i = Instance();
	auto fn = i->systemSaves[ENamedHardcodedLevel::MainMenu];

	return (i->* (fn))(false);
}

USaveGameCarrier* UNewGameSaveHolder::getEmptyGameSave(bool full)
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemEmpty", "Prázdná hra").ToString();
	c->SaveFullDescription = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemEmptyFullDescr", "Pokud se chceš jen tak proběhnout po planetě, tak začni tuto hru. Ale upozorňuji tě, že tam dlouho nepřežiješ.");
	c->FullFilePath = TEXT("_system_empty");
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::EmptyLevel;

	if (!full)
		return c;

	c->CurrentTime = FMath::RandHelper(86400);
	c->PlayerUseFPSCamera = true;
	c->PlayerHealth = 1000;
	c->PlayerOxygenComponent.CurrentObjectOxygen = 1000.0f;
	c->PlayerElectricityComponent.CurrentObjectEnergy = 500000.0f;

	c->inventoryTags = makeDefault();

	auto UsedBlocks = &c->usedBlocks;

	c->PlayerPosition = FVector(0, 0, 90);

	return c;
}

USaveGameCarrier* UNewGameSaveHolder::GetSaveByPath(FString path)
{
	if (path.IsEmpty())
		return NULL;

	if (path.StartsWith(TEXT("_system_")))
	{
		for (auto level : systemSaves)
		{
			auto lvl = (this->* (level.Value))(false);
			if (lvl->FullFilePath == path)
				return (this->* (level.Value))(true);
		}
		checkNoEntry();
	}

	TArray<FText> errorList;
	auto currentSaves = USaveGameCarrier::GetSaveGameInfoList(errorList);

	for (auto save : currentSaves)
	{
		if (save->FullFilePath == path)
			return save;
	}

	return NULL;
}

#pragma region Default level

USaveGameCarrier* UNewGameSaveHolder::getDefaultGameSave(bool full)
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemDefault", "Výchozí hra").ToString();
	c->SaveFullDescription = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemDefaultFullDescr", "Začni kolonizovat planetu co nejdříve! Středně zásob do začátku a středně velká základna dávají dobrý začátek pro další rozvoj.");
	c->FullFilePath = TEXT("_system_default");
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::DefaultLevel;

	if (!full)
		return c;

	c->CurrentTime = FMath::RandHelper(86400);
	c->PlayerUseFPSCamera = true;
	c->PlayerHealth = 1000;
	c->PlayerOxygenComponent.CurrentObjectOxygen = 1000.0f;
	c->PlayerElectricityComponent.CurrentObjectEnergy = 500000.0f;

	c->inventoryTags = makeDefault();

	c->buildableBlocks.Add(makeBuildable(DeleteID, FVector(1, 1, 1)));
	c->buildableBlocks.Add(makeBuildable(TerminalID, FVector(1, 8, 5)));

	c->buildableBlocks.Add(makeBuildable(OxygenTankID, FVector(2, 2, 2)));
	c->buildableBlocks.Add(makeBuildable(SwitcherID, FVector(1, 1, 1)));
	c->buildableBlocks.Add(makeBuildable(LightSmallID, FVector(1, 1, 1)));
	c->buildableBlocks.Add(makeBuildable(CreatorID, FVector(3, 3, 2)));

	auto UsedBlocks = &c->usedBlocks;

	//auto b_0 = make1(0, -6, -2, 1, 20, 20, 4, 0, 0, 0, 16000.000000, ""); mE(b_0, 0.000000); UsedBlocks->Add(b_0);
	//auto b_1 = make1(0, 14, -2, 1, 20, 20, 4, 0, 0, 0, 16000.000000, ""); mE(b_1, 0.000000); UsedBlocks->Add(b_1);
	//auto b_2 = make1(0, 14, -22, 1, 20, 20, 4, 0, 0, 0, 16000.000000, ""); mE(b_2, 0.000000); UsedBlocks->Add(b_2);
	//auto b_3 = make1(0, -6, -22, 1, 20, 20, 4, 0, 0, 0, 16000.000000, ""); mE(b_3, 0.000000); UsedBlocks->Add(b_3);
	//auto b_4 = make1(1, -15, -30, 9, 20, 1, 11, 0, 90, 0, 2200.000000, ""); mE(b_4, 0.000000); UsedBlocks->Add(b_4);
	//auto b_5 = make1(1, 24, -21, 6, 20, 1, 6, 0, -90, 0, 1200.000000, ""); mE(b_5, 0.000000); UsedBlocks->Add(b_5);
	//auto b_6 = make1(1, 24, -1, 6, 20, 1, 6, 0, -90, 0, 1200.000000, ""); mE(b_6, 0.000000); UsedBlocks->Add(b_6);
	//auto b_7 = make1(1, 14, -31, 6, 20, 1, 6, 0, 180, 0, 1200.000000, ""); mE(b_7, 0.000000); UsedBlocks->Add(b_7);
	//auto b_8 = make1(1, -15, -1, 6, 20, 1, 6, 0, -90, 0, 1200.000000, ""); mE(b_8, 0.000000); UsedBlocks->Add(b_8);
	//auto b_9 = make1(600, -12, -39, 9, 6, 1, 11, 0, 0, 0, 660.000000, ""); mE(b_9, 26400.000000); UsedBlocks->Add(b_9);
	//auto b_10 = make1(100, 10, -35, 1, 8, 14, 4, 0, -90, 0, 1493.333374, ""); mE(b_10, 0.000000); UsedBlocks->Add(b_10);
	//auto b_11 = make1(0, -6, -35, 1, 19, 8, 4, 0, 180, 0, 6080.000000, ""); mE(b_11, 0.000000); UsedBlocks->Add(b_11);
	//auto b_12 = make1(700, 0, -35, 9, 7, 7, 11, 0, 90, 0, 10.000000, ""); af(b_12, TEXT("DoorOpening"), 0); bd(b_12, TEXT("DoorState"), TEXT("0")); bd(b_12, TEXT("DoorYaw"), TEXT("0.0")); mE(b_12, 0.000000); UsedBlocks->Add(b_12);
	//auto b_13 = make1(1, 3, -39, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_13, 0.000000); UsedBlocks->Add(b_13);
	//auto b_14 = make1(600, 0, -39, 9, 6, 1, 11, 0, -180, 0, 660.000000, ""); mE(b_14, 26400.000000); UsedBlocks->Add(b_14);
	//auto b_15 = make1(600, -6, -39, 9, 5, 1, 11, 0, 0, 0, 550.000000, ""); mE(b_15, 22000.000000); UsedBlocks->Add(b_15);
	//auto b_16 = make1(2, 4, -27, 4, 2, 7, 1, 0, 0, 0, 140.000000, ""); UsedBlocks->Add(b_16);
	//auto b_17 = make1(2, 6, -27, 5, 2, 7, 1, 0, 0, 0, 140.000000, ""); UsedBlocks->Add(b_17);
	//auto b_18 = make1(2, 8, -27, 6, 2, 7, 1, 0, 0, 0, 140.000000, ""); UsedBlocks->Add(b_18);
	//auto b_19 = make1(2, 10, -27, 7, 2, 7, 1, 0, 0, 0, 140.000000, ""); UsedBlocks->Add(b_19);
	//auto b_20 = make1(2, 12, -27, 8, 2, 7, 1, 0, 0, 0, 140.000000, ""); UsedBlocks->Add(b_20);
	//auto b_21 = make1(2, 14, -27, 9, 2, 7, 1, 0, 0, 0, 140.000000, ""); UsedBlocks->Add(b_21);
	//auto b_22 = make1(1, 21, -31, 13, 8, 7, 1, 90, 0, 90, 560.000000, ""); mE(b_22, 0.000000); UsedBlocks->Add(b_22);
	//auto b_23 = make1(1, 14, -31, 13, 8, 7, 1, 90, 0, 90, 560.000000, ""); mE(b_23, 0.000000); UsedBlocks->Add(b_23);
	//auto b_24 = make1(1, 24, -27, 13, 8, 7, 1, 90, 0, -180, 560.000000, ""); mE(b_24, 0.000000); UsedBlocks->Add(b_24);
	//auto b_25 = make1(1, 24, -20, 13, 8, 7, 1, 90, 0, -180, 560.000000, ""); mE(b_25, 0.000000); UsedBlocks->Add(b_25);
	//auto b_26 = make1(2, 16, -27, 10, 2, 7, 1, 0, 0, 0, 140.000000, ""); UsedBlocks->Add(b_26);
	//auto b_27 = make1(1, 20, -27, 10, 6, 6, 1, 0, -90, 0, 360.000000, ""); mE(b_27, 0.000000); UsedBlocks->Add(b_27);
	//auto b_28 = make1(2, 20, -23, 11, 2, 7, 1, 0, -90, 0, 140.000000, ""); UsedBlocks->Add(b_28);
	//auto b_29 = make1(2, 20, -21, 12, 2, 7, 1, 0, -90, 0, 140.000000, ""); UsedBlocks->Add(b_29);
	//auto b_30 = make1(2, 20, -19, 13, 2, 7, 1, 0, -90, 0, 140.000000, ""); UsedBlocks->Add(b_30);
	//auto b_31 = make1(2, 20, -17, 14, 2, 7, 1, 0, -90, 0, 140.000000, ""); UsedBlocks->Add(b_31);
	//auto b_32 = make1(1, 24, -13, 14, 8, 7, 1, -90, 0, 0, 560.000000, ""); mE(b_32, 0.000000); UsedBlocks->Add(b_32);
	//auto b_33 = make1(2, 20, -15, 15, 2, 7, 1, 0, -90, 0, 140.000000, ""); UsedBlocks->Add(b_33);
	//auto b_34 = make1(2, 20, -13, 16, 2, 7, 1, 0, -90, 0, 140.000000, ""); UsedBlocks->Add(b_34);
	//auto b_35 = make1(2, 20, -11, 17, 2, 7, 1, 0, -90, 0, 140.000000, ""); UsedBlocks->Add(b_35);
	//auto b_36 = make1(1, 3, -31, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_36, 0.000000); UsedBlocks->Add(b_36);
	//auto b_37 = make1(600, 24, -7, 11, 5, 1, 4, 0, 90, 0, 200.000000, ""); mE(b_37, 8000.000000); UsedBlocks->Add(b_37);
	//auto b_38 = make1(600, 24, -7, 15, 5, 1, 4, 0, 90, 0, 200.000000, ""); mE(b_38, 8000.000000); UsedBlocks->Add(b_38);
	//auto b_39 = make1(600, 24, -2, 11, 5, 1, 4, 0, 90, 0, 200.000000, ""); mE(b_39, 8000.000000); UsedBlocks->Add(b_39);
	//auto b_40 = make1(600, 24, -2, 15, 5, 1, 4, 0, 90, 0, 200.000000, ""); mE(b_40, 8000.000000); UsedBlocks->Add(b_40);
	//auto b_41 = make1(600, 24, 3, 11, 5, 1, 4, 0, 90, 0, 200.000000, ""); mE(b_41, 8000.000000); UsedBlocks->Add(b_41);
	//auto b_42 = make1(600, 24, 3, 15, 5, 1, 4, 0, 90, 0, 200.000000, ""); mE(b_42, 8000.000000); UsedBlocks->Add(b_42);
	//auto b_43 = make1(1150, -12, 2, 4, 3, 3, 2, 0, 0, 0, 180.000000, ""); mE(b_43, 18000.000000); UsedBlocks->Add(b_43);
	//auto b_44 = make1(1150, -12, -1, 4, 3, 3, 2, 0, 0, 0, 180.000000, ""); mE(b_44, 18000.000000); UsedBlocks->Add(b_44);
	//auto b_45 = make1(1150, -9, 2, 4, 3, 3, 2, 0, 0, 0, 180.000000, ""); mE(b_45, 18000.000000); UsedBlocks->Add(b_45);
	//auto b_46 = make1(1150, -9, -1, 4, 3, 3, 2, 0, 0, 0, 180.000000, ""); mE(b_46, 18000.000000); UsedBlocks->Add(b_46);
	//auto b_47 = make1(1200, 22, -29, 5, 3, 3, 3, 0, -90, 0, 10.000000, ""); mE(b_47, 100000.000000); UsedBlocks->Add(b_47);
	//auto b_48 = make1(1200, 19, -29, 5, 3, 3, 3, 0, -90, 0, 10.000000, ""); mE(b_48, 100000.000000); UsedBlocks->Add(b_48);
	//auto b_49 = make1(1200, 16, -29, 5, 3, 3, 3, 0, -90, 0, 10.000000, ""); mE(b_49, 100000.000000); UsedBlocks->Add(b_49);
	//auto b_50 = make1(1200, 13, -29, 5, 3, 3, 3, 0, -90, 0, 10.000000, ""); mE(b_50, 100000.000000); UsedBlocks->Add(b_50);
	//auto b_51 = make1(1200, 22, -29, 8, 3, 3, 3, 0, -90, 0, 10.000000, ""); mE(b_51, 100000.000000); UsedBlocks->Add(b_51);
	//auto b_52 = make1(1200, 19, -29, 8, 3, 3, 3, 0, -90, 0, 10.000000, ""); mE(b_52, 100000.000000); UsedBlocks->Add(b_52);
	//auto b_53 = make1(1200, 22, -23, 5, 3, 3, 3, 0, -90, 0, 10.000000, ""); mE(b_53, 100000.000000); UsedBlocks->Add(b_53);
	//auto b_54 = make1(1200, 22, -23, 8, 3, 3, 3, 0, -90, 0, 10.000000, ""); mE(b_54, 100000.000000); UsedBlocks->Add(b_54);
	//auto b_55 = make1(1200, 22, -20, 5, 3, 3, 3, 0, -90, 0, 10.000000, ""); mE(b_55, 100000.000000); UsedBlocks->Add(b_55);
	//auto b_56 = make1(1200, 22, -20, 8, 3, 3, 3, 0, -90, 0, 10.000000, ""); mE(b_56, 100000.000000); UsedBlocks->Add(b_56);
	//auto b_57 = make1(1200, 22, -17, 5, 3, 3, 3, 0, -90, 0, 10.000000, ""); mE(b_57, 100000.000000); UsedBlocks->Add(b_57);
	//auto b_58 = make1(1200, 22, -17, 8, 3, 3, 3, 0, -90, 0, 10.000000, ""); mE(b_58, 100000.000000); UsedBlocks->Add(b_58);
	//auto b_59 = make1(1200, 22, -14, 5, 3, 3, 3, 0, 0, 0, 10.000000, ""); mE(b_59, 100000.000000); UsedBlocks->Add(b_59);
	//auto b_60 = make1(1200, 22, -14, 8, 3, 3, 3, 0, 0, 0, 10.000000, ""); mE(b_60, 100000.000000); UsedBlocks->Add(b_60);
	//auto b_61 = make1(1400, 22, -10, 5, 4, 3, 4, 0, 90, 0, 10.000000, "Plnicka"); bd(b_61, TEXT("CurrentFilling"), TEXT("0.0")); bd(b_61, TEXT("HasItem"), TEXT("0")); bd(b_61, TEXT("ItemTags"), TEXT("")); mE(b_61, 150.000000); mEP(b_61, 1, 1, 1.000000); mO(b_61, 150.000000);
	//mr(b_61, "A9F299C54AB0F2B6ABEBC280C23BF218"); mrc(b_61, "6EEA645949C55B8F90F421AB704204C1", 0); UsedBlocks->Add(b_61);
	//auto b_62 = make1(1050, 23, -11, 11, 1, 1, 1, 0, 90, 90, 10.000000, "Vypinac plnicka"); bd(b_62, TEXT("ReactsToDayCycle"), TEXT("1")); bd(b_62, TEXT("StateAtDay"), TEXT("1")); bd(b_62, TEXT("StateAtNight"), TEXT("0")); mE(b_62, 0.000000); mEP(b_62, 1, 1, 1.000000);
	//mr(b_62, "6EEA645949C55B8F90F421AB704204C1"); mrc(b_62, "A9F299C54AB0F2B6ABEBC280C23BF218", 1); UsedBlocks->Add(b_62);
	//auto b_63 = make1(1, -6, -35, 16, 20, 9, 3, 0, 0, 0, 5400.000000, ""); mE(b_63, 0.000000); UsedBlocks->Add(b_63);
	//auto b_64 = make1(101, -5, -42, 16, 20, 5, 3, 0, -180, 0, 1000.000122, ""); mE(b_64, 0.000000); UsedBlocks->Add(b_64);
	//auto b_65 = make1(101, 15, -42, 16, 20, 5, 3, 0, -180, 0, 1000.000122, ""); mE(b_65, 0.000000); UsedBlocks->Add(b_65);
	//auto b_66 = make1(101, 27, -29, 16, 20, 5, 3, 0, -90, 0, 1000.000122, ""); mE(b_66, 0.000000); UsedBlocks->Add(b_66);
	//auto b_67 = make1(101, -18, -30, 16, 20, 5, 3, 0, 90, 0, 1000.000122, ""); mE(b_67, 0.000000); UsedBlocks->Add(b_67);
	//auto b_68 = make1(101, -18, -10, 16, 20, 5, 3, 0, 90, 0, 1000.000122, ""); mE(b_68, 0.000000); UsedBlocks->Add(b_68);
	//auto b_69 = make1(101, -18, 10, 16, 20, 5, 3, 0, 90, 0, 1000.000122, ""); mE(b_69, 0.000000); UsedBlocks->Add(b_69);
	//auto b_70 = make1(201, -18, -42, 16, 5, 5, 3, 0, -180, 0, 375.000000, ""); mE(b_70, 0.000000); UsedBlocks->Add(b_70);
	//auto b_71 = make1(201, 27, -42, 16, 5, 5, 3, 0, -90, 0, 375.000000, ""); mE(b_71, 0.000000); UsedBlocks->Add(b_71);
	//auto b_72 = make1(201, 27, -17, 16, 5, 5, 3, 0, 0, 0, 375.000000, ""); mE(b_72, 0.000000); UsedBlocks->Add(b_72);
	//auto b_73 = make1(201, -18, 23, 16, 5, 5, 3, 0, 90, 0, 375.000000, ""); mE(b_73, 0.000000); UsedBlocks->Add(b_73);
	//auto b_74 = make1(101, -6, 23, 16, 20, 5, 3, 0, 0, 0, 1000.000122, ""); mE(b_74, 0.000000); UsedBlocks->Add(b_74);
	//auto b_75 = make1(101, 14, 23, 16, 20, 5, 3, 0, 0, 0, 1000.000122, ""); mE(b_75, 0.000000); UsedBlocks->Add(b_75);
	//auto b_76 = make1(201, 27, 23, 16, 5, 5, 3, 0, 0, 0, 375.000000, ""); mE(b_76, 0.000000); UsedBlocks->Add(b_76);
	//auto b_77 = make1(1, 4, -31, 12, 5, 1, 1, 90, 0, 0, 50.000000, ""); mE(b_77, 0.000000); UsedBlocks->Add(b_77);
	//auto b_78 = make1(600, 7, -31, 13, 8, 1, 6, 90, 0, -180, 480.000000, ""); mE(b_78, 19200.000000); UsedBlocks->Add(b_78);
	//auto b_79 = make1(1050, 11, -32, 10, 1, 1, 1, 0, -180, 90, 10.000000, "venku"); bd(b_79, TEXT("ReactsToDayCycle"), TEXT("1")); bd(b_79, TEXT("StateAtDay"), TEXT("0")); bd(b_79, TEXT("StateAtNight"), TEXT("1")); mE(b_79, 0.000000); mEP(b_79, 0, 1, 1.000000);
	//mr(b_79, "5CCE09794A8BA7EDD12133AFE8BA3E40"); mrc(b_79, "B38D77EB4E91A8B078D2C99A122301D6", 1); mrc(b_79, "DBBEC9D24F9FB03C11772CA23E1A78B3", 1); mrc(b_79, "FB1023564E262941B714038D3818F46A", 1); mrc(b_79, "9D8E84F0421C7F1BB533DE917CE8BB8A", 1); mrc(b_79, "FFF8DE93402EB4D1C45EE1BD8E8891FA", 1); mrc(b_79, "E1665290437109FA05AD0FB9E4DE2CBD", 1);
	//mrc(b_79, "8BFFE8DF46408261406EF398A7D59983", 1);
	//mrc(b_79, "E491A3E94A3951CC2C66C49F628C037C", 1);
	//mrc(b_79, "5EC8F9784C563E10FC104893F170F3AA", 1);
	//mrc(b_79, "4CC0A909459D22CB2338379B8FC19E09", 1);
	//mrc(b_79, "A771CB064171D8CB1334CF8BC9E3CC44", 1);
	//mrc(b_79, "DE63ABAA4300C4E79B5B93914C36E856", 1);
	//mrc(b_79, "26ABC9A542FC42FA805C54B0D86CBC71", 1);
	//UsedBlocks->Add(b_79);
	//auto b_80 = make1(1150, -12, -4, 4, 3, 3, 2, 0, -180, 0, 180.000000, ""); mE(b_80, 18000.000000); UsedBlocks->Add(b_80);
	//auto b_81 = make1(1150, -9, -4, 4, 3, 3, 2, 0, -180, 0, 180.000000, ""); mE(b_81, 18000.000000); UsedBlocks->Add(b_81);
	//auto b_82 = make1(1150, -6, 2, 4, 3, 3, 2, 0, -180, 0, 180.000000, ""); mE(b_82, 18000.000000); UsedBlocks->Add(b_82);
	//auto b_83 = make1(1150, -6, -1, 4, 3, 3, 2, 0, -180, 0, 180.000000, ""); mE(b_83, 18000.000000); UsedBlocks->Add(b_83);
	//auto b_84 = make1(1150, -6, -4, 4, 3, 3, 2, 0, -180, 0, 180.000000, ""); mE(b_84, 18000.000000); UsedBlocks->Add(b_84);
	//auto b_85 = make1(1100, -5, -35, 18, 20, 9, 2, 0, -180, 0, 3600.000000, ""); mE(b_85, 72000.000000); UsedBlocks->Add(b_85);
	//auto b_86 = make1(1100, -5, -21, 18, 20, 20, 2, 0, 90, 0, 8000.000000, ""); mE(b_86, 160000.000000); UsedBlocks->Add(b_86);
	//auto b_87 = make1(1100, -5, 0, 18, 20, 20, 2, 0, -180, 0, 8000.000000, ""); mE(b_87, 160000.000000); UsedBlocks->Add(b_87);
	//auto b_88 = make1(1100, -6, 15, 18, 20, 11, 2, 0, 0, 0, 4400.000000, ""); mE(b_88, 88000.000000); UsedBlocks->Add(b_88);
	//auto b_89 = make1(1100, 15, 15, 18, 20, 11, 2, 0, 180, 0, 4400.000000, ""); mE(b_89, 88000.000000); UsedBlocks->Add(b_89);
	//auto b_90 = make1(1100, 14, 4, 18, 20, 11, 2, 0, 0, 0, 4400.000000, ""); mE(b_90, 88000.000000); UsedBlocks->Add(b_90);
	//auto b_91 = make1(0, -6, 14, 1, 20, 12, 4, 0, 0, 0, 9600.000000, ""); mE(b_91, 0.000000); UsedBlocks->Add(b_91);
	//auto b_92 = make1(0, 14, 14, 1, 20, 12, 4, 0, 0, 0, 9600.000000, ""); mE(b_92, 0.000000); UsedBlocks->Add(b_92);
	//auto b_93 = make1(1, 20, -6, 18, 8, 9, 2, 0, 90, 0, 1440.000000, ""); mE(b_93, 0.000000); UsedBlocks->Add(b_93);
	//auto b_94 = make1(700, 20, -5, 25, 7, 7, 11, 0, -180, 0, 10.000000, ""); af(b_94, TEXT("DoorOpening"), 0); bd(b_94, TEXT("DoorState"), TEXT("0")); bd(b_94, TEXT("DoorYaw"), TEXT("0.0")); mE(b_94, 0.000000); UsedBlocks->Add(b_94);
	//auto b_95 = make1(1, 16, -17, 18, 14, 1, 2, 0, 90, 0, 280.000000, ""); mE(b_95, 0.000000); UsedBlocks->Add(b_95);
	//auto b_96 = make1(1, 10, -23, 18, 10, 1, 2, 0, 0, 0, 200.000000, ""); mE(b_96, 0.000000); UsedBlocks->Add(b_96);
	//auto b_97 = make1(1, 16, -23, 10, 1, 1, 14, 0, -90, 0, 140.000000, ""); mE(b_97, 0.000000); UsedBlocks->Add(b_97);
	//auto b_98 = make1(1, 15, -31, 18, 20, 1, 2, 0, -180, 0, 400.000000, ""); mE(b_98, 0.000000); UsedBlocks->Add(b_98);
	//auto b_99 = make1(1, 5, -23, 10, 1, 1, 14, 0, -90, 0, 140.000000, ""); mE(b_99, 0.000000); UsedBlocks->Add(b_99);
	//auto b_100 = make1(1, 24, -15, 18, 11, 1, 2, 0, 90, 0, 220.000000, ""); mE(b_100, 0.000000); UsedBlocks->Add(b_100);
	//auto b_101 = make1(1, 24, -25, 18, 10, 1, 2, 0, -90, 0, 200.000000, ""); mE(b_101, 0.000000); UsedBlocks->Add(b_101);
	//auto b_102 = make1(1, 5, -26, 18, 8, 1, 2, 0, -90, 0, 160.000000, ""); mE(b_102, 0.000000); UsedBlocks->Add(b_102);
	//auto b_103 = make1(1100, 15, -35, 18, 20, 8, 2, 0, 180, 0, 3200.000000, ""); mE(b_103, 64000.000000); UsedBlocks->Add(b_103);
	//auto b_104 = make1(1100, 10, -7, 18, 11, 11, 2, 0, -180, 0, 2420.000000, ""); mE(b_104, 48400.000000); UsedBlocks->Add(b_104);
	//auto b_105 = make1(1100, 10, -18, 18, 10, 11, 2, 0, 90, 0, 2200.000000, ""); mE(b_105, 44000.000000); UsedBlocks->Add(b_105);
	//auto b_106 = make1(600, 15, -31, 25, 18, 1, 11, 0, 180, 0, 1980.000000, ""); mE(b_106, 79200.000000); UsedBlocks->Add(b_106);
	//auto b_107 = make1(600, 24, -21, 25, 18, 1, 11, 0, -90, 0, 1980.000000, ""); mE(b_107, 79200.000000); UsedBlocks->Add(b_107);
	//auto b_108 = make1(600, 10, -23, 25, 10, 1, 11, 0, 0, 0, 1100.000000, ""); mE(b_108, 44000.000000); UsedBlocks->Add(b_108);
	//auto b_109 = make1(600, 5, -27, 25, 7, 1, 11, 0, 90, 0, 770.000000, ""); mE(b_109, 30800.000000); UsedBlocks->Add(b_109);
	//auto b_110 = make1(600, 16, -12, 25, 20, 1, 11, 0, -90, 0, 2200.000000, ""); mE(b_110, 88000.000000); UsedBlocks->Add(b_110);
	//auto b_111 = make1(600, 24, -8, 25, 10, 1, 11, 0, 90, 0, 1100.000000, ""); mE(b_111, 44000.000000); UsedBlocks->Add(b_111);
	//auto b_112 = make1(1100, 14, -27, 31, 20, 9, 2, 0, 0, 0, 3600.000000, ""); mE(b_112, 72000.000000); UsedBlocks->Add(b_112);
	//auto b_113 = make1(1000, 24, -31, 30, 1, 1, 1, 0, 180, 180, 10.000000, ""); mE(b_113, 2000.000000); mEP(b_113, 0, 1, 1.000000);
	//mr(b_113, "B08C9CB349F54C8A8D2759905F075011"); mrc(b_113, "9BF508254BFC1E9050D82AACB676C3C2", 0); UsedBlocks->Add(b_113);
	//auto b_114 = make1(1000, 5, -31, 30, 1, 1, 1, 0, 180, 180, 10.000000, ""); mE(b_114, 2000.000000); mEP(b_114, 0, 1, 1.000000);
	//mr(b_114, "3C28CE7C4665E962DDE55D99955C1BA5"); mrc(b_114, "9BF508254BFC1E9050D82AACB676C3C2", 0); UsedBlocks->Add(b_114);
	//auto b_115 = make1(1000, 5, -23, 30, 1, 1, 1, 0, 180, 180, 10.000000, ""); mE(b_115, 2000.000000); mEP(b_115, 0, 1, 1.000000);
	//mr(b_115, "CE6A396C4BD0507316F8249AE2014398"); mrc(b_115, "9BF508254BFC1E9050D82AACB676C3C2", 0); UsedBlocks->Add(b_115);
	//auto b_116 = make1(1100, 20, -12, 31, 20, 9, 2, 0, -90, 0, 3600.000000, ""); mE(b_116, 72000.000000); UsedBlocks->Add(b_116);
	//auto b_117 = make1(101, 20, -2, 31, 9, 1, 2, 0, 0, 0, 60.000004, ""); mE(b_117, 0.000000); UsedBlocks->Add(b_117);
	//auto b_118 = make1(1000, 16, -2, 30, 1, 1, 1, 0, 0, -180, 10.000000, ""); mE(b_118, 2000.000000); mEP(b_118, 0, 1, 1.000000);
	//mr(b_118, "6987FAAD427F5B2D3FD0258BE4D74D42"); mrc(b_118, "9BF508254BFC1E9050D82AACB676C3C2", 0); UsedBlocks->Add(b_118);
	//auto b_119 = make1(1000, 24, -2, 30, 1, 1, 1, 0, 0, -180, 10.000000, ""); mE(b_119, 2000.000000); mEP(b_119, 0, 1, 1.000000);
	//mr(b_119, "52BDA1A444EC5F18D049E0A37DE468A1"); mrc(b_119, "9BF508254BFC1E9050D82AACB676C3C2", 0); UsedBlocks->Add(b_119);
	//auto b_120 = make1(1000, 24, -2, 20, 1, 1, 1, 0, 0, 0, 10.000000, ""); mE(b_120, 2000.000000); mEP(b_120, 0, 1, 1.000000);
	//mr(b_120, "48C1EB7F4283A4CA55E10CB845A8E7E4"); mrc(b_120, "9BF508254BFC1E9050D82AACB676C3C2", 0); UsedBlocks->Add(b_120);
	//auto b_121 = make1(1000, 16, -2, 20, 1, 1, 1, 0, 0, 0, 10.000000, ""); mE(b_121, 2000.000000); mEP(b_121, 0, 1, 1.000000);
	//mr(b_121, "4C312C424F20CFA4F65CECBBEFF6A15E"); mrc(b_121, "9BF508254BFC1E9050D82AACB676C3C2", 0); UsedBlocks->Add(b_121);
	//auto b_122 = make1(1000, 5, -23, 20, 1, 1, 1, 0, 0, 0, 10.000000, ""); mE(b_122, 2000.000000); mEP(b_122, 0, 1, 1.000000);
	//mr(b_122, "CA4B8BCC4B4282E0187CD69B3BC75DC4"); mrc(b_122, "9BF508254BFC1E9050D82AACB676C3C2", 0); UsedBlocks->Add(b_122);
	//auto b_123 = make1(1000, 5, -31, 20, 1, 1, 1, 0, 0, 0, 10.000000, ""); mE(b_123, 2000.000000); mEP(b_123, 0, 1, 1.000000);
	//mr(b_123, "4A4DADB148D31205269D51A3A50093B4"); mrc(b_123, "9BF508254BFC1E9050D82AACB676C3C2", 0); UsedBlocks->Add(b_123);
	//auto b_124 = make1(1000, 24, -31, 20, 1, 1, 1, 0, 0, 0, 10.000000, ""); mE(b_124, 2000.000000); mEP(b_124, 0, 1, 1.000000);
	//mr(b_124, "DE32D649493F485950F52F8C2EA46F3F"); mrc(b_124, "9BF508254BFC1E9050D82AACB676C3C2", 0); UsedBlocks->Add(b_124);
	//auto b_125 = make1(1050, 23, -23, 19, 1, 1, 1, 0, 90, 90, 10.000000, "strecha"); bd(b_125, TEXT("ReactsToDayCycle"), TEXT("1")); bd(b_125, TEXT("StateAtDay"), TEXT("0")); bd(b_125, TEXT("StateAtNight"), TEXT("1")); mE(b_125, 0.000000); mEP(b_125, 0, 1, 1.000000);
	//mr(b_125, "9BF508254BFC1E9050D82AACB676C3C2"); mrc(b_125, "48C1EB7F4283A4CA55E10CB845A8E7E4", 1); mrc(b_125, "4C312C424F20CFA4F65CECBBEFF6A15E", 1); mrc(b_125, "52BDA1A444EC5F18D049E0A37DE468A1", 1); mrc(b_125, "6987FAAD427F5B2D3FD0258BE4D74D42", 1); mrc(b_125, "CA4B8BCC4B4282E0187CD69B3BC75DC4", 1); mrc(b_125, "CE6A396C4BD0507316F8249AE2014398", 1);
	//mrc(b_125, "4A4DADB148D31205269D51A3A50093B4", 1); mrc(b_125, "3C28CE7C4665E962DDE55D99955C1BA5", 1); mrc(b_125, "DE32D649493F485950F52F8C2EA46F3F", 1); mrc(b_125, "B08C9CB349F54C8A8D2759905F075011", 1);
	//UsedBlocks->Add(b_125);
	//auto b_126 = make1(2, 20, -10, 18, 7, 2, 1, 0, 180, 0, 140.000000, ""); UsedBlocks->Add(b_126);
	//auto b_127 = make1(2, -9, 4, 9, 11, 1, 9, 90, 0, -180, 990.000000, ""); UsedBlocks->Add(b_127);
	//auto b_128 = make1(2, -9, -6, 9, 11, 1, 9, 90, 0, 0, 990.000000, ""); UsedBlocks->Add(b_128);
	//auto b_129 = make1(2, -14, -1, 9, 11, 1, 9, 90, 0, 90, 990.000000, ""); UsedBlocks->Add(b_129);
	//auto b_130 = make1(2, -4, -1, 9, 11, 1, 9, 90, 0, 90, 990.000000, ""); UsedBlocks->Add(b_130);
	//auto b_131 = make1(2, -9, -1, 15, 11, 1, 11, 0, 90, 90, 1210.000000, ""); UsedBlocks->Add(b_131);
	//auto b_132 = make1(1, 24, 6, 13, 8, 1, 2, 90, 0, 90, 160.000000, ""); mE(b_132, 0.000000); UsedBlocks->Add(b_132);
	//auto b_133 = make1(1, 24, 20, 10, 1, 1, 14, 0, 0, 0, 140.000000, ""); mE(b_133, 0.000000); UsedBlocks->Add(b_133);
	//auto b_134 = make1(1, -15, 20, 10, 1, 1, 14, 0, 0, 0, 140.000000, ""); mE(b_134, 0.000000); UsedBlocks->Add(b_134);
	//auto b_135 = make1(600, 15, 20, 9, 18, 1, 11, 0, -180, 0, 1980.000000, ""); mE(b_135, 79200.000000); UsedBlocks->Add(b_135);
	//auto b_136 = make1(600, -5, 20, 9, 18, 1, 11, 0, -180, 0, 1980.000000, ""); mE(b_136, 79200.000000); UsedBlocks->Add(b_136);
	//auto b_137 = make1(1, 5, 20, 10, 1, 1, 14, 0, 90, 0, 140.000000, ""); mE(b_137, 0.000000); UsedBlocks->Add(b_137);
	//auto b_138 = make1(1, 4, 20, 10, 1, 1, 14, 0, 90, 0, 140.000000, ""); mE(b_138, 0.000000); UsedBlocks->Add(b_138);
	//auto b_139 = make1(600, -15, 14, 9, 11, 1, 11, 0, -90, 0, 1210.000000, ""); mE(b_139, 48400.000000); UsedBlocks->Add(b_139);
	//auto b_140 = make1(600, 24, 14, 9, 11, 1, 11, 0, -90, 0, 1210.000000, ""); mE(b_140, 48400.000000); UsedBlocks->Add(b_140);
	//auto b_141 = make1(600, 18, 6, 11, 14, 1, 11, -90, -90, 90, 1540.000000, ""); mE(b_141, 61600.000000); UsedBlocks->Add(b_141);
	//auto b_142 = make1(700, -1, 9, 9, 7, 7, 11, 0, 0, 0, 10.000000, ""); af(b_142, TEXT("DoorOpening"), 0); bd(b_142, TEXT("DoorState"), TEXT("0")); bd(b_142, TEXT("DoorYaw"), TEXT("0.0")); mE(b_142, 0.000000); UsedBlocks->Add(b_142);
	//auto b_143 = make1(1, -10, 6, 10, 10, 1, 14, 0, 0, 0, 1400.000000, ""); mE(b_143, 0.000000); UsedBlocks->Add(b_143);
	//auto b_144 = make1(1000, -14, 19, 17, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_144, 2000.000000); mEP(b_144, 1, 1, 1.000000);
	//mr(b_144, "01F2D6414CED08BCA8842BB981B2E87A"); mrc(b_144, "C70D1CEE4B62380C526D50BE6CB5F3F7", 0); UsedBlocks->Add(b_144);
	//auto b_145 = make1(1000, -14, 7, 17, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_145, 2000.000000); mEP(b_145, 1, 1, 1.000000);
	//mr(b_145, "E833DB734847B828C2163B82A6FFC818"); mrc(b_145, "C70D1CEE4B62380C526D50BE6CB5F3F7", 0); UsedBlocks->Add(b_145);
	//auto b_146 = make1(1, 4, 6, 16, 17, 1, 3, 0, 180, 0, 510.000000, ""); mE(b_146, 0.000000); UsedBlocks->Add(b_146);
	//auto b_147 = make1(1, 24, 13, 16, 12, 1, 3, 0, 90, 0, 360.000000, ""); mE(b_147, 0.000000); UsedBlocks->Add(b_147);
	//auto b_148 = make1(500, -9, -7, 6, 1, 8, 5, 0, -90, 0, 10.000000, ""); mE(b_148, 100000.000000); UsedBlocks->Add(b_148);
	//auto b_149 = make1(1, 24, 8, 12, 1, 1, 5, 0, 0, 0, 50.000000, ""); mE(b_149, 0.000000); UsedBlocks->Add(b_149);
	//auto b_150 = make1(700, -12, -16, 9, 7, 7, 11, 0, -90, 0, 10.000000, ""); af(b_150, TEXT("DoorOpening"), 0); bd(b_150, TEXT("DoorState"), TEXT("0")); bd(b_150, TEXT("DoorYaw"), TEXT("0.0")); mE(b_150, 0.000000); UsedBlocks->Add(b_150);
	//auto b_151 = make1(1, -15, -12, 10, 1, 1, 14, 0, -180, 0, 140.000000, ""); mE(b_151, 0.000000); UsedBlocks->Add(b_151);
	//auto b_152 = make1(1, -15, 6, 12, 5, 1, 5, 0, 90, 0, 250.000000, ""); mE(b_152, 0.000000); UsedBlocks->Add(b_152);
	//auto b_153 = make1(600, -15, -4, 12, 15, 1, 5, 0, 90, 0, 750.000000, ""); mE(b_153, 30000.000000); UsedBlocks->Add(b_153);
	//auto b_154 = make1(1, 8, 6, 9, 10, 1, 11, 0, 180, 0, 1100.000000, ""); mE(b_154, 0.000000); UsedBlocks->Add(b_154);
	//auto b_155 = make1(1, -10, 13, 5, 12, 9, 1, 0, -90, 0, 1080.000000, ""); mE(b_155, 0.000000); UsedBlocks->Add(b_155);
	//auto b_156 = make1(1, -12, 7, 6, 2, 3, 1, 0, 90, 0, 60.000000, ""); mE(b_156, 0.000000); UsedBlocks->Add(b_156);
	//auto b_157 = make1(1, -8, 7, 6, 2, 3, 1, 0, 90, 0, 60.000000, ""); mE(b_157, 0.000000); UsedBlocks->Add(b_157);
	//auto b_158 = make1(1, -10, 13, 6, 8, 7, 1, 0, 90, 0, 560.000000, ""); mE(b_158, 0.000000); UsedBlocks->Add(b_158);
	//auto b_159 = make1(1, -10, 19, 4, 9, 1, 2, 0, -180, 0, 180.000000, ""); mE(b_159, 0.000000); UsedBlocks->Add(b_159);
	//auto b_160 = make1(1, -10, 19, 6, 9, 1, 2, 0, -180, 0, 180.000000, ""); mE(b_160, 0.000000); UsedBlocks->Add(b_160);
	//auto b_161 = make1(1050, 4, 7, 11, 1, 1, 1, 0, 0, 90, 10.000000, "loznice"); bd(b_161, TEXT("ReactsToDayCycle"), TEXT("0")); bd(b_161, TEXT("StateAtDay"), TEXT("0")); bd(b_161, TEXT("StateAtNight"), TEXT("0")); mE(b_161, 0.000000); mEP(b_161, 1, 1, 1.000000);
	//mr(b_161, "C70D1CEE4B62380C526D50BE6CB5F3F7"); mrc(b_161, "E833DB734847B828C2163B82A6FFC818", 1); mrc(b_161, "01F2D6414CED08BCA8842BB981B2E87A", 1); mrc(b_161, "2FCC90004F4CF24B9334B88F4C7B5FFB", 1); mrc(b_161, "886E9EFB4E8995200EFA2FB3DC24CB8A", 1); UsedBlocks->Add(b_161);
	//auto b_162 = make1(1000, 23, 7, 17, 1, 1, 1, 0, 90, -180, 10.000000, ""); mE(b_162, 2000.000000); mEP(b_162, 1, 1, 1.000000);
	//mr(b_162, "886E9EFB4E8995200EFA2FB3DC24CB8A"); mrc(b_162, "C70D1CEE4B62380C526D50BE6CB5F3F7", 0); UsedBlocks->Add(b_162);
	//auto b_163 = make1(1000, 23, 20, 17, 1, 1, 1, 0, 90, -180, 10.000000, ""); mE(b_163, 2000.000000); mEP(b_163, 1, 1, 1.000000);
	//mr(b_163, "2FCC90004F4CF24B9334B88F4C7B5FFB"); mrc(b_163, "C70D1CEE4B62380C526D50BE6CB5F3F7", 0); UsedBlocks->Add(b_163);
	//auto b_164 = make1(1000, 23, 5, 17, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_164, 2000.000000); mEP(b_164, 0, 1, 1.000000);
	//mr(b_164, "4C1984E3403ED3A899E0D4878F1BA352"); mrc(b_164, "D45F7BF24F482877B4C421BF92613D99", 0); UsedBlocks->Add(b_164);
	//auto b_165 = make1(1000, 23, -9, 17, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_165, 2000.000000); mEP(b_165, 0, 1, 1.000000);
	//mr(b_165, "BC3075484EEF7C83662C8D854BC441AE"); mrc(b_165, "D45F7BF24F482877B4C421BF92613D99", 0); UsedBlocks->Add(b_165);
	//auto b_166 = make1(1000, -14, -12, 17, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_166, 2000.000000); mEP(b_166, 0, 1, 1.000000);
	//mr(b_166, "E6C3FD9D4B98CDA0053AD397B0599487"); mrc(b_166, "D45F7BF24F482877B4C421BF92613D99", 0); UsedBlocks->Add(b_166);
	//auto b_167 = make1(1000, -15, -30, 17, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_167, 2000.000000); mEP(b_167, 0, 1, 1.000000);
	//mr(b_167, "23B613F74B4820F14A0F378D34590C02"); mrc(b_167, "D45F7BF24F482877B4C421BF92613D99", 0); UsedBlocks->Add(b_167);
	//auto b_168 = make1(1000, 4, -30, 17, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_168, 2000.000000); mEP(b_168, 1, 1, 1.000000);
	//mr(b_168, "D7228B834419A7042C9737B3A8DA5918"); UsedBlocks->Add(b_168);
	//auto b_169 = make1(1000, -14, -38, 14, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_169, 2000.000000); mEP(b_169, 0, 1, 1.000000);
	//mr(b_169, "03265B244826C2A17A11E69C02BA9EEB"); mrc(b_169, "D45F7BF24F482877B4C421BF92613D99", 0); UsedBlocks->Add(b_169);
	//auto b_170 = make1(1050, 2, -31, 11, 1, 1, 1, 0, 90, 90, 10.000000, "hlavni vypinac"); bd(b_170, TEXT("ReactsToDayCycle"), TEXT("0")); bd(b_170, TEXT("StateAtDay"), TEXT("0")); bd(b_170, TEXT("StateAtNight"), TEXT("0")); mE(b_170, 0.000000); mEP(b_170, 0, 1, 1.000000);
	//mr(b_170, "D45F7BF24F482877B4C421BF92613D99"); mrc(b_170, "03265B244826C2A17A11E69C02BA9EEB", 1); mrc(b_170, "E6C3FD9D4B98CDA0053AD397B0599487", 1); mrc(b_170, "23B613F74B4820F14A0F378D34590C02", 1); mrc(b_170, "BC3075484EEF7C83662C8D854BC441AE", 1); mrc(b_170, "4C1984E3403ED3A899E0D4878F1BA352", 1); mrc(b_170, "FBE7184F415AC3753BA6E683B2F0FB19", 1);
	//UsedBlocks->Add(b_170);
	//auto b_171 = make1(1000, 4, -22, 17, 1, 1, 1, 0, 0, -180, 10.000000, ""); mE(b_171, 2000.000000); mEP(b_171, 0, 1, 1.000000);
	//mr(b_171, "FBE7184F415AC3753BA6E683B2F0FB19"); mrc(b_171, "D45F7BF24F482877B4C421BF92613D99", 0); UsedBlocks->Add(b_171);
	//auto b_172 = make1(1000, 25, 21, 14, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_172, 2000.000000); mEP(b_172, 0, 1, 1.000000);
	//mr(b_172, "DE63ABAA4300C4E79B5B93914C36E856"); mrc(b_172, "5CCE09794A8BA7EDD12133AFE8BA3E40", 0); UsedBlocks->Add(b_172);
	//auto b_173 = make1(1000, 5, 21, 14, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_173, 2000.000000); mEP(b_173, 0, 1, 1.000000);
	//mr(b_173, "A771CB064171D8CB1334CF8BC9E3CC44"); mrc(b_173, "5CCE09794A8BA7EDD12133AFE8BA3E40", 0); UsedBlocks->Add(b_173);
	//auto b_174 = make1(1000, 4, 21, 14, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_174, 2000.000000); mEP(b_174, 0, 1, 1.000000);
	//mr(b_174, "4CC0A909459D22CB2338379B8FC19E09"); mrc(b_174, "5CCE09794A8BA7EDD12133AFE8BA3E40", 0); UsedBlocks->Add(b_174);
	//auto b_175 = make1(1000, -16, 21, 14, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_175, 2000.000000); mEP(b_175, 0, 1, 1.000000);
	//mr(b_175, "5EC8F9784C563E10FC104893F170F3AA"); mrc(b_175, "5CCE09794A8BA7EDD12133AFE8BA3E40", 0); UsedBlocks->Add(b_175);
	//auto b_176 = make1(1000, -16, 6, 14, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_176, 2000.000000); mEP(b_176, 0, 1, 1.000000);
	//mr(b_176, "E491A3E94A3951CC2C66C49F628C037C"); mrc(b_176, "5CCE09794A8BA7EDD12133AFE8BA3E40", 0); UsedBlocks->Add(b_176);
	//auto b_177 = make1(1000, -16, -12, 14, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_177, 2000.000000); mEP(b_177, 0, 1, 1.000000);
	//mr(b_177, "8BFFE8DF46408261406EF398A7D59983"); mrc(b_177, "5CCE09794A8BA7EDD12133AFE8BA3E40", 0); UsedBlocks->Add(b_177);
	//auto b_178 = make1(1000, -16, -20, 14, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_178, 2000.000000); mEP(b_178, 0, 1, 1.000000);
	//mr(b_178, "E1665290437109FA05AD0FB9E4DE2CBD"); mrc(b_178, "5CCE09794A8BA7EDD12133AFE8BA3E40", 0); UsedBlocks->Add(b_178);
	//auto b_179 = make1(1000, -16, -40, 14, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_179, 2000.000000); mEP(b_179, 0, 1, 1.000000);
	//mr(b_179, "FFF8DE93402EB4D1C45EE1BD8E8891FA"); mrc(b_179, "5CCE09794A8BA7EDD12133AFE8BA3E40", 0); UsedBlocks->Add(b_179);
	//auto b_180 = make1(1000, -6, -40, 14, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_180, 2000.000000); mEP(b_180, 0, 1, 1.000000);
	//mr(b_180, "9D8E84F0421C7F1BB533DE917CE8BB8A"); mrc(b_180, "5CCE09794A8BA7EDD12133AFE8BA3E40", 0); UsedBlocks->Add(b_180);
	//auto b_181 = make1(1000, 4, -40, 14, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_181, 2000.000000); mEP(b_181, 0, 1, 1.000000);
	//mr(b_181, "FB1023564E262941B714038D3818F46A"); mrc(b_181, "5CCE09794A8BA7EDD12133AFE8BA3E40", 0); UsedBlocks->Add(b_181);
	//auto b_182 = make1(1000, 25, -40, 14, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_182, 2000.000000); mEP(b_182, 0, 1, 1.000000);
	//mr(b_182, "DBBEC9D24F9FB03C11772CA23E1A78B3"); mrc(b_182, "5CCE09794A8BA7EDD12133AFE8BA3E40", 0); UsedBlocks->Add(b_182);
	//auto b_183 = make1(1000, 25, -19, 14, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_183, 2000.000000); mEP(b_183, 0, 1, 1.000000);
	//mr(b_183, "B38D77EB4E91A8B078D2C99A122301D6"); mrc(b_183, "5CCE09794A8BA7EDD12133AFE8BA3E40", 0); UsedBlocks->Add(b_183);
	//auto b_184 = make1(201, 27, 8, 16, 5, 5, 3, 0, -90, 0, 375.000000, ""); mE(b_184, 0.000000); UsedBlocks->Add(b_184);
	//auto b_185 = make1(1000, 25, 10, 14, 1, 1, 1, 0, -90, 180, 10.000000, ""); mE(b_185, 2000.000000); mEP(b_185, 0, 1, 1.000000);
	//mr(b_185, "26ABC9A542FC42FA805C54B0D86CBC71"); mrc(b_185, "5CCE09794A8BA7EDD12133AFE8BA3E40", 0); UsedBlocks->Add(b_185);
	//auto b_186 = make1(101, 27, 16, 16, 10, 5, 3, 0, -90, 0, 500.000061, ""); mE(b_186, 0.000000); UsedBlocks->Add(b_186);
	//auto b_187 = make1(1500, 23, 4, 4, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_187, 1000.000000); UsedBlocks->Add(b_187);
	//auto b_188 = make1(1500, 23, 2, 4, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_188, 1000.000000); UsedBlocks->Add(b_188);
	//auto b_189 = make1(1500, 23, 0, 4, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_189, 1000.000000); UsedBlocks->Add(b_189);
	//auto b_190 = make1(1500, 23, -2, 4, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_190, 1000.000000); UsedBlocks->Add(b_190);
	//auto b_191 = make1(1500, 23, -4, 4, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_191, 1000.000000); UsedBlocks->Add(b_191);
	//auto b_192 = make1(1500, 23, -6, 4, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_192, 1000.000000); UsedBlocks->Add(b_192);
	//auto b_193 = make1(1500, 23, 4, 6, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_193, 1000.000000); UsedBlocks->Add(b_193);
	//auto b_194 = make1(1500, 23, 2, 6, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_194, 1000.000000); UsedBlocks->Add(b_194);
	//auto b_195 = make1(1500, 23, 0, 6, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_195, 1000.000000); UsedBlocks->Add(b_195);
	//auto b_196 = make1(1500, 23, -2, 6, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_196, 1000.000000); UsedBlocks->Add(b_196);
	//auto b_197 = make1(1500, 23, -4, 6, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_197, 1000.000000); UsedBlocks->Add(b_197);
	//auto b_198 = make1(1500, 23, -6, 6, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_198, 1000.000000); UsedBlocks->Add(b_198);
	//auto b_199 = make1(1500, 23, 4, 8, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_199, 1000.000000); UsedBlocks->Add(b_199);
	//auto b_200 = make1(1500, 23, 2, 8, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_200, 1000.000000); UsedBlocks->Add(b_200);
	//auto b_201 = make1(1500, 23, 0, 8, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_201, 1000.000000); UsedBlocks->Add(b_201);
	//auto b_202 = make1(1500, 23, -2, 8, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_202, 1000.000000); UsedBlocks->Add(b_202);
	//auto b_203 = make1(1500, 23, -4, 8, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_203, 1000.000000); UsedBlocks->Add(b_203);
	//auto b_204 = make1(1500, 23, -6, 8, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_204, 1000.000000); UsedBlocks->Add(b_204);

	c->PlayerPosition = FVector(404, 342, 163);
	c->PlayerRotation = FRotator(0, -151, 0);
	c->PlayerCameraRotation = FRotator(352.428925, 208.625107, 0.0);

	return c;
}

#pragma endregion

#pragma region MiniLevel

USaveGameCarrier* UNewGameSaveHolder::getMiniLevelGameSave(bool full)
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemMiniLevel", "Těžké začátky").ToString();
	c->SaveFullDescription = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemMiniLevelFullDescr", "Tento level má velice málo počátečních zásob, ale poskytuje nejvíce prostoru pro to, aby sis postavil svoji základnu přesně podle svých představ.");
	c->FullFilePath = TEXT("_system_miniLevel");
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::MiniLevel;

	if (!full)
		return c;

	c->CurrentTime = FMath::RandHelper(86400);
	c->PlayerUseFPSCamera = true;
	c->PlayerHealth = 1000;
	c->PlayerOxygenComponent.CurrentObjectOxygen = 1000.0f;
	c->PlayerElectricityComponent.CurrentObjectEnergy = 500000.0f;

	c->inventoryTags = makeDefault();

	c->buildableBlocks.Add(makeBuildable(DeleteID, FVector(1, 1, 1)));

	auto UsedBlocks = &c->usedBlocks;

	/*auto b_0 = make1(0, -146, 12, 1, 15, 15, 4, 0, -180, 0, 9000.000000, ""); mE(b_0, 0.000000); UsedBlocks->Add(b_0);
	auto b_1 = make1(700, -142, 9, 9, 7, 7, 11, 0, 90, 0, 10.000000, ""); af(b_1, TEXT("DoorOpening"), 0); bd(b_1, TEXT("DoorState"), TEXT("0")); bd(b_1, TEXT("DoorYaw"), TEXT("0.0")); mE(b_1, 0.000000); UsedBlocks->Add(b_1);
	auto b_2 = make1(1150, -150, 8, 4, 3, 3, 2, 0, -90, 0, 180.000000, ""); mE(b_2, 18000.000000); UsedBlocks->Add(b_2);
	auto b_3 = make1(2, -150, 10, 6, 5, 4, 1, -90, 0, -90, 200.000000, ""); UsedBlocks->Add(b_3);
	auto b_4 = make1(2, -148, 7, 6, 5, 4, 1, -90, 0, 0, 200.000000, ""); UsedBlocks->Add(b_4);
	auto b_5 = make1(2, -150, 6, 6, 5, 4, 1, -90, 0, 90, 200.000000, ""); UsedBlocks->Add(b_5);
	auto b_6 = make1(2, -152, 9, 6, 5, 4, 1, -90, 0, -180, 200.000000, ""); UsedBlocks->Add(b_6);
	auto b_7 = make1(2, -150, 8, 9, 5, 5, 1, 0, -90, 0, 250.000000, ""); UsedBlocks->Add(b_7);
	auto b_8 = make1(500, -152, 10, 12, 1, 8, 5, 0, -180, 0, 10.000000, ""); mE(b_8, 100000.000000); UsedBlocks->Add(b_8);
	auto b_9 = make1(1, -153, 12, 9, 14, 1, 11, 0, 90, 0, 1540.000000, ""); mE(b_9, 0.000000); UsedBlocks->Add(b_9);
	auto b_10 = make1(1, -139, 13, 9, 1, 1, 11, 0, 90, 0, 110.000000, ""); mE(b_10, 0.000000); UsedBlocks->Add(b_10);
	auto b_11 = make1(1, -139, 19, 9, 1, 1, 11, 0, 90, 0, 110.000000, ""); mE(b_11, 0.000000); UsedBlocks->Add(b_11);
	auto b_12 = make1(1, -139, 5, 9, 1, 1, 11, 0, 90, 0, 110.000000, ""); mE(b_12, 0.000000); UsedBlocks->Add(b_12);
	auto b_13 = make1(1, -150, 5, 9, 6, 1, 11, 0, -180, 0, 660.000000, ""); mE(b_13, 0.000000); UsedBlocks->Add(b_13);
	auto b_14 = make1(600, -146, 19, 9, 13, 1, 11, 0, 0, 0, 1430.000000, ""); mE(b_14, 57200.000000); UsedBlocks->Add(b_14);
	auto b_15 = make1(600, -139, 16, 9, 5, 1, 11, 0, -90, 0, 550.000000, ""); mE(b_15, 22000.000000); UsedBlocks->Add(b_15);
	auto b_16 = make1(600, -143, 5, 9, 8, 1, 11, 0, -180, 0, 880.000000, ""); mE(b_16, 35200.000000); UsedBlocks->Add(b_16);
	auto b_17 = make1(1100, -146, 12, 15, 15, 15, 2, 0, 180, 0, 4500.000000, ""); mE(b_17, 90000.000000); UsedBlocks->Add(b_17);
	auto b_18 = make1(1050, -140, 13, 11, 1, 1, 1, 90, 0, 0, 10.000000, ""); bd(b_18, TEXT("IsON"), TEXT("1")); mE(b_18, 0.000000);
	mr(b_18, "E0DE5F0C4742F701EBA06DB8EA53DD81"); mrc(b_18, "4A29A8B643CA566204D5FEB7C7F5ABB1", 1); mrc(b_18, "0BE05C674E43F2D52E3F558BF192E772", 1); mrc(b_18, "F76582C9406D13677F9251A376D755FD", 1); UsedBlocks->Add(b_18);
	auto b_19 = make1(1000, -148, 7, 14, 1, 1, 1, 0, -90, -180, 10.000000, ""); bd(b_19, TEXT("IsON"), TEXT("1")); bd(b_19, TEXT("AutoregulatePower"), TEXT("0")); mE(b_19, 2000.000000);
	mr(b_19, "F76582C9406D13677F9251A376D755FD"); mrc(b_19, "E0DE5F0C4742F701EBA06DB8EA53DD81", 0); UsedBlocks->Add(b_19);
	auto b_20 = make1(1000, -152, 18, 14, 1, 1, 1, 0, -90, -180, 10.000000, ""); bd(b_20, TEXT("IsON"), TEXT("1")); bd(b_20, TEXT("AutoregulatePower"), TEXT("0")); mE(b_20, 2000.000000);
	mr(b_20, "0BE05C674E43F2D52E3F558BF192E772"); mrc(b_20, "E0DE5F0C4742F701EBA06DB8EA53DD81", 0); UsedBlocks->Add(b_20);
	auto b_21 = make1(1000, -140, 18, 14, 1, 1, 1, 0, -90, -180, 10.000000, ""); bd(b_21, TEXT("IsON"), TEXT("1")); bd(b_21, TEXT("AutoregulatePower"), TEXT("0")); mE(b_21, 2000.000000);
	mr(b_21, "4A29A8B643CA566204D5FEB7C7F5ABB1"); mrc(b_21, "E0DE5F0C4742F701EBA06DB8EA53DD81", 0); UsedBlocks->Add(b_21);
	auto b_22 = make1(1200, -151, 17, 5, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_22, 100000.000000); UsedBlocks->Add(b_22);
	auto b_23 = make1(1200, -151, 14, 5, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_23, 100000.000000); UsedBlocks->Add(b_23);
	auto b_24 = make1(1200, -151, 17, 8, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_24, 100000.000000); UsedBlocks->Add(b_24);
	auto b_25 = make1(1200, -151, 14, 8, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_25, 100000.000000); UsedBlocks->Add(b_25);
	auto b_26 = make1(1500, -151, 12, 4, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_26, 1000.000000); UsedBlocks->Add(b_26);
	auto b_27 = make1(1500, -151, 12, 6, 2, 2, 2, 0, 180, 0, 10.000000, ""); mO(b_27, 1000.000000); UsedBlocks->Add(b_27);
	auto b_28 = make1(1500, -151, 12, 8, 2, 2, 2, 0, 180, 0, 10.000000, ""); mO(b_28, 1000.000000); UsedBlocks->Add(b_28);
	auto b_29 = make1(1500, -140, 17, 4, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_29, 1000.000000); UsedBlocks->Add(b_29);
	auto b_30 = make1(1500, -142, 17, 4, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_30, 1000.000000); UsedBlocks->Add(b_30);
	auto b_31 = make1(1500, -140, 17, 6, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_31, 1000.000000); UsedBlocks->Add(b_31);
	auto b_32 = make1(1500, -142, 17, 6, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_32, 1000.000000); UsedBlocks->Add(b_32);
	auto b_33 = make1(1500, -140, 17, 8, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_33, 1000.000000); UsedBlocks->Add(b_33);
	auto b_34 = make1(1500, -142, 17, 8, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_34, 1000.000000); UsedBlocks->Add(b_34);*/

	c->PlayerPosition = FVector(-2948, 328, 163);
	c->PlayerRotation = FRotator(0, -57, 0);
	c->PlayerCameraRotation = FRotator(350.181396, 303.457184, 0.0);

	return c;
}

#pragma endregion

#pragma region MainMenu
USaveGameCarrier* UNewGameSaveHolder::getMainMenuSave(bool full)
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = TEXT("Main Menu Save");
	c->FullFilePath = TEXT("_system_mainMenu");
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::MainMenu;

	if (!full)
		return c;

	c->CurrentTime = FMath::RandHelper(86400);
	c->PlayerUseFPSCamera = true;
	c->PlayerHealth = 1000;
	c->PlayerOxygenComponent.CurrentObjectOxygen = 1000.0f;
	c->PlayerElectricityComponent.CurrentObjectEnergy = 500000.0f;

	auto UsedBlocks = &c->usedBlocks;

	auto def = getDefaultGameSave(full);
	auto super = getSuperSizeGameSave(full);

	UsedBlocks->Append(def->usedBlocks);
	UsedBlocks->Append(super->usedBlocks);

	c->PlayerPosition = FVector(983, -712, 82);
	c->PlayerRotation = FRotator(0, 146, 0);

	return c;
}
#pragma endregion

#pragma region SuperSize

USaveGameCarrier* UNewGameSaveHolder::getSuperSizeGameSave(bool full)
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemSuperSize", "Továrna").ToString();
	c->SaveFullDescription = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemSuperSizeFullDescr", "Snadný level pro úplné začátečníky. Hodně zásob do začátku ti hru velmi usnadní.");
	c->FullFilePath = TEXT("_system_superSize");
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::SuperSize;

	if (!full)
		return c;

	c->CurrentTime = FMath::RandHelper(86400);
	c->PlayerUseFPSCamera = true;
	c->PlayerHealth = 1000;
	c->PlayerOxygenComponent.CurrentObjectOxygen = 1000.0f;
	c->PlayerElectricityComponent.CurrentObjectEnergy = 500000.0f;

	c->inventoryTags = makeDefault();

	c->buildableBlocks.Add(makeBuildable(DeleteID, FVector(1, 1, 1)));

	auto UsedBlocks = &c->usedBlocks;

	/*auto b_0 = make1(0, -72, 21, 1, 20, 20, 4, 0, 90, 0, 16000.000000, ""); mE(b_0, 0.000000); UsedBlocks->Add(b_0);
	auto b_1 = make1(1150, -72, 22, 4, 20, 20, 2, 0, -180, 0, 8000.000000, ""); mE(b_1, 800000.000000); UsedBlocks->Add(b_1);
	auto b_2 = make1(0, -72, 1, 1, 20, 20, 4, 0, 90, 0, 16000.000000, ""); mE(b_2, 0.000000); UsedBlocks->Add(b_2);
	auto b_3 = make1(0, -92, 1, 1, 20, 20, 4, 0, 90, 0, 16000.000000, ""); mE(b_3, 0.000000); UsedBlocks->Add(b_3);
	auto b_4 = make1(0, -92, 21, 1, 20, 20, 4, 0, 90, 0, 16000.000000, ""); mE(b_4, 0.000000); UsedBlocks->Add(b_4);
	auto b_5 = make1(0, -92, 41, 1, 20, 20, 4, 0, 90, 0, 16000.000000, ""); mE(b_5, 0.000000); UsedBlocks->Add(b_5);
	auto b_6 = make1(0, -72, 41, 1, 20, 20, 4, 0, 90, 0, 16000.000000, ""); mE(b_6, 0.000000); UsedBlocks->Add(b_6);
	auto b_7 = make1(0, -52, 41, 1, 20, 20, 4, 0, 90, 0, 16000.000000, ""); mE(b_7, 0.000000); UsedBlocks->Add(b_7);
	auto b_8 = make1(0, -52, 21, 1, 20, 20, 4, 0, 90, 0, 16000.000000, ""); mE(b_8, 0.000000); UsedBlocks->Add(b_8);
	auto b_9 = make1(0, -52, 1, 1, 20, 20, 4, 0, 90, 0, 16000.000000, ""); mE(b_9, 0.000000); UsedBlocks->Add(b_9);
	auto b_10 = make1(1, -102, -8, 13, 1, 1, 20, 0, 180, 0, 200.000000, ""); mE(b_10, 0.000000); UsedBlocks->Add(b_10);
	auto b_11 = make1(1, -102, 51, 13, 1, 1, 20, 0, 180, 0, 200.000000, ""); mE(b_11, 0.000000); UsedBlocks->Add(b_11);
	auto b_12 = make1(1, -43, 51, 13, 1, 1, 20, 0, 180, 0, 200.000000, ""); mE(b_12, 0.000000); UsedBlocks->Add(b_12);
	auto b_13 = make1(1, -43, -8, 13, 1, 1, 20, 0, 180, 0, 200.000000, ""); mE(b_13, 0.000000); UsedBlocks->Add(b_13);
	auto b_14 = make1(1, -102, 2, 13, 19, 1, 20, 0, -90, 0, 3800.000000, ""); mE(b_14, 0.000000); UsedBlocks->Add(b_14);
	auto b_15 = make1(1, -102, 41, 13, 19, 1, 20, 0, -90, 0, 3800.000000, ""); mE(b_15, 0.000000); UsedBlocks->Add(b_15);
	auto b_16 = make1(1, -92, -8, 13, 19, 1, 20, 0, 0, 0, 3800.000000, ""); mE(b_16, 0.000000); UsedBlocks->Add(b_16);
	auto b_17 = make1(1, -92, 51, 13, 19, 1, 20, 0, 0, 0, 3800.000000, ""); mE(b_17, 0.000000); UsedBlocks->Add(b_17);
	auto b_18 = make1(1, -83, 21, 4, 1, 1, 20, 0, 0, 90, 200.000000, ""); mE(b_18, 0.000000); UsedBlocks->Add(b_18);
	auto b_19 = make1(1, -72, 11, 4, 1, 1, 20, 0, 90, 90, 200.000000, ""); mE(b_19, 0.000000); UsedBlocks->Add(b_19);
	auto b_20 = make1(1, -72, 32, 4, 1, 1, 20, 0, 90, 90, 200.000000, ""); mE(b_20, 0.000000); UsedBlocks->Add(b_20);
	auto b_21 = make1(1, -62, 22, 4, 1, 1, 20, 0, -180, 90, 200.000000, ""); mE(b_21, 0.000000); UsedBlocks->Add(b_21);
	auto b_22 = make1(2, -62, 22, 5, 20, 1, 1, 0, -90, 0, 200.000000, ""); UsedBlocks->Add(b_22);
	auto b_23 = make1(2, -73, 32, 5, 20, 1, 1, 0, 0, 0, 200.000000, ""); UsedBlocks->Add(b_23);
	auto b_24 = make1(2, -73, 11, 5, 20, 1, 1, 0, 0, 0, 200.000000, ""); UsedBlocks->Add(b_24);
	auto b_25 = make1(2, -83, 21, 5, 20, 1, 1, 0, 90, 0, 200.000000, ""); UsedBlocks->Add(b_25);
	auto b_26 = make1(2, -73, 11, 15, 20, 1, 20, 0, 0, 0, 4000.000000, ""); UsedBlocks->Add(b_26);
	auto b_27 = make1(2, -73, 32, 15, 20, 1, 20, 0, 0, 0, 4000.000000, ""); UsedBlocks->Add(b_27);
	auto b_28 = make1(2, -83, 21, 15, 20, 1, 20, 0, 90, 0, 4000.000000, ""); UsedBlocks->Add(b_28);
	auto b_29 = make1(2, -62, 22, 15, 20, 1, 20, 0, -90, 0, 4000.000000, ""); UsedBlocks->Add(b_29);
	auto b_30 = make1(101, -62, 11, 4, 1, 1, 1, -90, 0, -90, 3.333333, ""); mE(b_30, 0.000000); UsedBlocks->Add(b_30);
	auto b_31 = make1(101, -83, 11, 4, 1, 1, 1, -90, 0, 180, 3.333333, ""); mE(b_31, 0.000000); UsedBlocks->Add(b_31);
	auto b_32 = make1(101, -83, 32, 4, 1, 1, 1, -90, 0, 90, 3.333333, ""); mE(b_32, 0.000000); UsedBlocks->Add(b_32);
	auto b_33 = make1(101, -62, 32, 4, 1, 1, 1, -90, 0, 0, 3.333333, ""); mE(b_33, 0.000000); UsedBlocks->Add(b_33);
	auto b_34 = make1(101, -62, 11, 14, 20, 1, 1, 90, 0, 180, 66.666672, ""); mE(b_34, 0.000000); UsedBlocks->Add(b_34);
	auto b_35 = make1(101, -83, 11, 14, 20, 1, 1, 90, 0, -90, 66.666672, ""); mE(b_35, 0.000000); UsedBlocks->Add(b_35);
	auto b_36 = make1(101, -83, 32, 14, 20, 1, 1, 90, 0, 0, 66.666672, ""); mE(b_36, 0.000000); UsedBlocks->Add(b_36);
	auto b_37 = make1(101, -62, 32, 14, 20, 1, 1, 90, 0, 90, 66.666672, ""); mE(b_37, 0.000000); UsedBlocks->Add(b_37);
	auto b_38 = make1(101, -62, 32, 25, 1, 1, 1, 90, 0, 90, 3.333333, ""); mE(b_38, 0.000000); UsedBlocks->Add(b_38);
	auto b_39 = make1(101, -62, 11, 25, 1, 1, 1, 90, 0, -180, 3.333333, ""); mE(b_39, 0.000000); UsedBlocks->Add(b_39);
	auto b_40 = make1(101, -83, 11, 25, 1, 1, 1, 90, 0, -90, 3.333333, ""); mE(b_40, 0.000000); UsedBlocks->Add(b_40);
	auto b_41 = make1(101, -83, 32, 25, 1, 1, 1, 90, 0, 0, 3.333333, ""); mE(b_41, 0.000000); UsedBlocks->Add(b_41);
	auto b_42 = make1(101, -83, 21, 26, 20, 1, 1, 0, 90, 0, 66.666672, ""); mE(b_42, 0.000000); UsedBlocks->Add(b_42);
	auto b_43 = make1(101, -72, 11, 26, 20, 1, 1, 0, 180, 0, 66.666672, ""); mE(b_43, 0.000000); UsedBlocks->Add(b_43);
	auto b_44 = make1(101, -62, 22, 26, 20, 1, 1, 0, -90, 0, 66.666672, ""); mE(b_44, 0.000000); UsedBlocks->Add(b_44);
	auto b_45 = make1(101, -73, 32, 26, 20, 1, 1, 0, 0, 0, 66.666672, ""); mE(b_45, 0.000000); UsedBlocks->Add(b_45);
	auto b_46 = make1(2, -73, 22, 26, 20, 1, 20, 0, -90, 90, 4000.000000, ""); UsedBlocks->Add(b_46);
	auto b_47 = make1(201, -83, 11, 26, 1, 1, 1, 0, 180, 0, 5.000000, ""); mE(b_47, 0.000000); UsedBlocks->Add(b_47);
	auto b_48 = make1(201, -62, 11, 26, 1, 1, 1, 0, -90, 0, 5.000000, ""); mE(b_48, 0.000000); UsedBlocks->Add(b_48);
	auto b_49 = make1(201, -62, 32, 26, 1, 1, 1, 0, 0, 0, 5.000000, ""); mE(b_49, 0.000000); UsedBlocks->Add(b_49);
	auto b_50 = make1(201, -83, 32, 26, 1, 1, 1, 0, 90, 0, 5.000000, ""); mE(b_50, 0.000000); UsedBlocks->Add(b_50);
	auto b_51 = make1(1000, -101, -3, 11, 1, 1, 1, -90, 0, 0, 10.000000, ""); mE(b_51, 2000.000000); mEP(b_51, 0, 1, 1.000000);
	mr(b_51, "2C3EDE374443338369117CB5D7D553D9"); mrc(b_51, "4C31F7884C556D01A12D6AB691795A01", 0); UsedBlocks->Add(b_51);
	auto b_52 = make1(1, -98, -4, 14, 7, 7, 1, 0, 180, 0, 490.000000, ""); mE(b_52, 0.000000); UsedBlocks->Add(b_52);
	auto b_53 = make1(2, -95, -4, 13, 7, 2, 1, 0, -90, 0, 140.000000, ""); UsedBlocks->Add(b_53);
	auto b_54 = make1(2, -94, -4, 12, 7, 2, 1, 0, -90, 0, 140.000000, ""); UsedBlocks->Add(b_54);
	auto b_55 = make1(2, -93, -4, 11, 7, 2, 1, 0, -90, 0, 140.000000, ""); UsedBlocks->Add(b_55);
	auto b_56 = make1(2, -92, -4, 10, 7, 2, 1, 0, -90, 0, 140.000000, ""); UsedBlocks->Add(b_56);
	auto b_57 = make1(2, -91, -4, 9, 7, 2, 1, 0, -90, 0, 140.000000, ""); UsedBlocks->Add(b_57);
	auto b_58 = make1(2, -90, -4, 8, 7, 2, 1, 0, -90, 0, 140.000000, ""); UsedBlocks->Add(b_58);
	auto b_59 = make1(2, -89, -4, 7, 7, 2, 1, 0, -90, 0, 140.000000, ""); UsedBlocks->Add(b_59);
	auto b_60 = make1(2, -88, -4, 6, 7, 2, 1, 0, -90, 0, 140.000000, ""); UsedBlocks->Add(b_60);
	auto b_61 = make1(2, -87, -4, 5, 7, 2, 1, 0, -90, 0, 140.000000, ""); UsedBlocks->Add(b_61);
	auto b_62 = make1(2, -86, -4, 4, 7, 2, 1, 0, -90, 0, 140.000000, ""); UsedBlocks->Add(b_62);
	auto b_63 = make1(2, -98, 1, 15, 7, 2, 1, 0, -180, 0, 140.000000, ""); UsedBlocks->Add(b_63);
	auto b_64 = make1(2, -98, 2, 16, 7, 2, 1, 0, -180, 0, 140.000000, ""); UsedBlocks->Add(b_64);
	auto b_65 = make1(2, -98, 3, 17, 7, 2, 1, 0, -180, 0, 140.000000, ""); UsedBlocks->Add(b_65);
	auto b_66 = make1(2, -98, 4, 18, 7, 2, 1, 0, -180, 0, 140.000000, ""); UsedBlocks->Add(b_66);
	auto b_67 = make1(2, -98, 5, 19, 7, 2, 1, 0, -180, 0, 140.000000, ""); UsedBlocks->Add(b_67);
	auto b_68 = make1(2, -98, 6, 20, 7, 2, 1, 0, -180, 0, 140.000000, ""); UsedBlocks->Add(b_68);
	auto b_69 = make1(2, -98, 7, 21, 7, 2, 1, 0, -180, 0, 140.000000, ""); UsedBlocks->Add(b_69);
	auto b_70 = make1(2, -98, 8, 22, 7, 2, 1, 0, -180, 0, 140.000000, ""); UsedBlocks->Add(b_70);
	auto b_71 = make1(2, -98, 9, 23, 7, 2, 1, 0, 180, 0, 140.000000, ""); UsedBlocks->Add(b_71);
	auto b_72 = make1(1000, -84, -7, 21, 1, 1, 1, 0, 180, -90, 10.000000, ""); mE(b_72, 2000.000000); mEP(b_72, 0, 1, 1.000000);
	mr(b_72, "8C4593B3471E366EAF8028A35D9B60F8"); mrc(b_72, "4C31F7884C556D01A12D6AB691795A01", 0); UsedBlocks->Add(b_72);
	auto b_73 = make1(1, -102, 3, 24, 20, 1, 2, 0, -90, 0, 400.000000, ""); mE(b_73, 0.000000); UsedBlocks->Add(b_73);
	auto b_74 = make1(1, -91, -8, 24, 20, 1, 2, 0, -180, 0, 400.000000, ""); mE(b_74, 0.000000); UsedBlocks->Add(b_74);
	auto b_75 = make1(1, -102, 40, 24, 20, 1, 2, 0, 90, 0, 400.000000, ""); mE(b_75, 0.000000); UsedBlocks->Add(b_75);
	auto b_76 = make1(1, -91, 51, 24, 20, 1, 2, 0, -180, 0, 400.000000, ""); mE(b_76, 0.000000); UsedBlocks->Add(b_76);
	auto b_77 = make1(600, -107, 1, 25, 20, 1, 8, 0, 90, -90, 1600.000000, ""); mE(b_77, 64000.000000); UsedBlocks->Add(b_77);
	auto b_78 = make1(600, -93, -12, 25, 20, 1, 8, 0, 0, -90, 1600.000000, ""); mE(b_78, 64000.000000); UsedBlocks->Add(b_78);
	auto b_79 = make1(600, -72, -13, 25, 20, 1, 8, 0, 180, -90, 1600.000000, ""); mE(b_79, 64000.000000); UsedBlocks->Add(b_79);
	auto b_80 = make1(600, -52, -13, 25, 20, 1, 8, 0, 180, -90, 1600.000000, ""); mE(b_80, 64000.000000); UsedBlocks->Add(b_80);
	auto b_81 = make1(600, -106, 22, 25, 20, 1, 8, 0, -90, -90, 1600.000000, ""); mE(b_81, 64000.000000); UsedBlocks->Add(b_81);
	auto b_82 = make1(600, -106, 42, 25, 20, 1, 8, 0, -90, -90, 1600.000000, ""); mE(b_82, 64000.000000); UsedBlocks->Add(b_82);
	auto b_83 = make1(600, -92, 55, 25, 20, 1, 8, 0, 180, -90, 1600.000000, ""); mE(b_83, 64000.000000); UsedBlocks->Add(b_83);
	auto b_84 = make1(600, -73, 55, 25, 20, 1, 8, 0, 0, 90, 1600.000000, ""); mE(b_84, 64000.000000); UsedBlocks->Add(b_84);
	auto b_85 = make1(600, -52, 55, 25, 20, 1, 8, 0, -180, -90, 1600.000000, ""); mE(b_85, 64000.000000); UsedBlocks->Add(b_85);
	auto b_86 = make1(600, -39, 42, 25, 20, 1, 8, 0, -90, 90, 1600.000000, ""); mE(b_86, 64000.000000); UsedBlocks->Add(b_86);
	auto b_87 = make1(600, -38, 21, 25, 20, 1, 8, 0, 90, 90, 1600.000000, ""); mE(b_87, 64000.000000); UsedBlocks->Add(b_87);
	auto b_88 = make1(600, -38, 1, 25, 20, 1, 8, 0, 90, 90, 1600.000000, ""); mE(b_88, 64000.000000); UsedBlocks->Add(b_88);
	auto b_89 = make1(101, -106, 55, 25, 1, 8, 8, -90, 0, 90, 213.333344, ""); mE(b_89, 0.000000); UsedBlocks->Add(b_89);
	auto b_90 = make1(101, -39, 55, 25, 1, 8, 8, -90, 0, 0, 213.333344, ""); mE(b_90, 0.000000); UsedBlocks->Add(b_90);
	auto b_91 = make1(101, -39, -12, 25, 1, 8, 8, -90, 0, -90, 213.333344, ""); mE(b_91, 0.000000); UsedBlocks->Add(b_91);
	auto b_92 = make1(101, -106, -12, 25, 1, 8, 8, -90, 0, 180, 213.333344, ""); mE(b_92, 0.000000); UsedBlocks->Add(b_92);
	auto b_93 = make1(1, -97, 47, 26, 10, 10, 2, 0, -180, 0, 2000.000000, ""); mE(b_93, 0.000000); UsedBlocks->Add(b_93);
	auto b_94 = make1(1, -87, 47, 26, 10, 10, 2, 0, 180, 0, 2000.000000, ""); mE(b_94, 0.000000); UsedBlocks->Add(b_94);
	auto b_95 = make1(1, -77, 47, 26, 10, 10, 2, 0, 180, 0, 2000.000000, ""); mE(b_95, 0.000000); UsedBlocks->Add(b_95);
	auto b_96 = make1(1, -67, 47, 26, 10, 10, 2, 0, 180, 0, 2000.000000, ""); mE(b_96, 0.000000); UsedBlocks->Add(b_96);
	auto b_97 = make1(1, -57, 47, 26, 10, 10, 2, 0, 180, 0, 2000.000000, ""); mE(b_97, 0.000000); UsedBlocks->Add(b_97);
	auto b_98 = make1(1, -97, 36, 26, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_98, 0.000000); UsedBlocks->Add(b_98);
	auto b_99 = make1(1, -97, 26, 26, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_99, 0.000000); UsedBlocks->Add(b_99);
	auto b_100 = make1(1, -97, 16, 26, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_100, 0.000000); UsedBlocks->Add(b_100);
	auto b_101 = make1(1, -47, 46, 26, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_101, 0.000000); UsedBlocks->Add(b_101);
	auto b_102 = make1(1, -47, 36, 26, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_102, 0.000000); UsedBlocks->Add(b_102);
	auto b_103 = make1(1, -47, 26, 26, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_103, 0.000000); UsedBlocks->Add(b_103);
	auto b_104 = make1(1, -47, 16, 26, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_104, 0.000000); UsedBlocks->Add(b_104);
	auto b_105 = make1(1, -47, 6, 26, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_105, 0.000000); UsedBlocks->Add(b_105);
	auto b_106 = make1(1, -47, -4, 26, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_106, 0.000000); UsedBlocks->Add(b_106);
	auto b_107 = make1(1, -58, -4, 26, 10, 10, 2, 0, 0, 0, 2000.000000, ""); mE(b_107, 0.000000); UsedBlocks->Add(b_107);
	auto b_108 = make1(1, -68, -4, 26, 10, 10, 2, 0, 0, 0, 2000.000000, ""); mE(b_108, 0.000000); UsedBlocks->Add(b_108);
	auto b_109 = make1(1, -78, -4, 26, 10, 10, 2, 0, 0, 0, 2000.000000, ""); mE(b_109, 0.000000); UsedBlocks->Add(b_109);
	auto b_110 = make1(1, -88, -4, 26, 10, 10, 2, 0, 0, 0, 2000.000000, ""); mE(b_110, 0.000000); UsedBlocks->Add(b_110);
	auto b_111 = make1(2, -98, 9, 24, 7, 2, 1, 0, 0, 0, 140.000000, ""); UsedBlocks->Add(b_111);
	auto b_112 = make1(2, -98, 10, 25, 7, 2, 1, 0, 0, 0, 140.000000, ""); UsedBlocks->Add(b_112);
	auto b_113 = make1(1, -53, -8, 13, 19, 1, 20, 0, -180, 0, 3800.000000, ""); mE(b_113, 0.000000); UsedBlocks->Add(b_113);
	auto b_114 = make1(1, -43, 2, 13, 19, 1, 20, 0, 90, 0, 3800.000000, ""); mE(b_114, 0.000000); UsedBlocks->Add(b_114);
	auto b_115 = make1(1, -43, 41, 13, 19, 1, 20, 0, 90, 0, 3800.000000, ""); mE(b_115, 0.000000); UsedBlocks->Add(b_115);
	auto b_116 = make1(1, -53, 51, 13, 19, 1, 20, 0, 0, 0, 3800.000000, ""); mE(b_116, 0.000000); UsedBlocks->Add(b_116);
	auto b_117 = make1(1, -53, -8, 24, 20, 1, 2, 0, -180, 0, 400.000000, ""); mE(b_117, 0.000000); UsedBlocks->Add(b_117);
	auto b_118 = make1(1, -43, 2, 24, 20, 1, 2, 0, 90, 0, 400.000000, ""); mE(b_118, 0.000000); UsedBlocks->Add(b_118);
	auto b_119 = make1(1, -43, 40, 24, 20, 1, 2, 0, 90, 0, 400.000000, ""); mE(b_119, 0.000000); UsedBlocks->Add(b_119);
	auto b_120 = make1(1, -54, 51, 24, 20, 1, 2, 0, 0, 0, 400.000000, ""); mE(b_120, 0.000000); UsedBlocks->Add(b_120);
	auto b_121 = make1(1, -93, 2, 26, 20, 1, 2, 0, -90, 0, 400.000000, ""); mE(b_121, 0.000000); UsedBlocks->Add(b_121);
	auto b_122 = make1(1, -102, 2, 26, 20, 1, 2, 0, -90, 0, 400.000000, ""); mE(b_122, 0.000000); UsedBlocks->Add(b_122);
	auto b_123 = make1(101, -94, 0, 13, 20, 1, 1, 90, 0, 90, 66.666672, ""); mE(b_123, 0.000000); UsedBlocks->Add(b_123);
	auto b_124 = make1(101, -98, 0, 14, 7, 1, 1, 0, 0, 90, 23.333334, ""); mE(b_124, 0.000000); UsedBlocks->Add(b_124);
	auto b_125 = make1(1, -72, -8, 24, 18, 1, 2, 0, 180, 0, 360.000000, ""); mE(b_125, 0.000000); UsedBlocks->Add(b_125);
	auto b_126 = make1(1, -102, 21, 24, 18, 1, 2, 0, 90, 0, 360.000000, ""); mE(b_126, 0.000000); UsedBlocks->Add(b_126);
	auto b_127 = make1(1, -73, 51, 24, 18, 1, 2, 0, 0, 0, 360.000000, ""); mE(b_127, 0.000000); UsedBlocks->Add(b_127);
	auto b_128 = make1(1, -43, 22, 24, 18, 1, 2, 0, -90, 0, 360.000000, ""); mE(b_128, 0.000000); UsedBlocks->Add(b_128);
	auto b_129 = make1(700, -46, 15, 9, 7, 7, 11, 0, 90, 0, 10.000000, ""); af(b_129, TEXT("DoorOpening"), 0); bd(b_129, TEXT("DoorState"), TEXT("0")); bd(b_129, TEXT("DoorYaw"), TEXT("0.0")); mE(b_129, 0.000000); UsedBlocks->Add(b_129);
	auto b_130 = make1(700, -66, -5, 9, 7, 7, 11, 0, 0, 0, 10.000000, ""); af(b_130, TEXT("DoorOpening"), 1); bd(b_130, TEXT("DoorState"), TEXT("0")); bd(b_130, TEXT("DoorYaw"), TEXT("0.0")); mE(b_130, 0.000000); UsedBlocks->Add(b_130);
	auto b_131 = make1(600, -73, -8, 19, 20, 1, 9, 0, 0, 0, 1800.000000, ""); mE(b_131, 72000.000000); UsedBlocks->Add(b_131);
	auto b_132 = make1(600, -43, 21, 19, 20, 1, 9, 0, 90, 0, 1800.000000, ""); mE(b_132, 72000.000000); UsedBlocks->Add(b_132);
	auto b_133 = make1(600, -72, 51, 19, 20, 1, 9, 0, 180, 0, 1800.000000, ""); mE(b_133, 72000.000000); UsedBlocks->Add(b_133);
	auto b_134 = make1(600, -102, 22, 19, 20, 1, 9, 0, -90, 0, 1800.000000, ""); mE(b_134, 72000.000000); UsedBlocks->Add(b_134);
	auto b_135 = make1(700, -79, 48, 9, 7, 7, 11, 0, -180, 0, 10.000000, ""); af(b_135, TEXT("DoorOpening"), 1); bd(b_135, TEXT("DoorState"), TEXT("0")); bd(b_135, TEXT("DoorYaw"), TEXT("0.0")); mE(b_135, 0.000000); UsedBlocks->Add(b_135);
	auto b_136 = make1(700, -99, 28, 9, 7, 7, 11, 0, -90, 0, 10.000000, ""); af(b_136, TEXT("DoorOpening"), 0); bd(b_136, TEXT("DoorState"), TEXT("0")); bd(b_136, TEXT("DoorYaw"), TEXT("0.0")); mE(b_136, 0.000000); UsedBlocks->Add(b_136);
	auto b_137 = make1(0, -38, 21, 1, 20, 8, 4, 0, 90, 0, 6400.000000, ""); mE(b_137, 0.000000); UsedBlocks->Add(b_137);
	auto b_138 = make1(0, -72, 56, 1, 20, 8, 4, 0, -180, 0, 6400.000000, ""); mE(b_138, 0.000000); UsedBlocks->Add(b_138);
	auto b_139 = make1(0, -107, 22, 1, 20, 8, 4, 0, -90, 0, 6400.000000, ""); mE(b_139, 0.000000); UsedBlocks->Add(b_139);
	auto b_140 = make1(0, -73, -13, 1, 20, 8, 4, 0, 0, 0, 6400.000000, ""); mE(b_140, 0.000000); UsedBlocks->Add(b_140);
	auto b_141 = make1(100, -38, 2, 1, 8, 20, 4, 0, 180, 0, 2133.333496, ""); mE(b_141, 0.000000); UsedBlocks->Add(b_141);
	auto b_142 = make1(100, -53, -12, 1, 8, 20, 4, 0, -90, 0, 2133.333496, ""); mE(b_142, 0.000000); UsedBlocks->Add(b_142);
	auto b_143 = make1(100, -92, -13, 1, 8, 20, 4, 0, 90, 0, 2133.333496, ""); mE(b_143, 0.000000); UsedBlocks->Add(b_143);
	auto b_144 = make1(100, -106, 2, 1, 8, 20, 4, 0, 180, 0, 2133.333496, ""); mE(b_144, 0.000000); UsedBlocks->Add(b_144);
	auto b_145 = make1(100, -107, 41, 1, 8, 20, 4, 0, 0, 0, 2133.333496, ""); mE(b_145, 0.000000); UsedBlocks->Add(b_145);
	auto b_146 = make1(100, -92, 55, 1, 8, 20, 4, 0, 90, 0, 2133.333496, ""); mE(b_146, 0.000000); UsedBlocks->Add(b_146);
	auto b_147 = make1(100, -53, 56, 1, 8, 20, 4, 0, -90, 0, 2133.333496, ""); mE(b_147, 0.000000); UsedBlocks->Add(b_147);
	auto b_148 = make1(100, -39, 41, 1, 8, 20, 4, 0, 0, 0, 2133.333496, ""); mE(b_148, 0.000000); UsedBlocks->Add(b_148);
	auto b_149 = make1(1000, -42, 32, 15, 1, 1, 1, 0, -90, 90, 10.000000, ""); mE(b_149, 2000.000000); mEP(b_149, 1, 1, 1.000000);
	mr(b_149, "CA7C8EE044CF5940EDFAF5AF1E0F70B5"); mrc(b_149, "409F8BAE4B0DDDC12F8E92A974B21DCF", 0); UsedBlocks->Add(b_149);
	auto b_150 = make1(1000, -42, 11, 15, 1, 1, 1, 0, -90, 90, 10.000000, ""); mE(b_150, 2000.000000); mEP(b_150, 1, 1, 1.000000);
	mr(b_150, "90CF4EAA4385DE9368821B9B0124A37F"); mrc(b_150, "409F8BAE4B0DDDC12F8E92A974B21DCF", 0); UsedBlocks->Add(b_150);
	auto b_151 = make1(1000, -62, 52, 15, 1, 1, 1, 0, 0, 90, 10.000000, ""); mE(b_151, 2000.000000); mEP(b_151, 1, 1, 1.000000);
	mr(b_151, "36ACA6D5404A6F6701E976B255CF5415"); mrc(b_151, "409F8BAE4B0DDDC12F8E92A974B21DCF", 0); UsedBlocks->Add(b_151);
	auto b_152 = make1(1000, -83, 52, 15, 1, 1, 1, 0, 0, 90, 10.000000, ""); mE(b_152, 2000.000000); mEP(b_152, 1, 1, 1.000000);
	mr(b_152, "9C926C8C40782D161292889F05820CC9"); mrc(b_152, "409F8BAE4B0DDDC12F8E92A974B21DCF", 0); UsedBlocks->Add(b_152);
	auto b_153 = make1(1000, -103, 32, 15, 1, 1, 1, 0, 90, 90, 10.000000, ""); mE(b_153, 2000.000000); mEP(b_153, 1, 1, 1.000000);
	mr(b_153, "11ED8BEE4DFA99EE096FCCAE702C58BA"); mrc(b_153, "409F8BAE4B0DDDC12F8E92A974B21DCF", 0); UsedBlocks->Add(b_153);
	auto b_154 = make1(1000, -103, 11, 15, 1, 1, 1, 0, 90, 90, 10.000000, ""); mE(b_154, 2000.000000); mEP(b_154, 1, 1, 1.000000);
	mr(b_154, "C219CF2348901785535E3D96003DE0A9"); mrc(b_154, "409F8BAE4B0DDDC12F8E92A974B21DCF", 0); UsedBlocks->Add(b_154);
	auto b_155 = make1(1000, -83, -9, 15, 1, 1, 1, 0, -180, 90, 10.000000, ""); mE(b_155, 2000.000000); mEP(b_155, 1, 1, 1.000000);
	mr(b_155, "010F6056495DF42C202EC9B9516F80B0"); mrc(b_155, "409F8BAE4B0DDDC12F8E92A974B21DCF", 0); UsedBlocks->Add(b_155);
	auto b_156 = make1(1000, -62, -9, 15, 1, 1, 1, 0, -180, 90, 10.000000, ""); mE(b_156, 2000.000000); mEP(b_156, 1, 1, 1.000000);
	mr(b_156, "75202C82439F58D5B7AF9AA470EDB8DA"); mrc(b_156, "409F8BAE4B0DDDC12F8E92A974B21DCF", 0); UsedBlocks->Add(b_156);
	auto b_157 = make1(1, -98, -8, 26, 8, 1, 2, 0, 0, 0, 160.000000, ""); mE(b_157, 0.000000); UsedBlocks->Add(b_157);
	auto b_158 = make1(600, -97, 51, 33, 9, 1, 11, 0, -180, 0, 990.000000, ""); mE(b_158, 39600.000000); UsedBlocks->Add(b_158);
	auto b_159 = make1(600, -48, 51, 33, 9, 1, 11, 0, -180, 0, 990.000000, ""); mE(b_159, 39600.000000); UsedBlocks->Add(b_159);
	auto b_160 = make1(600, -43, 46, 33, 9, 1, 11, 0, 90, 0, 990.000000, ""); mE(b_160, 39600.000000); UsedBlocks->Add(b_160);
	auto b_161 = make1(600, -43, -3, 33, 9, 1, 11, 0, 90, 0, 990.000000, ""); mE(b_161, 39600.000000); UsedBlocks->Add(b_161);
	auto b_162 = make1(600, -48, -8, 33, 9, 1, 11, 0, 0, 0, 990.000000, ""); mE(b_162, 39600.000000); UsedBlocks->Add(b_162);
	auto b_163 = make1(600, -97, -8, 33, 9, 1, 11, 0, 0, 0, 990.000000, ""); mE(b_163, 39600.000000); UsedBlocks->Add(b_163);
	auto b_164 = make1(600, -102, -3, 33, 9, 1, 11, 0, -90, 0, 990.000000, ""); mE(b_164, 39600.000000); UsedBlocks->Add(b_164);
	auto b_165 = make1(600, -87, -8, 33, 10, 1, 11, 0, -180, 0, 1100.000000, ""); mE(b_165, 44000.000000); UsedBlocks->Add(b_165);
	auto b_166 = make1(600, -57, -8, 33, 10, 1, 11, 0, -180, 0, 1100.000000, ""); mE(b_166, 44000.000000); UsedBlocks->Add(b_166);
	auto b_167 = make1(600, -43, 6, 33, 10, 1, 11, 0, 90, 0, 1100.000000, ""); mE(b_167, 44000.000000); UsedBlocks->Add(b_167);
	auto b_168 = make1(600, -43, 36, 33, 10, 1, 11, 0, 90, 0, 1100.000000, ""); mE(b_168, 44000.000000); UsedBlocks->Add(b_168);
	auto b_169 = make1(600, -58, 51, 33, 10, 1, 11, 0, 0, 0, 1100.000000, ""); mE(b_169, 44000.000000); UsedBlocks->Add(b_169);
	auto b_170 = make1(600, -88, 51, 33, 10, 1, 11, 0, 0, 0, 1100.000000, ""); mE(b_170, 44000.000000); UsedBlocks->Add(b_170);
	auto b_171 = make1(600, -102, 37, 33, 10, 1, 11, 0, -90, 0, 1100.000000, ""); mE(b_171, 44000.000000); UsedBlocks->Add(b_171);
	auto b_172 = make1(600, -102, 46, 33, 9, 1, 11, 0, 90, 0, 990.000000, ""); mE(b_172, 39600.000000); UsedBlocks->Add(b_172);
	auto b_173 = make1(600, -102, 7, 33, 10, 1, 11, 0, -90, 0, 1100.000000, ""); mE(b_173, 44000.000000); UsedBlocks->Add(b_173);
	auto b_174 = make1(1100, -48, 46, 39, 10, 10, 2, 0, 0, 0, 2000.000000, ""); mE(b_174, 40000.000000); UsedBlocks->Add(b_174);
	auto b_175 = make1(1100, -58, 46, 39, 10, 10, 2, 0, 0, 0, 2000.000000, ""); mE(b_175, 40000.000000); UsedBlocks->Add(b_175);
	auto b_176 = make1(1100, -68, 46, 39, 10, 10, 2, 0, 0, 0, 2000.000000, ""); mE(b_176, 40000.000000); UsedBlocks->Add(b_176);
	auto b_177 = make1(1100, -78, 46, 39, 10, 10, 2, 0, 0, 0, 2000.000000, ""); mE(b_177, 40000.000000); UsedBlocks->Add(b_177);
	auto b_178 = make1(1100, -47, 36, 39, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_178, 40000.000000); UsedBlocks->Add(b_178);
	auto b_179 = make1(1100, -47, 26, 39, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_179, 40000.000000); UsedBlocks->Add(b_179);
	auto b_180 = make1(1100, -47, 16, 39, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_180, 40000.000000); UsedBlocks->Add(b_180);
	auto b_181 = make1(1000, -44, 50, 38, 1, 1, 1, 0, 180, 180, 10.000000, "strop"); mE(b_181, 2000.000000); mEP(b_181, 1, 1, 1.000000);
	mr(b_181, "58800FBA4BB3478DB03516ABA5B58CB8"); mrc(b_181, "871552E149B983151EB42EBC88527631", 0); UsedBlocks->Add(b_181);
	auto b_182 = make1(1100, -47, 6, 39, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_182, 40000.000000); UsedBlocks->Add(b_182);
	auto b_183 = make1(1100, -48, -4, 39, 10, 10, 2, 0, 0, 0, 2000.000000, ""); mE(b_183, 40000.000000); UsedBlocks->Add(b_183);
	auto b_184 = make1(1100, -58, -4, 39, 10, 10, 2, 0, 0, 0, 2000.000000, ""); mE(b_184, 40000.000000); UsedBlocks->Add(b_184);
	auto b_185 = make1(1100, -68, -4, 39, 10, 10, 2, 0, 0, 0, 2000.000000, ""); mE(b_185, 40000.000000); UsedBlocks->Add(b_185);
	auto b_186 = make1(1000, -43, -8, 38, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_186, 2000.000000); mEP(b_186, 1, 1, 1.000000);
	mr(b_186, "F630E3294352487131950F9CF6CFD772"); mrc(b_186, "409F8BAE4B0DDDC12F8E92A974B21DCF", 0); UsedBlocks->Add(b_186);
	auto b_187 = make1(1000, -43, -8, 28, 1, 1, 1, 0, -90, 0, 10.000000, ""); mE(b_187, 2000.000000); mEP(b_187, 1, 1, 1.000000);
	mr(b_187, "40B54F9949B401D1BB8007B104A896E7"); mrc(b_187, "409F8BAE4B0DDDC12F8E92A974B21DCF", 0); UsedBlocks->Add(b_187);
	auto b_188 = make1(1000, -43, 51, 28, 1, 1, 1, 0, -90, 0, 10.000000, ""); mE(b_188, 2000.000000); mEP(b_188, 1, 1, 1.000000);
	mr(b_188, "5725DC8E4860A9E16F1FC0A28A7DD7B8"); mrc(b_188, "409F8BAE4B0DDDC12F8E92A974B21DCF", 0); UsedBlocks->Add(b_188);
	auto b_189 = make1(1000, -43, 51, 38, 1, 1, 1, 0, -90, -180, 10.000000, ""); mE(b_189, 2000.000000); mEP(b_189, 1, 1, 1.000000);
	mr(b_189, "D5A1945F47EC757D41C7DF9440CA09CC"); mrc(b_189, "409F8BAE4B0DDDC12F8E92A974B21DCF", 0); UsedBlocks->Add(b_189);
	auto b_190 = make1(1000, -102, 51, 28, 1, 1, 1, 0, 90, 0, 10.000000, ""); mE(b_190, 2000.000000); mEP(b_190, 1, 1, 1.000000);
	mr(b_190, "8391987A4F47E8B8CED0AF806E83238C"); mrc(b_190, "409F8BAE4B0DDDC12F8E92A974B21DCF", 0); UsedBlocks->Add(b_190);
	auto b_191 = make1(1000, -102, -8, 28, 1, 1, 1, 0, -180, 0, 10.000000, ""); mE(b_191, 2000.000000); mEP(b_191, 1, 1, 1.000000);
	mr(b_191, "5466012F48745AB130212294A94276DC"); mrc(b_191, "409F8BAE4B0DDDC12F8E92A974B21DCF", 0); UsedBlocks->Add(b_191);
	auto b_192 = make1(1100, -78, -4, 39, 10, 10, 2, 0, 0, 0, 2000.000000, ""); mE(b_192, 40000.000000); UsedBlocks->Add(b_192);
	auto b_193 = make1(1100, -88, -4, 39, 10, 10, 2, 0, 0, 0, 2000.000000, ""); mE(b_193, 40000.000000); UsedBlocks->Add(b_193);
	auto b_194 = make1(1100, -98, -4, 39, 10, 10, 2, 0, 0, 0, 2000.000000, ""); mE(b_194, 40000.000000); UsedBlocks->Add(b_194);
	auto b_195 = make1(1100, -98, 6, 39, 10, 10, 2, 0, 0, 0, 2000.000000, ""); mE(b_195, 40000.000000); UsedBlocks->Add(b_195);
	auto b_196 = make1(1000, -102, -8, 38, 1, 1, 1, 0, 90, -180, 10.000000, ""); mE(b_196, 2000.000000); mEP(b_196, 1, 1, 1.000000);
	mr(b_196, "7D26F2134D6E906B70C01EA5D4FFF34D"); mrc(b_196, "409F8BAE4B0DDDC12F8E92A974B21DCF", 0); UsedBlocks->Add(b_196);
	auto b_197 = make1(1000, -101, -7, 38, 1, 1, 1, 0, 0, -180, 10.000000, "strop"); mE(b_197, 2000.000000); mEP(b_197, 1, 1, 1.000000);
	mr(b_197, "5DE934F943CBE06F41B5A9A569B693B7"); mrc(b_197, "871552E149B983151EB42EBC88527631", 0); UsedBlocks->Add(b_197);
	auto b_198 = make1(1100, -87, 46, 39, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_198, 40000.000000); UsedBlocks->Add(b_198);
	auto b_199 = make1(1100, -97, 46, 39, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_199, 40000.000000); UsedBlocks->Add(b_199);
	auto b_200 = make1(1100, -97, 36, 39, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_200, 40000.000000); UsedBlocks->Add(b_200);
	auto b_201 = make1(1100, -97, 26, 39, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_201, 40000.000000); UsedBlocks->Add(b_201);
	auto b_202 = make1(1100, -97, 16, 39, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_202, 40000.000000); UsedBlocks->Add(b_202);
	auto b_203 = make1(101, -89, 5, 40, 1, 8, 8, 90, 0, 90, 213.333344, ""); mE(b_203, 0.000000); UsedBlocks->Add(b_203);
	auto b_204 = make1(101, -89, 38, 40, 1, 8, 8, 90, 0, -180, 213.333344, ""); mE(b_204, 0.000000); UsedBlocks->Add(b_204);
	auto b_205 = make1(101, -56, 38, 40, 1, 8, 8, 90, 0, -90, 213.333344, ""); mE(b_205, 0.000000); UsedBlocks->Add(b_205);
	auto b_206 = make1(101, -56, 5, 40, 1, 8, 8, 90, 0, 0, 213.333344, ""); mE(b_206, 0.000000); UsedBlocks->Add(b_206);
	auto b_207 = make1(1100, -57, 26, 41, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_207, 40000.000000); UsedBlocks->Add(b_207);
	auto b_208 = make1(1100, -57, 16, 41, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_208, 40000.000000); UsedBlocks->Add(b_208);
	auto b_209 = make1(1100, -57, 6, 41, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_209, 40000.000000); UsedBlocks->Add(b_209);
	auto b_210 = make1(1100, -67, 6, 41, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_210, 40000.000000); UsedBlocks->Add(b_210);
	auto b_211 = make1(1100, -77, 6, 41, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_211, 40000.000000); UsedBlocks->Add(b_211);
	auto b_212 = make1(1100, -87, 6, 41, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_212, 40000.000000); UsedBlocks->Add(b_212);
	auto b_213 = make1(1100, -87, 16, 41, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_213, 40000.000000); UsedBlocks->Add(b_213);
	auto b_214 = make1(1100, -87, 26, 41, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_214, 40000.000000); UsedBlocks->Add(b_214);
	auto b_215 = make1(1100, -87, 36, 41, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_215, 40000.000000); UsedBlocks->Add(b_215);
	auto b_216 = make1(1100, -77, 36, 41, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_216, 40000.000000); UsedBlocks->Add(b_216);
	auto b_217 = make1(1100, -67, 36, 41, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_217, 40000.000000); UsedBlocks->Add(b_217);
	auto b_218 = make1(1100, -57, 36, 41, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_218, 40000.000000); UsedBlocks->Add(b_218);
	auto b_219 = make1(101, -79, 28, 42, 1, 8, 8, -90, 0, -90, 213.333344, ""); mE(b_219, 0.000000); UsedBlocks->Add(b_219);
	auto b_220 = make1(101, -79, 15, 42, 1, 8, 8, -90, 0, 0, 213.333344, ""); mE(b_220, 0.000000); UsedBlocks->Add(b_220);
	auto b_221 = make1(101, -66, 15, 42, 1, 8, 8, -90, 0, 90, 213.333344, ""); mE(b_221, 0.000000); UsedBlocks->Add(b_221);
	auto b_222 = make1(101, -66, 28, 42, 1, 8, 8, -90, 0, -180, 213.333344, ""); mE(b_222, 0.000000); UsedBlocks->Add(b_222);
	auto b_223 = make1(1100, -68, 26, 43, 10, 10, 2, 0, 0, 0, 2000.000000, ""); mE(b_223, 40000.000000); UsedBlocks->Add(b_223);
	auto b_224 = make1(1100, -78, 26, 43, 10, 10, 2, 0, 0, 0, 2000.000000, ""); mE(b_224, 40000.000000); UsedBlocks->Add(b_224);
	auto b_225 = make1(1100, -78, 16, 43, 10, 10, 2, 0, 0, 0, 2000.000000, ""); mE(b_225, 40000.000000); UsedBlocks->Add(b_225);
	auto b_226 = make1(1100, -67, 16, 43, 10, 10, 2, 0, 90, 0, 2000.000000, ""); mE(b_226, 40000.000000); UsedBlocks->Add(b_226);
	auto b_227 = make1(1000, -63, 12, 41, 1, 1, 1, 0, 90, -180, 10.000000, "strop"); mE(b_227, 2000.000000); mEP(b_227, 1, 1, 1.000000);
	mr(b_227, "B0BC32824087429BA1C952AE641E7F3D"); mrc(b_227, "871552E149B983151EB42EBC88527631", 0); UsedBlocks->Add(b_227);
	auto b_228 = make1(1000, -63, 31, 41, 1, 1, 1, 0, 90, -180, 10.000000, "strop"); mE(b_228, 2000.000000); mEP(b_228, 1, 1, 1.000000);
	mr(b_228, "431C7BAD41EA4C6DA85C379BDD646A74"); mrc(b_228, "871552E149B983151EB42EBC88527631", 0); UsedBlocks->Add(b_228);
	auto b_229 = make1(1000, -92, 41, 39, 1, 1, 1, 0, 90, -180, 10.000000, "strop"); mE(b_229, 2000.000000); mEP(b_229, 1, 1, 1.000000);
	mr(b_229, "CAB2FBFD4C2046E342B017A681481D0A"); mrc(b_229, "871552E149B983151EB42EBC88527631", 0); UsedBlocks->Add(b_229);
	auto b_230 = make1(1000, -82, 31, 41, 1, 1, 1, 0, 90, -180, 10.000000, "strop"); mE(b_230, 2000.000000); mEP(b_230, 1, 1, 1.000000);
	mr(b_230, "630F294F4C748242957A129767F1D485"); mrc(b_230, "871552E149B983151EB42EBC88527631", 0); UsedBlocks->Add(b_230);
	auto b_231 = make1(1000, -82, 12, 41, 1, 1, 1, 0, 90, -180, 10.000000, "strop"); mE(b_231, 2000.000000); mEP(b_231, 1, 1, 1.000000);
	mr(b_231, "F8A8D3244F650134FD8D70B4B48B6A70"); mrc(b_231, "871552E149B983151EB42EBC88527631", 0); UsedBlocks->Add(b_231);
	auto b_232 = make1(1000, -72, 22, 42, 1, 1, 1, 0, 180, 180, 10.000000, "strop"); mE(b_232, 2000.000000); mEP(b_232, 1, 1, 1.000000);
	mr(b_232, "09C0BA7F44B88E7CA70D419348A4BC1B"); mrc(b_232, "871552E149B983151EB42EBC88527631", 0); UsedBlocks->Add(b_232);
	auto b_233 = make1(1000, -72, 21, 42, 1, 1, 1, 0, 180, 180, 10.000000, "strop"); mE(b_233, 2000.000000); mEP(b_233, 1, 1, 1.000000);
	mr(b_233, "FA7CE1054761F31C086EB2B7C5672308"); mrc(b_233, "871552E149B983151EB42EBC88527631", 0); UsedBlocks->Add(b_233);
	auto b_234 = make1(1000, -73, 22, 42, 1, 1, 1, 0, 180, 180, 10.000000, "strop"); mE(b_234, 2000.000000); mEP(b_234, 1, 1, 1.000000);
	mr(b_234, "B242F1E04752F4F26726F59D5C6FFB09"); mrc(b_234, "871552E149B983151EB42EBC88527631", 0); UsedBlocks->Add(b_234);
	auto b_235 = make1(1000, -73, 21, 42, 1, 1, 1, 0, 180, 180, 10.000000, "strop"); mE(b_235, 2000.000000); mEP(b_235, 1, 1, 1.000000);
	mr(b_235, "A10C533E4A180A1722B5A382117118B7"); mrc(b_235, "871552E149B983151EB42EBC88527631", 0); UsedBlocks->Add(b_235);
	auto b_236 = make1(1000, -53, 2, 39, 1, 1, 1, 0, 90, 180, 10.000000, "strop"); mE(b_236, 2000.000000); mEP(b_236, 1, 1, 1.000000);
	mr(b_236, "226D74BB44A9A12FEF7533B424DE5053"); mrc(b_236, "871552E149B983151EB42EBC88527631", 0); UsedBlocks->Add(b_236);
	auto b_237 = make1(1000, -53, 41, 39, 1, 1, 1, 0, 90, 180, 10.000000, "strop"); mE(b_237, 2000.000000); mEP(b_237, 1, 1, 1.000000);
	mr(b_237, "15E2F33A42541A566DAF28BF86C2F5D8"); mrc(b_237, "871552E149B983151EB42EBC88527631", 0); UsedBlocks->Add(b_237);
	auto b_238 = make1(1000, -92, 2, 39, 1, 1, 1, 0, 90, 180, 10.000000, "strop"); mE(b_238, 2000.000000); mEP(b_238, 1, 1, 1.000000);
	mr(b_238, "7F959AAF4B8947A6C06D129D02F9653D"); mrc(b_238, "871552E149B983151EB42EBC88527631", 0); UsedBlocks->Add(b_238);
	auto b_239 = make1(1000, -101, 50, 38, 1, 1, 1, 0, -90, -180, 10.000000, "strop"); mE(b_239, 2000.000000); mEP(b_239, 1, 1, 1.000000);
	mr(b_239, "E21F297E4635EB0057BBDA8BFBB133F9"); mrc(b_239, "871552E149B983151EB42EBC88527631", 0); UsedBlocks->Add(b_239);
	auto b_240 = make1(1000, -44, -7, 38, 1, 1, 1, 0, 0, -180, 10.000000, "strop"); mE(b_240, 2000.000000); mEP(b_240, 1, 1, 1.000000);
	mr(b_240, "FA89803843A59D1CB009E498ACCC17AF"); mrc(b_240, "871552E149B983151EB42EBC88527631", 0); UsedBlocks->Add(b_240);
	auto b_241 = make1(1050, -58, -7, 11, 1, 1, 1, 0, 0, 90, 10.000000, "strop"); bd(b_241, TEXT("ReactsToDayCycle"), TEXT("1")); bd(b_241, TEXT("StateAtDay"), TEXT("0")); bd(b_241, TEXT("StateAtNight"), TEXT("1")); mE(b_241, 0.000000); mEP(b_241, 1, 1, 1.000000);
	mr(b_241, "871552E149B983151EB42EBC88527631"); mrc(b_241, "FA89803843A59D1CB009E498ACCC17AF", 1); mrc(b_241, "E21F297E4635EB0057BBDA8BFBB133F9", 1); mrc(b_241, "5DE934F943CBE06F41B5A9A569B693B7", 1); mrc(b_241, "58800FBA4BB3478DB03516ABA5B58CB8", 1); mrc(b_241, "CAB2FBFD4C2046E342B017A681481D0A", 1);
	mrc(b_241, "15E2F33A42541A566DAF28BF86C2F5D8", 1); mrc(b_241, "226D74BB44A9A12FEF7533B424DE5053", 1); mrc(b_241, "7F959AAF4B8947A6C06D129D02F9653D", 1); mrc(b_241, "B0BC32824087429BA1C952AE641E7F3D", 1); mrc(b_241, "F8A8D3244F650134FD8D70B4B48B6A70", 1);
	mrc(b_241, "630F294F4C748242957A129767F1D485", 1); mrc(b_241, "431C7BAD41EA4C6DA85C379BDD646A74", 1); mrc(b_241, "A10C533E4A180A1722B5A382117118B7", 1); mrc(b_241, "B242F1E04752F4F26726F59D5C6FFB09", 1); mrc(b_241, "FA7CE1054761F31C086EB2B7C5672308", 1);
	mrc(b_241, "09C0BA7F44B88E7CA70D419348A4BC1B", 1); UsedBlocks->Add(b_241);
	auto b_242 = make1(1000, -102, 51, 38, 1, 1, 1, 0, 90, -180, 10.000000, ""); mE(b_242, 2000.000000); mEP(b_242, 1, 1, 1.000000);
	mr(b_242, "9E0600C44D0E6CA563534BBAFFF02B15"); mrc(b_242, "409F8BAE4B0DDDC12F8E92A974B21DCF", 0); UsedBlocks->Add(b_242);
	auto b_243 = make1(1050, -56, -7, 11, 1, 1, 1, 0, 0, 90, 10.000000, "venku"); bd(b_243, TEXT("ReactsToDayCycle"), TEXT("1")); bd(b_243, TEXT("StateAtDay"), TEXT("0")); bd(b_243, TEXT("StateAtNight"), TEXT("1")); mE(b_243, 0.000000); mEP(b_243, 1, 1, 1.000000);
	mr(b_243, "409F8BAE4B0DDDC12F8E92A974B21DCF"); mrc(b_243, "8391987A4F47E8B8CED0AF806E83238C", 1); mrc(b_243, "9E0600C44D0E6CA563534BBAFFF02B15", 1); mrc(b_243, "5725DC8E4860A9E16F1FC0A28A7DD7B8", 1); mrc(b_243, "D5A1945F47EC757D41C7DF9440CA09CC", 1); mrc(b_243, "40B54F9949B401D1BB8007B104A896E7", 1);
	mrc(b_243, "F630E3294352487131950F9CF6CFD772", 1); mrc(b_243, "5466012F48745AB130212294A94276DC", 1); mrc(b_243, "7D26F2134D6E906B70C01EA5D4FFF34D", 1); mrc(b_243, "C219CF2348901785535E3D96003DE0A9", 1); mrc(b_243, "11ED8BEE4DFA99EE096FCCAE702C58BA", 1);
	mrc(b_243, "9C926C8C40782D161292889F05820CC9", 1); mrc(b_243, "36ACA6D5404A6F6701E976B255CF5415", 1); mrc(b_243, "CA7C8EE044CF5940EDFAF5AF1E0F70B5", 1); mrc(b_243, "90CF4EAA4385DE9368821B9B0124A37F", 1); mrc(b_243, "75202C82439F58D5B7AF9AA470EDB8DA", 1);
	mrc(b_243, "010F6056495DF42C202EC9B9516F80B0", 1); UsedBlocks->Add(b_243);
	auto b_244 = make1(1000, -61, -7, 21, 1, 1, 1, 0, 0, 90, 10.000000, ""); mE(b_244, 2000.000000); mEP(b_244, 0, 1, 1.000000);
	mr(b_244, "9F0344484B2E6236B25078935BA0A543"); mrc(b_244, "4C31F7884C556D01A12D6AB691795A01", 0); UsedBlocks->Add(b_244);
	auto b_245 = make1(1000, -101, 33, 21, 1, 1, 1, 0, -90, 90, 10.000000, ""); mE(b_245, 2000.000000); mEP(b_245, 0, 1, 1.000000);
	mr(b_245, "7D9248FB47CF230DDF3C45BC21E70028"); mrc(b_245, "4C31F7884C556D01A12D6AB691795A01", 0); UsedBlocks->Add(b_245);
	auto b_246 = make1(1000, -101, 10, 21, 1, 1, 1, 0, -90, 90, 10.000000, ""); mE(b_246, 2000.000000); mEP(b_246, 0, 1, 1.000000);
	mr(b_246, "CE4F34C7481B56427A68DB8026C5702C"); mrc(b_246, "4C31F7884C556D01A12D6AB691795A01", 0); UsedBlocks->Add(b_246);
	auto b_247 = make1(1000, -84, 50, 21, 1, 1, 1, -90, 0, -90, 10.000000, ""); mE(b_247, 2000.000000); mEP(b_247, 0, 1, 1.000000);
	mr(b_247, "9AB54FBD4442086CD94F008761FCEEFC"); mrc(b_247, "4C31F7884C556D01A12D6AB691795A01", 0); UsedBlocks->Add(b_247);
	auto b_248 = make1(1000, -61, 50, 21, 1, 1, 1, -90, 0, -90, 10.000000, ""); mE(b_248, 2000.000000); mEP(b_248, 0, 1, 1.000000);
	mr(b_248, "84E8E1BB41FDE0DA834C2FBE6239E508"); mrc(b_248, "4C31F7884C556D01A12D6AB691795A01", 0); UsedBlocks->Add(b_248);
	auto b_249 = make1(1000, -44, 33, 21, 1, 1, 1, -90, 0, -180, 10.000000, ""); mE(b_249, 2000.000000); mEP(b_249, 0, 1, 1.000000);
	mr(b_249, "F7C689CE46C6AB1896B773BE9E02F454"); mrc(b_249, "4C31F7884C556D01A12D6AB691795A01", 0); UsedBlocks->Add(b_249);
	auto b_250 = make1(1000, -44, 10, 21, 1, 1, 1, -90, 0, -180, 10.000000, ""); mE(b_250, 2000.000000); mEP(b_250, 0, 1, 1.000000);
	mr(b_250, "45BD2AA3450D3011ED1F53AD45B9D5A2"); mrc(b_250, "4C31F7884C556D01A12D6AB691795A01", 0); UsedBlocks->Add(b_250);
	auto b_251 = make1(1050, -54, -7, 11, 1, 1, 1, 0, 0, 90, 10.000000, "uvnitr"); bd(b_251, TEXT("ReactsToDayCycle"), TEXT("1")); bd(b_251, TEXT("StateAtDay"), TEXT("1")); bd(b_251, TEXT("StateAtNight"), TEXT("0")); mE(b_251, 0.000000); mEP(b_251, 0, 1, 1.000000);
	mr(b_251, "4C31F7884C556D01A12D6AB691795A01"); mrc(b_251, "9F0344484B2E6236B25078935BA0A543", 1); mrc(b_251, "45BD2AA3450D3011ED1F53AD45B9D5A2", 1); mrc(b_251, "8C4593B3471E366EAF8028A35D9B60F8", 1); mrc(b_251, "CE4F34C7481B56427A68DB8026C5702C", 1); mrc(b_251, "7D9248FB47CF230DDF3C45BC21E70028", 1);
	mrc(b_251, "9AB54FBD4442086CD94F008761FCEEFC", 1); mrc(b_251, "84E8E1BB41FDE0DA834C2FBE6239E508", 1); mrc(b_251, "F7C689CE46C6AB1896B773BE9E02F454", 1); mrc(b_251, "2C3EDE374443338369117CB5D7D553D9", 1); UsedBlocks->Add(b_251);
	auto b_252 = make1(1200, -45, 49, 5, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_252, 100000.000000); UsedBlocks->Add(b_252);
	auto b_253 = make1(1200, -48, 49, 5, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_253, 100000.000000); UsedBlocks->Add(b_253);
	auto b_254 = make1(1200, -51, 49, 5, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_254, 100000.000000); UsedBlocks->Add(b_254);
	auto b_255 = make1(1200, -54, 49, 5, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_255, 100000.000000); UsedBlocks->Add(b_255);
	auto b_256 = make1(1200, -57, 49, 5, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_256, 100000.000000); UsedBlocks->Add(b_256);
	auto b_257 = make1(1200, -60, 49, 5, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_257, 100000.000000); UsedBlocks->Add(b_257);
	auto b_258 = make1(1200, -60, 49, 8, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_258, 100000.000000); UsedBlocks->Add(b_258);
	auto b_259 = make1(1200, -57, 49, 8, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_259, 100000.000000); UsedBlocks->Add(b_259);
	auto b_260 = make1(1200, -54, 49, 8, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_260, 100000.000000); UsedBlocks->Add(b_260);
	auto b_261 = make1(1200, -51, 49, 8, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_261, 100000.000000); UsedBlocks->Add(b_261);
	auto b_262 = make1(1200, -48, 49, 8, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_262, 100000.000000); UsedBlocks->Add(b_262);
	auto b_263 = make1(1200, -45, 49, 8, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_263, 100000.000000); UsedBlocks->Add(b_263);
	auto b_264 = make1(1200, -45, 49, 11, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_264, 100000.000000); UsedBlocks->Add(b_264);
	auto b_265 = make1(1200, -48, 49, 11, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_265, 100000.000000); UsedBlocks->Add(b_265);
	auto b_266 = make1(1200, -51, 49, 11, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_266, 100000.000000); UsedBlocks->Add(b_266);
	auto b_267 = make1(1200, -54, 49, 11, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_267, 100000.000000); UsedBlocks->Add(b_267);
	auto b_268 = make1(1200, -57, 49, 11, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_268, 100000.000000); UsedBlocks->Add(b_268);
	auto b_269 = make1(1200, -60, 49, 11, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_269, 100000.000000); UsedBlocks->Add(b_269);
	auto b_270 = make1(1200, -60, 49, 14, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_270, 100000.000000); UsedBlocks->Add(b_270);
	auto b_271 = make1(1200, -57, 49, 14, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_271, 100000.000000); UsedBlocks->Add(b_271);
	auto b_272 = make1(1200, -54, 49, 14, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_272, 100000.000000); UsedBlocks->Add(b_272);
	auto b_273 = make1(1200, -51, 49, 14, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_273, 100000.000000); UsedBlocks->Add(b_273);
	auto b_274 = make1(1200, -48, 49, 14, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_274, 100000.000000); UsedBlocks->Add(b_274);
	auto b_275 = make1(1200, -45, 49, 14, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_275, 100000.000000); UsedBlocks->Add(b_275);
	auto b_276 = make1(1200, -45, 49, 17, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_276, 100000.000000); UsedBlocks->Add(b_276);
	auto b_277 = make1(1200, -48, 49, 17, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_277, 100000.000000); UsedBlocks->Add(b_277);
	auto b_278 = make1(1200, -51, 49, 17, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_278, 100000.000000); UsedBlocks->Add(b_278);
	auto b_279 = make1(1200, -54, 49, 17, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_279, 100000.000000); UsedBlocks->Add(b_279);
	auto b_280 = make1(1200, -57, 49, 17, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_280, 100000.000000); UsedBlocks->Add(b_280);
	auto b_281 = make1(1200, -60, 49, 17, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_281, 100000.000000); UsedBlocks->Add(b_281);
	auto b_282 = make1(1200, -45, 46, 5, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_282, 100000.000000); UsedBlocks->Add(b_282);
	auto b_283 = make1(1200, -45, 46, 8, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_283, 100000.000000); UsedBlocks->Add(b_283);
	auto b_284 = make1(1200, -45, 46, 11, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_284, 100000.000000); UsedBlocks->Add(b_284);
	auto b_285 = make1(1200, -45, 46, 14, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_285, 100000.000000); UsedBlocks->Add(b_285);
	auto b_286 = make1(1200, -45, 46, 17, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_286, 100000.000000); UsedBlocks->Add(b_286);
	auto b_287 = make1(1200, -45, 43, 5, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_287, 100000.000000); UsedBlocks->Add(b_287);
	auto b_288 = make1(1200, -45, 43, 8, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_288, 100000.000000); UsedBlocks->Add(b_288);
	auto b_289 = make1(1200, -45, 43, 11, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_289, 100000.000000); UsedBlocks->Add(b_289);
	auto b_290 = make1(1200, -45, 43, 14, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_290, 100000.000000); UsedBlocks->Add(b_290);
	auto b_291 = make1(1200, -45, 43, 17, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_291, 100000.000000); UsedBlocks->Add(b_291);
	auto b_292 = make1(1200, -45, 40, 5, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_292, 100000.000000); UsedBlocks->Add(b_292);
	auto b_293 = make1(1200, -45, 40, 8, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_293, 100000.000000); UsedBlocks->Add(b_293);
	auto b_294 = make1(1200, -45, 40, 11, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_294, 100000.000000); UsedBlocks->Add(b_294);
	auto b_295 = make1(1200, -45, 40, 14, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_295, 100000.000000); UsedBlocks->Add(b_295);
	auto b_296 = make1(1200, -45, 40, 17, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_296, 100000.000000); UsedBlocks->Add(b_296);
	auto b_297 = make1(1200, -45, 37, 5, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_297, 100000.000000); UsedBlocks->Add(b_297);
	auto b_298 = make1(1200, -45, 37, 8, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_298, 100000.000000); UsedBlocks->Add(b_298);
	auto b_299 = make1(1200, -45, 37, 11, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_299, 100000.000000); UsedBlocks->Add(b_299);
	auto b_300 = make1(1200, -45, 37, 14, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_300, 100000.000000); UsedBlocks->Add(b_300);
	auto b_301 = make1(1200, -45, 37, 17, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_301, 100000.000000); UsedBlocks->Add(b_301);
	auto b_302 = make1(1200, -45, 34, 5, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_302, 100000.000000); UsedBlocks->Add(b_302);
	auto b_303 = make1(1200, -45, 34, 8, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_303, 100000.000000); UsedBlocks->Add(b_303);
	auto b_304 = make1(1200, -45, 34, 11, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_304, 100000.000000); UsedBlocks->Add(b_304);
	auto b_305 = make1(1200, -45, 34, 14, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_305, 100000.000000); UsedBlocks->Add(b_305);
	auto b_306 = make1(1200, -45, 34, 17, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_306, 100000.000000); UsedBlocks->Add(b_306);
	auto b_307 = make1(1500, -100, 50, 4, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_307, 1000.000000); UsedBlocks->Add(b_307);
	auto b_308 = make1(1500, -100, 50, 6, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_308, 1000.000000); UsedBlocks->Add(b_308);
	auto b_309 = make1(1500, -100, 50, 8, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_309, 1000.000000); UsedBlocks->Add(b_309);
	auto b_310 = make1(1500, -100, 50, 10, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_310, 1000.000000); UsedBlocks->Add(b_310);
	auto b_311 = make1(1500, -100, 49, 12, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_311, 1000.000000); UsedBlocks->Add(b_311);
	auto b_312 = make1(1500, -100, 49, 14, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_312, 1000.000000); UsedBlocks->Add(b_312);
	auto b_313 = make1(1500, -100, 49, 16, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_313, 1000.000000); UsedBlocks->Add(b_313);
	auto b_314 = make1(1500, -100, 47, 4, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_314, 1000.000000); UsedBlocks->Add(b_314);
	auto b_315 = make1(1500, -100, 47, 6, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_315, 1000.000000); UsedBlocks->Add(b_315);
	auto b_316 = make1(1500, -100, 47, 8, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_316, 1000.000000); UsedBlocks->Add(b_316);
	auto b_317 = make1(1500, -100, 47, 10, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_317, 1000.000000); UsedBlocks->Add(b_317);
	auto b_318 = make1(1500, -100, 47, 12, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_318, 1000.000000); UsedBlocks->Add(b_318);
	auto b_319 = make1(1500, -100, 47, 14, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_319, 1000.000000); UsedBlocks->Add(b_319);
	auto b_320 = make1(1500, -100, 47, 16, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_320, 1000.000000); UsedBlocks->Add(b_320);
	auto b_321 = make1(1500, -100, 45, 4, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_321, 1000.000000); UsedBlocks->Add(b_321);
	auto b_322 = make1(1500, -100, 43, 4, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_322, 1000.000000); UsedBlocks->Add(b_322);
	auto b_323 = make1(1500, -100, 41, 4, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_323, 1000.000000); UsedBlocks->Add(b_323);
	auto b_324 = make1(1500, -100, 39, 4, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_324, 1000.000000); UsedBlocks->Add(b_324);
	auto b_325 = make1(1500, -100, 37, 4, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_325, 1000.000000); UsedBlocks->Add(b_325);
	auto b_326 = make1(1500, -100, 35, 4, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_326, 1000.000000); UsedBlocks->Add(b_326);
	auto b_327 = make1(1500, -100, 33, 4, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_327, 1000.000000); UsedBlocks->Add(b_327);
	auto b_328 = make1(1500, -100, 33, 6, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_328, 1000.000000); UsedBlocks->Add(b_328);
	auto b_329 = make1(1500, -100, 33, 8, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_329, 1000.000000); UsedBlocks->Add(b_329);
	auto b_330 = make1(1500, -100, 33, 10, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_330, 1000.000000); UsedBlocks->Add(b_330);
	auto b_331 = make1(1500, -100, 33, 12, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_331, 1000.000000); UsedBlocks->Add(b_331);
	auto b_332 = make1(1500, -100, 33, 14, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_332, 1000.000000); UsedBlocks->Add(b_332);
	auto b_333 = make1(1500, -100, 33, 16, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_333, 1000.000000); UsedBlocks->Add(b_333);
	auto b_334 = make1(1500, -100, 45, 16, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_334, 1000.000000); UsedBlocks->Add(b_334);
	auto b_335 = make1(1500, -100, 43, 16, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_335, 1000.000000); UsedBlocks->Add(b_335);
	auto b_336 = make1(1500, -100, 41, 16, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_336, 1000.000000); UsedBlocks->Add(b_336);
	auto b_337 = make1(1500, -100, 39, 16, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_337, 1000.000000); UsedBlocks->Add(b_337);
	auto b_338 = make1(1500, -100, 37, 16, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_338, 1000.000000); UsedBlocks->Add(b_338);
	auto b_339 = make1(1500, -100, 35, 16, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_339, 1000.000000); UsedBlocks->Add(b_339);
	auto b_340 = make1(1500, -100, 35, 14, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_340, 1000.000000); UsedBlocks->Add(b_340);
	auto b_341 = make1(1500, -100, 37, 14, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_341, 1000.000000); UsedBlocks->Add(b_341);
	auto b_342 = make1(1500, -100, 39, 14, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_342, 1000.000000); UsedBlocks->Add(b_342);
	auto b_343 = make1(1500, -100, 42, 14, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_343, 1000.000000); UsedBlocks->Add(b_343);
	auto b_344 = make1(1500, -100, 44, 14, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_344, 1000.000000); UsedBlocks->Add(b_344);
	auto b_345 = make1(1500, -100, 46, 14, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_345, 1000.000000); UsedBlocks->Add(b_345);
	auto b_346 = make1(1500, -100, 46, 12, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_346, 1000.000000); UsedBlocks->Add(b_346);
	auto b_347 = make1(1500, -100, 44, 12, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_347, 1000.000000); UsedBlocks->Add(b_347);
	auto b_348 = make1(1500, -100, 42, 12, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_348, 1000.000000); UsedBlocks->Add(b_348);
	auto b_349 = make1(1500, -100, 40, 12, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_349, 1000.000000); UsedBlocks->Add(b_349);
	auto b_350 = make1(1500, -100, 38, 12, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_350, 1000.000000); UsedBlocks->Add(b_350);
	auto b_351 = make1(1500, -100, 36, 12, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_351, 1000.000000); UsedBlocks->Add(b_351);
	auto b_352 = make1(1500, -100, 36, 10, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_352, 1000.000000); UsedBlocks->Add(b_352);
	auto b_353 = make1(1500, -100, 36, 8, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_353, 1000.000000); UsedBlocks->Add(b_353);
	auto b_354 = make1(1500, -100, 36, 6, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_354, 1000.000000); UsedBlocks->Add(b_354);
	auto b_355 = make1(1500, -100, 38, 10, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_355, 1000.000000); UsedBlocks->Add(b_355);
	auto b_356 = make1(1500, -100, 38, 8, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_356, 1000.000000); UsedBlocks->Add(b_356);
	auto b_357 = make1(1500, -100, 38, 6, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_357, 1000.000000); UsedBlocks->Add(b_357);
	auto b_358 = make1(1500, -100, 40, 10, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_358, 1000.000000); UsedBlocks->Add(b_358);
	auto b_359 = make1(1500, -100, 40, 8, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_359, 1000.000000); UsedBlocks->Add(b_359);
	auto b_360 = make1(1500, -100, 40, 6, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_360, 1000.000000); UsedBlocks->Add(b_360);
	auto b_361 = make1(1500, -100, 42, 10, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_361, 1000.000000); UsedBlocks->Add(b_361);
	auto b_362 = make1(1500, -100, 42, 8, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_362, 1000.000000); UsedBlocks->Add(b_362);
	auto b_363 = make1(1500, -100, 42, 6, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_363, 1000.000000); UsedBlocks->Add(b_363);
	auto b_364 = make1(1500, -100, 44, 10, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_364, 1000.000000); UsedBlocks->Add(b_364);
	auto b_365 = make1(1500, -100, 44, 8, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_365, 1000.000000); UsedBlocks->Add(b_365);
	auto b_366 = make1(1500, -100, 44, 6, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_366, 1000.000000); UsedBlocks->Add(b_366);
	auto b_367 = make1(1500, -100, 46, 10, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_367, 1000.000000); UsedBlocks->Add(b_367);
	auto b_368 = make1(1500, -100, 46, 8, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_368, 1000.000000); UsedBlocks->Add(b_368);
	auto b_369 = make1(1500, -100, 46, 6, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_369, 1000.000000); UsedBlocks->Add(b_369);
	auto b_370 = make1(1500, -98, 50, 4, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_370, 1000.000000); UsedBlocks->Add(b_370);
	auto b_371 = make1(1500, -98, 50, 6, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_371, 1000.000000); UsedBlocks->Add(b_371);
	auto b_372 = make1(1500, -98, 50, 16, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_372, 1000.000000); UsedBlocks->Add(b_372);
	auto b_373 = make1(1500, -96, 50, 16, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_373, 1000.000000); UsedBlocks->Add(b_373);
	auto b_374 = make1(1500, -94, 50, 16, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_374, 1000.000000); UsedBlocks->Add(b_374);
	auto b_375 = make1(1500, -92, 50, 16, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_375, 1000.000000); UsedBlocks->Add(b_375);
	auto b_376 = make1(1500, -90, 50, 16, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_376, 1000.000000); UsedBlocks->Add(b_376);
	auto b_377 = make1(1500, -88, 50, 16, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_377, 1000.000000); UsedBlocks->Add(b_377);
	auto b_378 = make1(1500, -86, 50, 16, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_378, 1000.000000); UsedBlocks->Add(b_378);
	auto b_379 = make1(1500, -84, 50, 16, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_379, 1000.000000); UsedBlocks->Add(b_379);
	auto b_380 = make1(1500, -84, 50, 14, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_380, 1000.000000); UsedBlocks->Add(b_380);
	auto b_381 = make1(1500, -84, 50, 12, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_381, 1000.000000); UsedBlocks->Add(b_381);
	auto b_382 = make1(1500, -84, 50, 10, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_382, 1000.000000); UsedBlocks->Add(b_382);
	auto b_383 = make1(1500, -84, 50, 8, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_383, 1000.000000); UsedBlocks->Add(b_383);
	auto b_384 = make1(1500, -84, 50, 6, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_384, 1000.000000); UsedBlocks->Add(b_384);
	auto b_385 = make1(1500, -84, 50, 4, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_385, 1000.000000); UsedBlocks->Add(b_385);
	auto b_386 = make1(1500, -86, 50, 4, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_386, 1000.000000); UsedBlocks->Add(b_386);
	auto b_387 = make1(1500, -88, 50, 4, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_387, 1000.000000); UsedBlocks->Add(b_387);
	auto b_388 = make1(1500, -90, 50, 4, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_388, 1000.000000); UsedBlocks->Add(b_388);
	auto b_389 = make1(1500, -92, 50, 4, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_389, 1000.000000); UsedBlocks->Add(b_389);
	auto b_390 = make1(1500, -94, 50, 4, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_390, 1000.000000); UsedBlocks->Add(b_390);
	auto b_391 = make1(1500, -96, 50, 4, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_391, 1000.000000); UsedBlocks->Add(b_391);
	auto b_392 = make1(1500, -86, 50, 6, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_392, 1000.000000); UsedBlocks->Add(b_392);
	auto b_393 = make1(1500, -86, 50, 8, 2, 2, 2, 0, -180, 0, 10.000000, ""); mO(b_393, 1000.000000); UsedBlocks->Add(b_393);
	auto b_394 = make1(1500, -86, 49, 10, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_394, 1000.000000); UsedBlocks->Add(b_394);
	auto b_395 = make1(1500, -86, 49, 12, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_395, 1000.000000); UsedBlocks->Add(b_395);
	auto b_396 = make1(1500, -86, 49, 14, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_396, 1000.000000); UsedBlocks->Add(b_396);
	auto b_397 = make1(1500, -88, 49, 14, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_397, 1000.000000); UsedBlocks->Add(b_397);
	auto b_398 = make1(1500, -90, 49, 14, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_398, 1000.000000); UsedBlocks->Add(b_398);
	auto b_399 = make1(1500, -92, 49, 14, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_399, 1000.000000); UsedBlocks->Add(b_399);
	auto b_400 = make1(1500, -94, 49, 14, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_400, 1000.000000); UsedBlocks->Add(b_400);
	auto b_401 = make1(1500, -96, 49, 14, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_401, 1000.000000); UsedBlocks->Add(b_401);
	auto b_402 = make1(1500, -98, 49, 14, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_402, 1000.000000); UsedBlocks->Add(b_402);
	auto b_403 = make1(1500, -88, 49, 12, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_403, 1000.000000); UsedBlocks->Add(b_403);
	auto b_404 = make1(1500, -90, 49, 12, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_404, 1000.000000); UsedBlocks->Add(b_404);
	auto b_405 = make1(1500, -92, 49, 12, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_405, 1000.000000); UsedBlocks->Add(b_405);
	auto b_406 = make1(1500, -94, 49, 12, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_406, 1000.000000); UsedBlocks->Add(b_406);
	auto b_407 = make1(1500, -98, 49, 12, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_407, 1000.000000); UsedBlocks->Add(b_407);
	auto b_408 = make1(1500, -96, 49, 12, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_408, 1000.000000); UsedBlocks->Add(b_408);
	auto b_409 = make1(1500, -98, 49, 10, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_409, 1000.000000); UsedBlocks->Add(b_409);
	auto b_410 = make1(1500, -96, 49, 10, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_410, 1000.000000); UsedBlocks->Add(b_410);
	auto b_411 = make1(1500, -94, 49, 10, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_411, 1000.000000); UsedBlocks->Add(b_411);
	auto b_412 = make1(1500, -92, 49, 10, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_412, 1000.000000); UsedBlocks->Add(b_412);
	auto b_413 = make1(1500, -90, 49, 10, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_413, 1000.000000); UsedBlocks->Add(b_413);
	auto b_414 = make1(1500, -88, 49, 10, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_414, 1000.000000); UsedBlocks->Add(b_414);
	auto b_415 = make1(1500, -88, 49, 8, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_415, 1000.000000); UsedBlocks->Add(b_415);
	auto b_416 = make1(1500, -90, 49, 8, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_416, 1000.000000); UsedBlocks->Add(b_416);
	auto b_417 = make1(1500, -92, 49, 8, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_417, 1000.000000); UsedBlocks->Add(b_417);
	auto b_418 = make1(1500, -94, 49, 8, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_418, 1000.000000); UsedBlocks->Add(b_418);
	auto b_419 = make1(1500, -96, 49, 8, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_419, 1000.000000); UsedBlocks->Add(b_419);
	auto b_420 = make1(1500, -98, 49, 8, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_420, 1000.000000); UsedBlocks->Add(b_420);
	auto b_421 = make1(1500, -96, 49, 6, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_421, 1000.000000); UsedBlocks->Add(b_421);
	auto b_422 = make1(1500, -94, 49, 6, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_422, 1000.000000); UsedBlocks->Add(b_422);
	auto b_423 = make1(1500, -92, 49, 6, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_423, 1000.000000); UsedBlocks->Add(b_423);
	auto b_424 = make1(1500, -90, 49, 6, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_424, 1000.000000); UsedBlocks->Add(b_424);
	auto b_425 = make1(1500, -88, 49, 6, 2, 2, 2, 0, 90, 0, 10.000000, ""); mO(b_425, 1000.000000); UsedBlocks->Add(b_425);
	auto b_426 = make1(1, -46, 4, 9, 8, 4, 1, 0, -90, 0, 320.000000, ""); mE(b_426, 0.000000); UsedBlocks->Add(b_426);
	auto b_427 = make1(500, -46, 3, 12, 1, 8, 5, 0, 0, 0, 10.000000, ""); mE(b_427, 100000.000000); UsedBlocks->Add(b_427);
	auto b_428 = make1(1, -47, 0, 6, 1, 1, 5, 0, 90, 0, 50.000000, ""); mE(b_428, 0.000000); UsedBlocks->Add(b_428);
	auto b_429 = make1(1, -47, 7, 6, 1, 1, 5, 0, 90, 0, 50.000000, ""); mE(b_429, 0.000000); UsedBlocks->Add(b_429);
	auto b_430 = make1(1, -94, 5, 25, 12, 1, 4, 0, 90, 0, 480.000000, ""); mE(b_430, 0.000000); UsedBlocks->Add(b_430);
	auto b_431 = make1(101, -88, 6, 27, 1, 10, 10, -90, 0, 0, 333.333374, ""); mE(b_431, 0.000000); UsedBlocks->Add(b_431);
	auto b_432 = make1(600, -102, 22, 33, 20, 1, 11, 0, -90, 0, 2200.000000, ""); mE(b_432, 88000.000000); UsedBlocks->Add(b_432);
	auto b_433 = make1(600, -72, 51, 33, 20, 1, 11, 0, -180, 0, 2200.000000, ""); mE(b_433, 88000.000000); UsedBlocks->Add(b_433);
	auto b_434 = make1(600, -43, 21, 33, 20, 1, 11, 0, 90, 0, 2200.000000, ""); mE(b_434, 88000.000000); UsedBlocks->Add(b_434);
	auto b_435 = make1(600, -73, -8, 33, 20, 1, 11, 0, 0, 0, 2200.000000, ""); mE(b_435, 88000.000000); UsedBlocks->Add(b_435);
	auto b_436 = make1(2, -82, 42, 30, 20, 1, 5, 0, -180, 0, 1000.000000, ""); UsedBlocks->Add(b_436);
	auto b_437 = make1(2, -62, 42, 30, 20, 1, 5, 0, 180, 0, 1000.000000, ""); UsedBlocks->Add(b_437);
	auto b_438 = make1(2, -52, 31, 30, 20, 1, 5, 0, 90, 0, 1000.000000, ""); UsedBlocks->Add(b_438);
	auto b_439 = make1(2, -52, 11, 30, 20, 1, 5, 0, 90, 0, 1000.000000, ""); UsedBlocks->Add(b_439);
	auto b_440 = make1(2, -63, 1, 30, 20, 1, 5, 0, 0, 0, 1000.000000, ""); UsedBlocks->Add(b_440);
	auto b_441 = make1(2, -93, 31, 30, 20, 1, 5, 0, 90, 0, 1000.000000, ""); UsedBlocks->Add(b_441);
	auto b_442 = make1(2, -78, 1, 30, 10, 1, 5, 0, 0, 0, 500.000000, ""); UsedBlocks->Add(b_442);
	auto b_443 = make1(2, -93, 16, 30, 10, 1, 5, 0, 90, 0, 500.000000, ""); UsedBlocks->Add(b_443);
	auto b_444 = make1(2, -83, 2, 30, 1, 1, 5, 0, 0, 0, 50.000000, ""); UsedBlocks->Add(b_444);
	auto b_445 = make1(2, -84, 3, 30, 1, 1, 5, 0, 0, 0, 50.000000, ""); UsedBlocks->Add(b_445);
	auto b_446 = make1(2, -85, 4, 30, 1, 1, 5, 0, 0, 0, 50.000000, ""); UsedBlocks->Add(b_446);
	auto b_447 = make1(2, -86, 5, 30, 1, 1, 5, 0, 0, 0, 50.000000, ""); UsedBlocks->Add(b_447);
	auto b_448 = make1(2, -84, 12, 18, 1, 1, 5, 0, 0, 0, 50.000000, ""); UsedBlocks->Add(b_448);
	auto b_449 = make1(2, -87, 6, 30, 1, 1, 5, 0, 0, 0, 50.000000, ""); UsedBlocks->Add(b_449);
	auto b_450 = make1(2, -88, 7, 30, 1, 1, 5, 0, 0, 0, 50.000000, ""); UsedBlocks->Add(b_450);
	auto b_451 = make1(2, -89, 8, 30, 1, 1, 5, 0, 0, 0, 50.000000, ""); UsedBlocks->Add(b_451);
	auto b_452 = make1(2, -90, 9, 30, 1, 1, 5, 0, 0, 0, 50.000000, ""); UsedBlocks->Add(b_452);
	auto b_453 = make1(2, -91, 10, 30, 1, 1, 5, 0, 0, 0, 50.000000, ""); UsedBlocks->Add(b_453);
	auto b_454 = make1(2, -92, 11, 30, 1, 1, 5, 0, 0, 0, 50.000000, ""); UsedBlocks->Add(b_454);
	auto b_455 = make1(2, -93, -7, 30, 1, 1, 5, 0, -90, 0, 50.000000, ""); UsedBlocks->Add(b_455);
	auto b_456 = make1(2, -93, -6, 30, 1, 1, 5, 0, -90, 0, 50.000000, ""); UsedBlocks->Add(b_456);
	auto b_457 = make1(2, -93, -5, 30, 1, 1, 5, 0, -90, 0, 50.000000, ""); UsedBlocks->Add(b_457);
	auto b_458 = make1(2, -93, -4, 30, 1, 1, 5, 0, -90, 0, 50.000000, ""); UsedBlocks->Add(b_458);
	auto b_459 = make1(2, -93, -3, 30, 1, 1, 5, 0, -90, 0, 50.000000, ""); UsedBlocks->Add(b_459);
	auto b_460 = make1(2, -93, -2, 30, 1, 1, 5, 0, -90, 0, 50.000000, ""); UsedBlocks->Add(b_460);
	auto b_461 = make1(2, -93, -1, 30, 1, 1, 5, 0, -90, 0, 50.000000, ""); UsedBlocks->Add(b_461);
	auto b_462 = make1(2, -94, 0, 30, 1, 1, 5, 0, -90, 0, 50.000000, ""); UsedBlocks->Add(b_462);
	auto b_463 = make1(2, -94, 1, 30, 1, 1, 5, 0, -90, 0, 50.000000, ""); UsedBlocks->Add(b_463);
	auto b_464 = make1(2, -94, 2, 30, 1, 1, 5, 0, -90, 0, 50.000000, ""); UsedBlocks->Add(b_464);
	auto b_465 = make1(2, -94, 3, 30, 1, 1, 5, 0, -90, 0, 50.000000, ""); UsedBlocks->Add(b_465);
	auto b_466 = make1(2, -94, 4, 30, 1, 1, 5, 0, -90, 0, 50.000000, ""); UsedBlocks->Add(b_466);
	auto b_467 = make1(2, -76, -8, 9, 13, 1, 11, 0, 180, 0, 1430.000000, ""); UsedBlocks->Add(b_467);
	auto b_468 = make1(2, -43, 25, 9, 13, 1, 11, 0, 90, 0, 1430.000000, ""); UsedBlocks->Add(b_468);
	auto b_469 = make1(2, -69, 51, 9, 13, 1, 11, 0, 0, 0, 1430.000000, ""); UsedBlocks->Add(b_469);
	auto b_470 = make1(2, -102, 18, 9, 13, 1, 11, 0, -90, 0, 1430.000000, ""); UsedBlocks->Add(b_470);*/

	c->PlayerPosition = FVector(-1913, 310, 642);
	c->PlayerRotation = FRotator(0, 13, 0);
	c->PlayerCameraRotation = FRotator(343.147522, 13.125078, 0.0);

	return c;
}
#pragma endregion

USaveGameCarrier* UNewGameSaveHolder::getTutorialGameSave(bool full)
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemTutorial", "Tutorial").ToString();
	c->SaveFullDescription = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemTutorialFullDescr", "Nauč se hrát tuto hru a její základní principy.");
	c->FullFilePath = TEXT("_system_tutorial");
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::TutorialLevel;
	c->IsTutorial = true;


	if (!full)
		return c;

	c->CurrentTime = 36000;
	c->PlayerUseFPSCamera = true;
	c->PlayerHealth = 1000;
	c->PlayerOxygenComponent.CurrentObjectOxygen = 1000.0f;
	c->PlayerElectricityComponent.CurrentObjectEnergy = 500000.0f;

	c->inventoryTags = makeDefault();

	c->weatherState = makeTutorialWatherState();

	auto UsedBlocks = &c->usedBlocks;


	auto b_0 = make1(0, -6, -2, 1, 20, 20, 4, 0, 0, 0, 16000.000000, ""); mE(b_0, 0.000000); UsedBlocks->Add(b_0);
	auto b_1 = make1(0, 14, -22, 1, 20, 20, 4, 0, 0, 0, 16000.000000, ""); mE(b_1, 0.000000); UsedBlocks->Add(b_1);
	auto b_2 = make1(0, -6, -22, 1, 20, 20, 4, 0, 0, 0, 16000.000000, ""); mE(b_2, 0.000000); UsedBlocks->Add(b_2);
	auto b_3 = make1(600, -12, -39, 9, 6, 1, 11, 0, 0, 0, 660.000000, ""); mE(b_3, 26400.000000); UsedBlocks->Add(b_3);
	auto b_4 = make1(100, 10, -35, 1, 8, 14, 4, 0, -90, 0, 1493.333374, ""); mE(b_4, 0.000000); UsedBlocks->Add(b_4);
	auto b_5 = make1(0, -6, -35, 1, 19, 8, 4, 0, 180, 0, 6080.000000, ""); mE(b_5, 0.000000); UsedBlocks->Add(b_5);
	auto b_6 = make1(700, 0, -35, 9, 7, 7, 11, 0, 90, 0, 10.000000, ""); af(b_6, TEXT("DoorOpening"), 0); bd(b_6, TEXT("DoorState"), TEXT("0")); bd(b_6, TEXT("DoorYaw"), TEXT("0.0")); mE(b_6, 0.000000); UsedBlocks->Add(b_6);
	auto b_7 = make1(1, 3, -39, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_7, 0.000000); UsedBlocks->Add(b_7);
	auto b_8 = make1(600, 0, -39, 9, 6, 1, 11, 0, -180, 0, 660.000000, ""); mE(b_8, 26400.000000); UsedBlocks->Add(b_8);
	auto b_9 = make1(600, -6, -39, 9, 5, 1, 11, 0, 0, 0, 550.000000, ""); mE(b_9, 22000.000000); UsedBlocks->Add(b_9);
	auto b_10 = make1(1, 3, -31, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_10, 0.000000); UsedBlocks->Add(b_10);
	auto b_11 = make1(1, 24, -31, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_11, 0.000000); UsedBlocks->Add(b_11);
	auto b_12 = make1(2, 13, -31, 9, 20, 1, 11, 0, 0, 0, 2200.000000, ""); UsedBlocks->Add(b_12);
	auto b_13 = make1(2, 24, -20, 5, 20, 1, 3, 0, -90, 0, 600.000000, ""); UsedBlocks->Add(b_13);
	auto b_14 = make1(2, 24, -20, 13, 20, 1, 3, 0, -90, 0, 600.000000, ""); UsedBlocks->Add(b_14);
	auto b_15 = make1(1, 24, -14, 11, 1, 1, 3, 0, -180, 90, 30.000000, ""); mE(b_15, 0.000000); UsedBlocks->Add(b_15);
	auto b_16 = make1(1, 24, -18, 11, 1, 1, 3, 0, -180, 90, 30.000000, ""); mE(b_16, 0.000000); UsedBlocks->Add(b_16);
	auto b_17 = make1(1, 24, -22, 11, 1, 1, 3, 0, -180, 90, 30.000000, ""); mE(b_17, 0.000000); UsedBlocks->Add(b_17);
	auto b_18 = make1(1, 24, -26, 9, 1, 1, 5, 0, 180, 0, 50.000000, ""); mE(b_18, 0.000000); UsedBlocks->Add(b_18);
	auto b_19 = make1(1, 24, -28, 9, 1, 1, 5, 0, 180, 0, 50.000000, ""); mE(b_19, 0.000000); UsedBlocks->Add(b_19);
	auto b_20 = make1(1, 24, -14, 8, 1, 1, 4, 0, 180, 0, 40.000000, ""); mE(b_20, 0.000000); UsedBlocks->Add(b_20);
	auto b_21 = make1(1, 24, -17, 9, 1, 1, 3, 0, 180, 0, 30.000000, ""); mE(b_21, 0.000000); UsedBlocks->Add(b_21);
	auto b_22 = make1(1, 24, -21, 8, 1, 1, 4, 0, 180, 0, 40.000000, ""); mE(b_22, 0.000000); UsedBlocks->Add(b_22);
	auto b_23 = make1(1, 24, -18, 7, 1, 1, 3, 0, 180, 90, 30.000000, ""); mE(b_23, 0.000000); UsedBlocks->Add(b_23);
	auto b_24 = make1(1, 24, -19, 10, 1, 1, 1, 0, -180, 0, 10.000000, ""); mE(b_24, 0.000000); UsedBlocks->Add(b_24);
	auto b_25 = make1(1, 24, -19, 8, 1, 1, 1, 0, -180, 0, 10.000000, ""); mE(b_25, 0.000000); UsedBlocks->Add(b_25);
	auto b_26 = make1(1, 24, -22, 9, 1, 1, 2, 0, 180, 90, 20.000000, ""); mE(b_26, 0.000000); UsedBlocks->Add(b_26);
	auto b_27 = make1(1, 24, -10, 9, 1, 1, 11, 0, -180, 0, 110.000000, ""); mE(b_27, 0.000000); UsedBlocks->Add(b_27);
	auto b_28 = make1(2, 24, -30, 9, 1, 2, 5, 0, 0, 0, 100.000000, ""); UsedBlocks->Add(b_28);
	auto b_29 = make1(2, 24, -12, 9, 1, 2, 5, 0, 0, 0, 100.000000, ""); UsedBlocks->Add(b_29);
	auto b_30 = make1(2, 24, -16, 9, 1, 1, 5, 0, -180, 0, 50.000000, ""); UsedBlocks->Add(b_30);
	auto b_31 = make1(2, 24, -20, 9, 1, 1, 5, 0, -180, 0, 50.000000, ""); UsedBlocks->Add(b_31);
	auto b_32 = make1(2, 24, -27, 9, 1, 1, 5, 0, -180, 0, 50.000000, ""); UsedBlocks->Add(b_32);
	auto b_33 = make1(2, 24, -24, 9, 1, 2, 5, 0, 180, 0, 100.000000, ""); UsedBlocks->Add(b_33);
	auto b_34 = make1(2, 24, -13, 8, 1, 1, 4, 0, -90, 0, 40.000000, ""); UsedBlocks->Add(b_34);
	auto b_35 = make1(2, 24, -15, 8, 1, 1, 4, 0, -90, 0, 40.000000, ""); UsedBlocks->Add(b_35);
	auto b_36 = make1(2, 24, -18, 9, 1, 1, 3, 0, 180, 0, 30.000000, ""); UsedBlocks->Add(b_36);
	auto b_37 = make1(2, 24, -23, 10, 1, 1, 2, 0, -180, -90, 20.000000, ""); UsedBlocks->Add(b_37);
	auto b_38 = make1(2, 24, -22, 7, 1, 1, 2, 0, -180, 0, 20.000000, ""); UsedBlocks->Add(b_38);
	auto b_39 = make1(2, 24, -23, 7, 1, 1, 2, 0, -180, 0, 20.000000, ""); UsedBlocks->Add(b_39);
	auto b_40 = make1(2, 24, -19, 9, 1, 1, 1, 0, -180, 0, 10.000000, ""); UsedBlocks->Add(b_40);
	auto b_41 = make1(1, -15, -39, 9, 1, 1, 11, 0, 180, 0, 110.000000, ""); mE(b_41, 0.000000); mEP(b_41, 1, 1, 1.000000); UsedBlocks->Add(b_41);
	auto b_42 = make1(1000, 23, -30, 4, 1, 1, 1, 0, 90, 0, 10.000000, ""); mE(b_42, 2000.000000); mEP(b_42, 1, 1, 1.000000);
	mr(b_42, "97CF1EDB4D942B960C31B5A6AF7B17E6"); UsedBlocks->Add(b_42);
	auto b_43 = make1(1000, 23, -11, 4, 1, 1, 1, 0, 90, 0, 10.000000, ""); mE(b_43, 2000.000000); mEP(b_43, 1, 1, 1.000000);
	mr(b_43, "24CDB92B4512372CBCCAA9842D3C73FD"); UsedBlocks->Add(b_43);
	auto b_44 = make1(1, 22, -20, 9, 1, 20, 11, 0, 180, 0, 2200.000000, ""); mE(b_44, 0.000000); mEP(b_44, 1, 1, 1.000000); UsedBlocks->Add(b_44);
	auto b_45 = make1(500, 21, -21, 11, 1, 8, 5, 0, 0, 0, 10.000000, ""); mE(b_45, 100000.000000); UsedBlocks->Add(b_45);
	auto b_46 = make1(101, 21, -20, 8, 8, 1, 1, 0, -90, 180, 26.666668, ""); mE(b_46, 0.000000); UsedBlocks->Add(b_46);
	auto b_47 = make1(1150, 14, -13, 4, 3, 3, 2, 0, -180, 0, 180.000000, ""); mE(b_47, 18000.000000); UsedBlocks->Add(b_47);
	auto b_48 = make1(1150, 11, -13, 4, 3, 3, 2, 0, -180, 0, 180.000000, ""); mE(b_48, 18000.000000); UsedBlocks->Add(b_48);
	auto b_49 = make1(1150, 14, -10, 4, 3, 3, 2, 0, -180, 0, 180.000000, ""); mE(b_49, 18000.000000); UsedBlocks->Add(b_49);
	auto b_50 = make1(1, 9, -12, 4, 6, 1, 1, 0, 90, 0, 60.000000, ""); mE(b_50, 0.000000); UsedBlocks->Add(b_50);
	auto b_51 = make1(1, 12, -15, 4, 6, 1, 1, 0, 0, 0, 60.000000, ""); mE(b_51, 0.000000); UsedBlocks->Add(b_51);
	auto b_52 = make1(1, 16, -11, 4, 6, 1, 1, 0, -90, 0, 60.000000, ""); mE(b_52, 0.000000); UsedBlocks->Add(b_52);
	auto b_53 = make1(1, 13, -8, 4, 6, 1, 1, 0, 180, 0, 60.000000, ""); mE(b_53, 0.000000); UsedBlocks->Add(b_53);
	auto b_54 = make1(101, 8, -12, 4, 8, 1, 1, 0, 90, 0, 26.666668, ""); mE(b_54, 0.000000); UsedBlocks->Add(b_54);
	auto b_55 = make1(101, 13, -16, 4, 8, 1, 1, 0, 180, 0, 26.666668, ""); mE(b_55, 0.000000); UsedBlocks->Add(b_55);
	auto b_56 = make1(101, 17, -11, 4, 8, 1, 1, 0, -90, 0, 26.666668, ""); mE(b_56, 0.000000); UsedBlocks->Add(b_56);
	auto b_57 = make1(101, 12, -7, 4, 8, 1, 1, 0, 0, 0, 26.666668, ""); mE(b_57, 0.000000); UsedBlocks->Add(b_57);
	auto b_58 = make1(201, 8, -7, 4, 1, 1, 1, 0, 90, 0, 5.000000, ""); mE(b_58, 0.000000); UsedBlocks->Add(b_58);
	auto b_59 = make1(201, 8, -16, 4, 1, 1, 1, 0, 180, 0, 5.000000, ""); mE(b_59, 0.000000); UsedBlocks->Add(b_59);
	auto b_60 = make1(201, 17, -16, 4, 1, 1, 1, 0, -90, 0, 5.000000, ""); mE(b_60, 0.000000); UsedBlocks->Add(b_60);
	auto b_61 = make1(201, 17, -7, 4, 1, 1, 1, 0, 0, 0, 5.000000, ""); mE(b_61, 0.000000); UsedBlocks->Add(b_61);
	auto b_62 = make1(1, 9, -8, 4, 1, 1, 1, 0, -90, 0, 10.000000, ""); mE(b_62, 0.000000); UsedBlocks->Add(b_62);
	auto b_63 = make1(1, 16, -8, 4, 1, 1, 1, 0, -90, 0, 10.000000, ""); mE(b_63, 0.000000); UsedBlocks->Add(b_63);
	auto b_64 = make1(1, 16, -15, 4, 1, 1, 1, 0, -90, 0, 10.000000, ""); mE(b_64, 0.000000); UsedBlocks->Add(b_64);
	auto b_65 = make1(1, 9, -15, 4, 1, 1, 1, 0, -90, 0, 10.000000, ""); mE(b_65, 0.000000); UsedBlocks->Add(b_65);
	auto b_66 = make1(101, 16, -15, 8, 7, 1, 1, 90, -90, 90, 23.333334, ""); mE(b_66, 0.000000); UsedBlocks->Add(b_66);
	auto b_67 = make1(101, 9, -15, 8, 7, 1, 1, 90, 0, -90, 23.333334, ""); mE(b_67, 0.000000); UsedBlocks->Add(b_67);
	auto b_68 = make1(101, 9, -8, 8, 7, 1, 1, 90, 0, 0, 23.333334, ""); mE(b_68, 0.000000); UsedBlocks->Add(b_68);
	auto b_69 = make1(101, 16, -8, 8, 7, 1, 1, 90, 0, 90, 23.333334, ""); mE(b_69, 0.000000); UsedBlocks->Add(b_69);
	auto b_70 = make1(201, 16, -8, 12, 1, 1, 1, 0, 0, 0, 5.000000, ""); mE(b_70, 0.000000); UsedBlocks->Add(b_70);
	auto b_71 = make1(201, 9, -8, 12, 1, 1, 1, 0, 90, 0, 5.000000, ""); mE(b_71, 0.000000); UsedBlocks->Add(b_71);
	auto b_72 = make1(201, 9, -15, 12, 1, 1, 1, 0, -180, 0, 5.000000, ""); mE(b_72, 0.000000); UsedBlocks->Add(b_72);
	auto b_73 = make1(201, 16, -15, 12, 1, 1, 1, 0, -90, 0, 5.000000, ""); mE(b_73, 0.000000); UsedBlocks->Add(b_73);
	auto b_74 = make1(101, 12, -15, 12, 6, 1, 1, 0, 0, -90, 20.000000, ""); mE(b_74, 0.000000); UsedBlocks->Add(b_74);
	auto b_75 = make1(101, 16, -11, 12, 6, 1, 1, 0, -90, 0, 20.000000, ""); mE(b_75, 0.000000); UsedBlocks->Add(b_75);
	auto b_76 = make1(101, 12, -8, 12, 6, 1, 1, 0, 0, 0, 20.000000, ""); mE(b_76, 0.000000); UsedBlocks->Add(b_76);
	auto b_77 = make1(101, 9, -12, 12, 6, 1, 1, 0, 90, 0, 20.000000, ""); mE(b_77, 0.000000); UsedBlocks->Add(b_77);
	auto b_78 = make1(2, 13, -12, 12, 6, 6, 1, 0, 90, 0, 360.000000, ""); UsedBlocks->Add(b_78);
	auto b_79 = make1(2, 12, -8, 8, 6, 1, 7, 0, 0, 0, 420.000000, ""); UsedBlocks->Add(b_79);
	auto b_80 = make1(1, 3, -21, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_80, 0.000000); UsedBlocks->Add(b_80);
	auto b_81 = make1(1, -15, -21, 9, 1, 1, 11, 0, 90, 0, 110.000000, ""); mE(b_81, 0.000000); UsedBlocks->Add(b_81);
	auto b_82 = make1(600, -15, -30, 9, 17, 1, 11, 0, -90, 0, 1870.000000, ""); mE(b_82, 74800.000000); UsedBlocks->Add(b_82);
	auto b_83 = make1(2, 16, -12, 8, 6, 1, 7, 0, 90, 0, 420.000000, ""); UsedBlocks->Add(b_83);
	auto b_84 = make1(2, 14, -15, 8, 3, 1, 7, 0, 0, 0, 210.000000, ""); UsedBlocks->Add(b_84);
	auto b_85 = make1(2, 11, -15, 6, 3, 1, 4, 0, 0, 0, 120.000000, ""); UsedBlocks->Add(b_85);
	auto b_86 = make1(0, -26, -12, 1, 20, 20, 4, 0, 0, 0, 16000.000000, ""); mE(b_86, 0.000000); UsedBlocks->Add(b_86);
	auto b_87 = make1(0, -26, 8, 1, 20, 20, 4, 0, 0, 0, 16000.000000, ""); mE(b_87, 0.000000); UsedBlocks->Add(b_87);
	auto b_88 = make1(0, -5, 14, 1, 20, 10, 4, 0, -180, 0, 8000.000000, ""); mE(b_88, 0.000000); UsedBlocks->Add(b_88);
	auto b_89 = make1(0, 15, 14, 1, 20, 10, 4, 0, -180, 0, 8000.000000, ""); mE(b_89, 0.000000); UsedBlocks->Add(b_89);
	auto b_90 = make1(1150, 12, 6, 4, 9, 9, 2, 0, 0, 0, 1620.000000, ""); mE(b_90, 162000.000000); UsedBlocks->Add(b_90);
	auto b_91 = make1(2, 7, 6, 9, 9, 1, 10, 0, 90, 0, 900.000000, ""); UsedBlocks->Add(b_91);
	auto b_92 = make1(2, 12, 11, 9, 9, 1, 10, 0, 180, 0, 900.000000, ""); UsedBlocks->Add(b_92);
	auto b_93 = make1(2, 17, 6, 9, 9, 1, 10, 0, 90, 0, 900.000000, ""); UsedBlocks->Add(b_93);
	auto b_94 = make1(2, 12, 1, 9, 9, 1, 10, 0, -180, 0, 900.000000, ""); UsedBlocks->Add(b_94);
	auto b_95 = make1(2, 12, 6, 15, 9, 9, 1, 0, -90, 0, 810.000000, ""); UsedBlocks->Add(b_95);
	auto b_96 = make1(1, 23, -10, 9, 2, 1, 11, 0, 180, 0, 220.000000, ""); mE(b_96, 0.000000); UsedBlocks->Add(b_96);
	auto b_97 = make1(1, 24, 18, 9, 1, 1, 11, 0, -90, 0, 110.000000, ""); mE(b_97, 0.000000); UsedBlocks->Add(b_97);
	auto b_98 = make1(1, 24, 3, 9, 2, 1, 11, 0, 90, 0, 220.000000, ""); mE(b_98, 0.000000); UsedBlocks->Add(b_98);
	auto b_99 = make1(1, 4, 18, 9, 2, 1, 11, 0, 0, 0, 220.000000, ""); mE(b_99, 0.000000); UsedBlocks->Add(b_99);
	auto b_100 = make1(1, -4, -21, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_100, 0.000000); UsedBlocks->Add(b_100);
	auto b_101 = make1(1, -4, 18, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_101, 0.000000); UsedBlocks->Add(b_101);
	auto b_102 = make1(1, -4, -7, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_102, 0.000000); UsedBlocks->Add(b_102);
	auto b_103 = make1(1, -4, 1, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_103, 0.000000); UsedBlocks->Add(b_103);
	auto b_104 = make1(0, 15, -2, 1, 20, 20, 4, 0, 90, 0, 16000.000000, ""); mE(b_104, 0.000000); UsedBlocks->Add(b_104);
	auto b_105 = make1(1, -15, -7, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_105, 0.000000); UsedBlocks->Add(b_105);
	auto b_106 = make1(700, -7, -3, 9, 7, 7, 11, 0, 90, 0, 10.000000, ""); af(b_106, TEXT("DoorOpening"), 1); bd(b_106, TEXT("DoorState"), TEXT("0")); bd(b_106, TEXT("DoorYaw"), TEXT("0.0")); mE(b_106, 0.000000); UsedBlocks->Add(b_106);
	auto b_107 = make1(1, -35, -21, 9, 1, 1, 11, 0, -90, 0, 110.000000, ""); mE(b_107, 0.000000); UsedBlocks->Add(b_107);
	auto b_108 = make1(1, -35, 18, 9, 1, 1, 11, 0, -90, 0, 110.000000, ""); mE(b_108, 0.000000); UsedBlocks->Add(b_108);
	auto b_109 = make1(1, -35, -1, 9, 2, 1, 11, 0, -90, 0, 220.000000, ""); mE(b_109, 0.000000); UsedBlocks->Add(b_109);
	auto b_110 = make1(600, -25, -21, 9, 19, 1, 11, 0, 0, 0, 2090.000000, ""); mE(b_110, 83600.000000); UsedBlocks->Add(b_110);
	auto b_111 = make1(600, -35, -11, 9, 18, 1, 11, 0, -90, 0, 1980.000000, ""); mE(b_111, 79200.000000); UsedBlocks->Add(b_111);
	auto b_112 = make1(600, -25, 18, 9, 18, 1, 11, 0, -180, 0, 1980.000000, ""); mE(b_112, 79200.000000); UsedBlocks->Add(b_112);
	auto b_113 = make1(600, 0, 18, 9, 7, 1, 11, 0, -180, 0, 770.000000, ""); mE(b_113, 30800.000000); UsedBlocks->Add(b_113);
	auto b_114 = make1(600, 24, -4, 9, 12, 1, 11, 0, 90, 0, 1320.000000, ""); mE(b_114, 52800.000000); UsedBlocks->Add(b_114);
	auto b_115 = make1(600, 24, 11, 9, 13, 1, 11, 0, 90, 0, 1430.000000, ""); mE(b_115, 57200.000000); UsedBlocks->Add(b_115);
	auto b_116 = make1(1, -4, 10, 9, 1, 16, 11, 0, -180, 0, 1760.000000, ""); mE(b_116, 0.000000); UsedBlocks->Add(b_116);
	auto b_117 = make1(2, -10, -21, 9, 1, 10, 11, 0, -90, 0, 1100.000000, ""); UsedBlocks->Add(b_117);
	auto b_118 = make1(2, 0, -21, 9, 1, 6, 11, 0, 90, 0, 660.000000, ""); UsedBlocks->Add(b_118);
	auto b_119 = make1(2, -4, -14, 9, 1, 13, 11, 0, 180, 0, 1430.000000, ""); UsedBlocks->Add(b_119);
	auto b_120 = make1(1, -10, 9, 15, 1, 16, 11, 90, 0, 0, 1760.000000, ""); mE(b_120, 0.000000); UsedBlocks->Add(b_120);
	auto b_121 = make1(1, -10, 1, 9, 11, 2, 11, 0, 0, 0, 2420.000000, ""); mE(b_121, 0.000000); UsedBlocks->Add(b_121);
	auto b_122 = make1(1, -10, 18, 9, 11, 2, 11, 0, 180, 0, 2420.000000, ""); mE(b_122, 0.000000); UsedBlocks->Add(b_122);
	auto b_123 = make1(1, -16, 18, 9, 1, 1, 11, 0, 90, 0, 110.000000, ""); mE(b_123, 0.000000); UsedBlocks->Add(b_123);
	auto b_124 = make1(1, -16, 17, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_124, 0.000000); UsedBlocks->Add(b_124);
	auto b_125 = make1(1, -16, 16, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_125, 0.000000); UsedBlocks->Add(b_125);
	auto b_126 = make1(1, -16, 15, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_126, 0.000000); UsedBlocks->Add(b_126);
	auto b_127 = make1(1, -16, 14, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_127, 0.000000); UsedBlocks->Add(b_127);
	auto b_128 = make1(1, -16, 13, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_128, 0.000000); UsedBlocks->Add(b_128);
	auto b_129 = make1(1, -16, 12, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_129, 0.000000); UsedBlocks->Add(b_129);
	auto b_130 = make1(1, -16, 11, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_130, 0.000000); UsedBlocks->Add(b_130);
	auto b_131 = make1(1, -16, 10, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_131, 0.000000); UsedBlocks->Add(b_131);
	auto b_132 = make1(1, -16, 9, 9, 1, 1, 11, 0, 0, 0, 110.000000, ""); mE(b_132, 0.000000); UsedBlocks->Add(b_132);
	auto b_133 = make1(2, -15, -3, 9, 7, 1, 11, 0, 90, 0, 770.000000, ""); UsedBlocks->Add(b_133);
	auto b_134 = make1(600, -35, 8, 9, 18, 1, 11, 0, 90, 0, 1980.000000, ""); mE(b_134, 79200.000000); UsedBlocks->Add(b_134);
	auto b_135 = make1(600, 15, 18, 9, 18, 1, 11, 0, 180, 0, 1980.000000, ""); mE(b_135, 79200.000000); UsedBlocks->Add(b_135);
	auto b_136 = make1(1200, -18, 16, 5, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_136, 100000.000000); mEP(b_136, 1, 1, 1.000000); UsedBlocks->Add(b_136);
	auto b_137 = make1(1200, -18, 16, 8, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_137, 100000.000000); mEP(b_137, 1, 1, 1.000000); UsedBlocks->Add(b_137);
	auto b_138 = make1(1200, -18, 16, 11, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_138, 100000.000000); mEP(b_138, 1, 1, 1.000000); UsedBlocks->Add(b_138);
	auto b_139 = make1(1200, -18, 13, 5, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_139, 100000.000000); mEP(b_139, 1, 1, 1.000000); UsedBlocks->Add(b_139);
	auto b_140 = make1(1200, -18, 13, 8, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_140, 100000.000000); mEP(b_140, 1, 1, 1.000000); UsedBlocks->Add(b_140);
	auto b_141 = make1(1200, -18, 13, 11, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_141, 100000.000000); mEP(b_141, 1, 1, 1.000000); UsedBlocks->Add(b_141);
	auto b_142 = make1(1200, -18, 10, 5, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_142, 100000.000000); mEP(b_142, 1, 1, 1.000000); UsedBlocks->Add(b_142);
	auto b_143 = make1(1200, -18, 10, 8, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_143, 100000.000000); mEP(b_143, 1, 1, 1.000000); UsedBlocks->Add(b_143);
	auto b_144 = make1(1200, -18, 10, 11, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_144, 100000.000000); mEP(b_144, 1, 1, 1.000000); UsedBlocks->Add(b_144);
	auto b_145 = make1(1200, -18, 10, 14, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_145, 100000.000000); mEP(b_145, 1, 1, 1.000000); UsedBlocks->Add(b_145);
	auto b_146 = make1(1200, -18, 13, 14, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_146, 100000.000000); mEP(b_146, 1, 1, 1.000000); UsedBlocks->Add(b_146);
	auto b_147 = make1(1200, -18, 16, 14, 3, 3, 3, 0, 90, 0, 10.000000, ""); mE(b_147, 100000.000000); mEP(b_147, 1, 1, 1.000000); UsedBlocks->Add(b_147);
	auto b_148 = make1(1400, -13, -18, 5, 4, 3, 4, 0, 0, 0, 10.000000, "P1"); bd(b_148, TEXT("ItemTags"), TEXT("")); bd(b_148, TEXT("HasItem"), TEXT("0")); bd(b_148, TEXT("CurrentFilling"), TEXT("0.0")); mE(b_148, 150.000000); mEP(b_148, 1, 1, 1.000000); mO(b_148, 150.000000);
	mr(b_148, "469CB5F14D6D183C7F32AFACAC1B157D"); mrc(b_148, "6EF43F13408CD16117341C9C24CF0B84", 0); UsedBlocks->Add(b_148);
	auto b_149 = make1(1500, -8, -18, 4, 2, 2, 2, 0, -90, 0, 10.000000, ""); bd(b_149, TEXT("ItemTags"), TEXT("")); mO(b_149, 1000.000000); UsedBlocks->Add(b_149);
	auto b_150 = make1(1050, -15, -20, 11, 1, 1, 1, 0, 0, 90, 10.000000, "V1"); bd(b_150, TEXT("ReactsToDayCycle"), TEXT("0")); bd(b_150, TEXT("StateAtDay"), TEXT("0")); bd(b_150, TEXT("StateAtNight"), TEXT("0")); mE(b_150, 0.000000); mEP(b_150, 1, 1, 1.000000);
	mr(b_150, "6EF43F13408CD16117341C9C24CF0B84"); mrc(b_150, "469CB5F14D6D183C7F32AFACAC1B157D", 1); UsedBlocks->Add(b_150);
	auto b_151 = make1(1050, -5, 6, 11, 1, 1, 1, 90, 0, 0, 10.000000, "V2"); bd(b_151, TEXT("ReactsToDayCycle"), TEXT("0")); bd(b_151, TEXT("StateAtDay"), TEXT("0")); bd(b_151, TEXT("StateAtNight"), TEXT("0")); mE(b_151, 0.000000); mEP(b_151, 0, 1, 1.000000);
	mr(b_151, "F0004A164B28EB3091C545ABA3C0C027"); mrc(b_151, "32551A924362FD87E92200913083304E", 1); UsedBlocks->Add(b_151);
	auto b_152 = make1(1000, -14, 16, 13, 1, 1, 1, 90, 0, -90, 10.000000, "S2"); mE(b_152, 2000.000000); mEP(b_152, 0, 1, 1.000000);
	mr(b_152, "32551A924362FD87E92200913083304E"); mrc(b_152, "F0004A164B28EB3091C545ABA3C0C027", 0); UsedBlocks->Add(b_152);
	auto b_155 = make1(1200, 11, -10, 5, 3, 3, 3, 0, 0, 0, 10.000000, ""); mE(b_155, 100000.000000); UsedBlocks->Add(b_155);


	c->PlayerPosition = FVector(1067, -790, 82);
	c->PlayerRotation = FRotator(0, 154, 0);
	c->PlayerCameraRotation = FRotator(1.177662, 154.450226, 0.0);


	return c;
}