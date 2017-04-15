#include "GameSave.h"
#include "NewGameSaveHolder.h"

UNewGameSaveHolder::UNewGameSaveHolder()
{
	systemSaves = TMap<ENamedHardcodedLevel, FunctionPtrType>();

	systemSaves.Add(ENamedHardcodedLevel::MainMenu, &UNewGameSaveHolder::getMainMenuSave);
	systemSaves.Add(ENamedHardcodedLevel::DefaultLevel, &UNewGameSaveHolder::getDefaultGameSave);
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

USaveGameCarrier* UNewGameSaveHolder::getDefaultGameSave(bool full)
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemDefault", "Výchozí hra").ToString();
	c->FullFilePath = TEXT("_system_default");
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::DefaultLevel;

	if (!full)
		return c;

	c->CurrentTime = 34920.0f;
	c->PlayerUseFPSCamera = true;
	c->IsCreativeMode = true;

	c->PlayerOxygenComponent.CurrentFillingValue = 150.0f;
	c->PlayerElectricityComponent.CurrentObjectEnergy = 500.0f;

	c->inventoryTags = makeDefault();

	c->buildableBlocks.Add(makeBuildable(EmptyHandID, FVector(1, 1, 1)));
	c->inventoryBuildableBlocks.Add(makeInventoryBuildable(EmptyHandID, FVector(1, 1, 1)));
	c->buildableBlocks.Add(makeBuildable(DeleteID, FVector(1, 1, 1)));

	c->buildableBlocks.Add(makeBuildable(CubeBaseID, FVector(20, 20, 4)));
	c->buildableBlocks.Add(makeBuildable(CubePolycarbonateID, FVector(4, 4, 1)));
	c->buildableBlocks.Add(makeBuildable(CubeBuildingID, FVector(6, 6, 1)));
	c->buildableBlocks.Add(makeBuildable(CubeSideBaseID, FVector(20, 20, 4)));
	c->buildableBlocks.Add(makeBuildable(CubeSideBuildingID, FVector(1, 4, 1)));
	c->buildableBlocks.Add(makeBuildable(CubeBodyBuildingID, FVector(1, 1, 1)));
	c->buildableBlocks.Add(makeBuildable(OxygenTankID, FVector(2, 2, 2)));

	auto t = makeBuildable(TerminalID, FVector(1, 1, 1));
	t.AdditionalFlags.Add(TEXT("IsUsable"), 1);
	c->buildableBlocks.Add(t);

	c->buildableBlocks.Add(makeBuildable(WindowID, FVector(5, 1, 4)));

	auto UsedBlocks = &c->usedBlocks;

	auto b_0 = make(0, FVector(-6, -2, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 0, ""); b_0.HasElectricityData = true; b_0.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_0);
	auto b_1 = make(0, FVector(14, -2, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 0, ""); b_1.HasElectricityData = true; b_1.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_1);
	auto b_2 = make(0, FVector(14, -22, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 0, ""); b_2.HasElectricityData = true; b_2.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_2);
	auto b_3 = make(0, FVector(-6, -22, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 0, ""); b_3.HasElectricityData = true; b_3.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_3);
	auto b_4 = make(500, FVector(0, -17, 6), FVector(1, 1, 1), FRotator(0, 0, 0), 500, ""); b_4.HasElectricityData = true; b_4.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_4);
	auto b_5 = make(1, FVector(-15, -30, 9), FVector(20, 1, 11), FRotator(0, 90, 0), 1, ""); b_5.HasElectricityData = true; b_5.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_5);
	auto b_6 = make(1, FVector(24, -21, 6), FVector(20, 1, 6), FRotator(0, -90, 0), 1, ""); b_6.HasElectricityData = true; b_6.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_6);
	auto b_7 = make(1, FVector(24, -1, 6), FVector(20, 1, 6), FRotator(0, -90, 0), 1, ""); b_7.HasElectricityData = true; b_7.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_7);
	auto b_8 = make(1, FVector(14, -31, 6), FVector(20, 1, 6), FRotator(0, 180, 0), 1, ""); b_8.HasElectricityData = true; b_8.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_8);
	auto b_9 = make(1, FVector(-15, -1, 6), FVector(20, 1, 6), FRotator(0, -90, 0), 1, ""); b_9.HasElectricityData = true; b_9.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_9);
	auto b_10 = make(600, FVector(-12, -39, 9), FVector(6, 1, 11), FRotator(0, 0, 0), 600, ""); b_10.HasElectricityData = true; b_10.ElectricityInfo.CurrentObjectEnergy = 26400.000000;  UsedBlocks->Add(b_10);
	auto b_11 = make(100, FVector(10, -35, 1), FVector(8, 14, 4), FRotator(0, -90, 0), 100, ""); b_11.HasElectricityData = true; b_11.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_11);
	auto b_12 = make(1000, FVector(19, -32, 8), FVector(1, 1, 1), FRotator(0, 0, -90), 1000, ""); b_12.HasElectricityData = true; b_12.ElectricityInfo.CurrentObjectEnergy = 200.000000;  UsedBlocks->Add(b_12);
	auto b_13 = make(1000, FVector(25, -13, 8), FVector(1, 1, 1), FRotator(0, 90, -90), 1000, ""); b_13.HasElectricityData = true; b_13.ElectricityInfo.CurrentObjectEnergy = 200.000000;  UsedBlocks->Add(b_13);
	auto b_14 = make(1000, FVector(13, 9, 3), FVector(1, 1, 1), FRotator(0, -180, -90), 1000, ""); b_14.HasElectricityData = true; b_14.ElectricityInfo.CurrentObjectEnergy = 200.000000;  UsedBlocks->Add(b_14);
	auto b_15 = make(1000, FVector(-16, -10, 2), FVector(1, 1, 1), FRotator(0, -90, -90), 1000, ""); b_15.HasElectricityData = true; b_15.ElectricityInfo.CurrentObjectEnergy = 200.000000;  UsedBlocks->Add(b_15);
	auto b_16 = make(1000, FVector(4, -6, 4), FVector(1, 1, 1), FRotator(0, 90, 0), 1000, ""); b_16.HasElectricityData = true; b_16.ElectricityInfo.CurrentObjectEnergy = 200.000000;  UsedBlocks->Add(b_16);
	auto b_17 = make(1000, FVector(-11, -19, 4), FVector(1, 1, 1), FRotator(0, 90, 0), 1000, ""); b_17.HasElectricityData = true; b_17.ElectricityInfo.CurrentObjectEnergy = 200.000000;  UsedBlocks->Add(b_17);
	auto b_18 = make(0, FVector(-6, -35, 1), FVector(19, 8, 4), FRotator(0, 180, 0), 0, ""); b_18.HasElectricityData = true; b_18.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_18);
	auto b_19 = make(700, FVector(0, -35, 9), FVector(7, 7, 11), FRotator(0, 90, 0), 700, ""); b_19.HasElectricityData = true; b_19.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_19);
	auto b_20 = make(1, FVector(3, -39, 9), FVector(1, 1, 11), FRotator(0, 0, 0), 1, ""); b_20.HasElectricityData = true; b_20.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_20);
	auto b_21 = make(600, FVector(0, -39, 9), FVector(6, 1, 11), FRotator(0, -180, 0), 600, ""); b_21.HasElectricityData = true; b_21.ElectricityInfo.CurrentObjectEnergy = 26400.000000;  UsedBlocks->Add(b_21);
	auto b_22 = make(600, FVector(-6, -39, 9), FVector(5, 1, 11), FRotator(0, 0, 0), 600, ""); b_22.HasElectricityData = true; b_22.ElectricityInfo.CurrentObjectEnergy = 22000.000000;  UsedBlocks->Add(b_22);
	auto b_23 = make(2, FVector(4, -27, 4), FVector(2, 7, 1), FRotator(0, 0, 0), 2, "");  UsedBlocks->Add(b_23);
	auto b_24 = make(2, FVector(6, -27, 5), FVector(2, 7, 1), FRotator(0, 0, 0), 2, "");  UsedBlocks->Add(b_24);
	auto b_25 = make(2, FVector(8, -27, 6), FVector(2, 7, 1), FRotator(0, 0, 0), 2, "");  UsedBlocks->Add(b_25);
	auto b_26 = make(2, FVector(10, -27, 7), FVector(2, 7, 1), FRotator(0, 0, 0), 2, "");  UsedBlocks->Add(b_26);
	auto b_27 = make(2, FVector(12, -27, 8), FVector(2, 7, 1), FRotator(0, 0, 0), 2, "");  UsedBlocks->Add(b_27);
	auto b_28 = make(2, FVector(14, -27, 9), FVector(2, 7, 1), FRotator(0, 0, 0), 2, "");  UsedBlocks->Add(b_28);
	auto b_29 = make(1, FVector(21, -31, 13), FVector(8, 7, 1), FRotator(90, 0, 90), 1, ""); b_29.HasElectricityData = true; b_29.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_29);
	auto b_30 = make(1, FVector(14, -31, 13), FVector(8, 7, 1), FRotator(90, 0, 90), 1, ""); b_30.HasElectricityData = true; b_30.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_30);
	auto b_31 = make(1, FVector(24, -27, 13), FVector(8, 7, 1), FRotator(90, 0, -180), 1, ""); b_31.HasElectricityData = true; b_31.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_31);
	auto b_32 = make(1, FVector(24, -20, 13), FVector(8, 7, 1), FRotator(90, 0, -180), 1, ""); b_32.HasElectricityData = true; b_32.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_32);
	auto b_33 = make(2, FVector(16, -27, 10), FVector(2, 7, 1), FRotator(0, 0, 0), 2, "");  UsedBlocks->Add(b_33);
	auto b_34 = make(1, FVector(20, -27, 10), FVector(6, 6, 1), FRotator(0, -90, 0), 1, ""); b_34.HasElectricityData = true; b_34.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_34);
	auto b_35 = make(2, FVector(20, -23, 11), FVector(2, 7, 1), FRotator(0, -90, 0), 2, "");  UsedBlocks->Add(b_35);
	auto b_36 = make(2, FVector(20, -21, 12), FVector(2, 7, 1), FRotator(0, -90, 0), 2, "");  UsedBlocks->Add(b_36);
	auto b_37 = make(2, FVector(20, -19, 13), FVector(2, 7, 1), FRotator(0, -90, 0), 2, "");  UsedBlocks->Add(b_37);
	auto b_38 = make(2, FVector(20, -17, 14), FVector(2, 7, 1), FRotator(0, -90, 0), 2, "");  UsedBlocks->Add(b_38);
	auto b_39 = make(1, FVector(24, -13, 14), FVector(8, 7, 1), FRotator(-90, 0, 0), 1, ""); b_39.HasElectricityData = true; b_39.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_39);
	auto b_40 = make(2, FVector(20, -15, 15), FVector(2, 7, 1), FRotator(0, -90, 0), 2, "");  UsedBlocks->Add(b_40);
	auto b_41 = make(2, FVector(20, -13, 16), FVector(2, 7, 1), FRotator(0, -90, 0), 2, "");  UsedBlocks->Add(b_41);
	auto b_42 = make(2, FVector(20, -11, 17), FVector(2, 7, 1), FRotator(0, -90, 0), 2, "");  UsedBlocks->Add(b_42);
	auto b_43 = make(1, FVector(3, -31, 9), FVector(1, 1, 11), FRotator(0, 0, 0), 1, ""); b_43.HasElectricityData = true; b_43.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_43);
	auto b_44 = make(1, FVector(16, -23, 9), FVector(1, 1, 11), FRotator(0, 0, 0), 1, ""); b_44.HasElectricityData = true; b_44.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_44);
	auto b_45 = make(1100, FVector(11, 18, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_45.HasElectricityData = true; b_45.ElectricityInfo.CurrentObjectEnergy = 3405.000000;  UsedBlocks->Add(b_45);
	auto b_46 = make(1100, FVector(-9, 18, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_46.HasElectricityData = true; b_46.ElectricityInfo.CurrentObjectEnergy = 3291.000000;  UsedBlocks->Add(b_46);
	auto b_47 = make(1100, FVector(-9, 38, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_47.HasElectricityData = true; b_47.ElectricityInfo.CurrentObjectEnergy = 3353.000000;  UsedBlocks->Add(b_47);
	auto b_48 = make(1100, FVector(11, 38, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_48.HasElectricityData = true; b_48.ElectricityInfo.CurrentObjectEnergy = 3328.000000;  UsedBlocks->Add(b_48);
	auto b_49 = make(1100, FVector(31, 18, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_49.HasElectricityData = true; b_49.ElectricityInfo.CurrentObjectEnergy = 3338.000000;  UsedBlocks->Add(b_49);
	auto b_50 = make(1100, FVector(31, 38, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_50.HasElectricityData = true; b_50.ElectricityInfo.CurrentObjectEnergy = 3292.000000;  UsedBlocks->Add(b_50);
	auto b_51 = make(1100, FVector(31, 58, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_51.HasElectricityData = true; b_51.ElectricityInfo.CurrentObjectEnergy = 3373.000000;  UsedBlocks->Add(b_51);
	auto b_52 = make(1100, FVector(11, 59, 0), FVector(20, 20, 2), FRotator(0, -90, 0), 1100, ""); b_52.HasElectricityData = true; b_52.ElectricityInfo.CurrentObjectEnergy = 3335.000000;  UsedBlocks->Add(b_52);
	auto b_53 = make(1100, FVector(-9, 59, 0), FVector(20, 20, 2), FRotator(0, -90, 0), 1100, ""); b_53.HasElectricityData = true; b_53.ElectricityInfo.CurrentObjectEnergy = 3281.000000;  UsedBlocks->Add(b_53);
	auto b_54 = make(1050, FVector(-3, -9, 4), FVector(1, 1, 1), FRotator(0, 90, 0), 1050, "Tlacitko"); b_54.HasElectricityData = true; b_54.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_54);


	c->PlayerPosition = FVector(-214, -227, 163);
	c->PlayerRotation = FRotator(0, -32, 0);

	return c;
}

USaveGameCarrier* UNewGameSaveHolder::getEmptyGameSave(bool full)
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemEmpty", "Prázdná hra").ToString();
	c->FullFilePath = TEXT("_system_empty");
	c->CurrentTime = 34920.0f;
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::DefaultLevel;

	if (!full)
		return c;

	c->PlayerPosition = FVector(0, 0, 90);
	c->CurrentTime = 0.5f;
	c->PlayerUseFPSCamera = false;

	c->inventoryTags = makeDefault();

	auto UsedBlocks = &c->usedBlocks;

	//for (size_t x = 24995; x < 25000; x++)
	//{
	//	for (size_t y = 24995; y < 25000; y++)
	//	{
	//		for (size_t z = 0; z < 1; z++)
	//		{
	//			UsedBlocks->Add(make(1, FVector(-1 * x, -1 * y, z + 1), FVector(1, 1, 1), FRotator(0, 0, 0)));
	//			UsedBlocks->Add(make(1, FVector(x, y, z), FVector(1, 1, 1), FRotator(0, 0, 0)));
	//		}
	//	}
	//}

	c->PlayerOxygenComponent.CurrentFillingValue = 150.0f;
	c->PlayerElectricityComponent.CurrentObjectEnergy = 1000.0f;

	return c;
}

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

	c->PlayerPosition = FVector(-600, 0, 90);
	auto n = FDateTime::Now().GetTimeOfDay();
	c->CurrentTime = (float)n.GetTotalSeconds();
	c->PlayerUseFPSCamera = true;

#if WITH_EDITORONLY_DATA
#endif
	c->CurrentTime = 34920.0f;

	auto UsedBlocks = &c->usedBlocks;

	// TODO

	UsedBlocks->Add(make(0, FVector(-6, -2, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 15979.000000));
	UsedBlocks->Add(make(0, FVector(14, -2, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 15971.000000));
	UsedBlocks->Add(make(0, FVector(14, -22, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 15983.000000));
	UsedBlocks->Add(make(0, FVector(-6, -22, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 15976.000000));
	UsedBlocks->Add(make(500, FVector(0, -17, 6), FVector(1, 1, 1), FRotator(0, 0, 0), 9.000000));
	UsedBlocks->Add(make(1, FVector(-15, -30, 9), FVector(20, 1, 11), FRotator(0, 90, 0), 2199.000000));
	UsedBlocks->Add(make(1, FVector(24, -21, 6), FVector(20, 1, 6), FRotator(0, -90, 0), 1200.000000));
	UsedBlocks->Add(make(1, FVector(24, -1, 6), FVector(20, 1, 6), FRotator(0, -90, 0), 1200.000000));
	UsedBlocks->Add(make(1, FVector(14, -31, 6), FVector(20, 1, 6), FRotator(0, 180, 0), 1199.000000));
	UsedBlocks->Add(make(1, FVector(-15, -1, 6), FVector(20, 1, 6), FRotator(0, -90, 0), 1200.000000));
	UsedBlocks->Add(make(600, FVector(-12, -39, 9), FVector(6, 1, 11), FRotator(0, 0, 0), 660.000000));
	UsedBlocks->Add(make(100, FVector(10, -35, 1), FVector(8, 14, 4), FRotator(0, -90, 0), 1484.333374));
	UsedBlocks->Add(make(1000, FVector(19, -32, 8), FVector(1, 1, 1), FRotator(0, 0, -90), 9.000000));
	UsedBlocks->Add(make(1000, FVector(25, -13, 8), FVector(1, 1, 1), FRotator(0, 90, -90), 10.000000));
	UsedBlocks->Add(make(1000, FVector(13, 9, 3), FVector(1, 1, 1), FRotator(0, -180, -90), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-16, -10, 2), FVector(1, 1, 1), FRotator(0, -90, -90), 10.000000));
	UsedBlocks->Add(make(1000, FVector(4, -6, 4), FVector(1, 1, 1), FRotator(0, 90, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-11, -19, 4), FVector(1, 1, 1), FRotator(0, 90, 0), 10.000000));
	UsedBlocks->Add(make(0, FVector(-6, -35, 1), FVector(19, 8, 4), FRotator(0, 180, 0), 6072.000000));
	UsedBlocks->Add(make(700, FVector(0, -35, 9), FVector(7, 7, 11), FRotator(0, 90, 0), 9.000000));
	UsedBlocks->Add(make(1, FVector(3, -39, 9), FVector(1, 1, 11), FRotator(0, 0, 0), 110.000000));
	UsedBlocks->Add(make(600, FVector(0, -39, 9), FVector(6, 1, 11), FRotator(0, -180, 0), 660.000000));
	UsedBlocks->Add(make(600, FVector(-6, -39, 9), FVector(5, 1, 11), FRotator(0, 0, 0), 549.000000));
	UsedBlocks->Add(make(1000, FVector(16, 68, 0), FVector(1, 1, 1), FRotator(0, -90, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(9, 78, 0), FVector(1, 1, 1), FRotator(0, -90, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-9, 79, 0), FVector(1, 1, 1), FRotator(0, -90, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-17, 65, 0), FVector(1, 1, 1), FRotator(0, -90, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-29, 66, 0), FVector(1, 1, 1), FRotator(0, -90, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-37, 64, 0), FVector(1, 1, 1), FRotator(0, -90, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-39, 56, 0), FVector(1, 1, 1), FRotator(0, -90, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-36, 50, 0), FVector(1, 1, 1), FRotator(0, -90, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-41, 38, 0), FVector(1, 1, 1), FRotator(0, -90, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-86, -15, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-88, -6, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-86, 1, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-84, 14, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-83, 21, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-82, 27, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-85, 32, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-89, 38, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-93, 42, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-97, 46, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-100, 52, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-105, 56, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-108, 59, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-111, 65, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-116, 71, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-120, 75, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-123, 80, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-105, 89, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000));
	UsedBlocks->Add(make(1000, FVector(-98, 86, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000));
	UsedBlocks->Add(make(2, FVector(4, -27, 4), FVector(2, 7, 1), FRotator(0, 0, 0), 140.000000));
	UsedBlocks->Add(make(2, FVector(6, -27, 5), FVector(2, 7, 1), FRotator(0, 0, 0), 139.000000));
	UsedBlocks->Add(make(2, FVector(8, -27, 6), FVector(2, 7, 1), FRotator(0, 0, 0), 140.000000));
	UsedBlocks->Add(make(2, FVector(10, -27, 7), FVector(2, 7, 1), FRotator(0, 0, 0), 140.000000));
	UsedBlocks->Add(make(2, FVector(12, -27, 8), FVector(2, 7, 1), FRotator(0, 0, 0), 139.000000));
	UsedBlocks->Add(make(2, FVector(14, -27, 9), FVector(2, 7, 1), FRotator(0, 0, 0), 139.000000));
	UsedBlocks->Add(make(1, FVector(21, -31, 13), FVector(8, 7, 1), FRotator(90, 0, 90), 558.000000));
	UsedBlocks->Add(make(1, FVector(14, -31, 13), FVector(8, 7, 1), FRotator(90, 0, 90), 560.000000));
	UsedBlocks->Add(make(1, FVector(24, -27, 13), FVector(8, 7, 1), FRotator(90, 0, -180), 559.000000));
	UsedBlocks->Add(make(1, FVector(24, -20, 13), FVector(8, 7, 1), FRotator(90, 0, -180), 560.000000));
	UsedBlocks->Add(make(2, FVector(16, -27, 10), FVector(2, 7, 1), FRotator(0, 0, 0), 139.000000));
	UsedBlocks->Add(make(1, FVector(20, -27, 10), FVector(6, 6, 1), FRotator(0, -90, 0), 359.000000));
	UsedBlocks->Add(make(2, FVector(20, -23, 11), FVector(2, 7, 1), FRotator(0, -90, 0), 138.000000));
	UsedBlocks->Add(make(2, FVector(20, -21, 12), FVector(2, 7, 1), FRotator(0, -90, 0), 140.000000));
	UsedBlocks->Add(make(2, FVector(20, -19, 13), FVector(2, 7, 1), FRotator(0, -90, 0), 140.000000));
	UsedBlocks->Add(make(2, FVector(20, -17, 14), FVector(2, 7, 1), FRotator(0, -90, 0), 140.000000));
	UsedBlocks->Add(make(1, FVector(24, -13, 14), FVector(8, 7, 1), FRotator(-90, 0, 0), 560.000000));
	UsedBlocks->Add(make(2, FVector(20, -15, 15), FVector(2, 7, 1), FRotator(0, -90, 0), 140.000000));
	UsedBlocks->Add(make(2, FVector(20, -13, 16), FVector(2, 7, 1), FRotator(0, -90, 0), 139.000000));
	UsedBlocks->Add(make(2, FVector(20, -11, 17), FVector(2, 7, 1), FRotator(0, -90, 0), 139.000000));
	UsedBlocks->Add(make(1, FVector(3, -31, 9), FVector(1, 1, 11), FRotator(0, 0, 0), 110.000000));
	UsedBlocks->Add(make(1, FVector(16, -23, 9), FVector(1, 1, 11), FRotator(0, 0, 0), 110.000000));

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