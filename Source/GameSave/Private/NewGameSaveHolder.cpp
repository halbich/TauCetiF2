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

	UsedBlocks->Add(make(0, FVector(-6, -2, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 6018.000000));
	UsedBlocks->Add(make(0, FVector(14, -2, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 6211.000000));
	UsedBlocks->Add(make(0, FVector(14, -22, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 12264.000000));
	UsedBlocks->Add(make(0, FVector(-6, -22, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 6478.000000));
	UsedBlocks->Add(make(500, FVector(0, -17, 6), FVector(1, 1, 1), FRotator(0, 0, 0), 0.000000));
	UsedBlocks->Add(make(1, FVector(-15, -30, 9), FVector(20, 1, 11), FRotator(0, 90, 0), 1660.000000));
	UsedBlocks->Add(make(1, FVector(24, -21, 6), FVector(20, 1, 6), FRotator(0, -90, 0), 1200.000000));
	UsedBlocks->Add(make(1, FVector(24, -1, 6), FVector(20, 1, 6), FRotator(0, -90, 0), 697.000000));
	UsedBlocks->Add(make(1, FVector(14, -31, 6), FVector(20, 1, 6), FRotator(0, 180, 0), 982.000000));
	UsedBlocks->Add(make(1, FVector(-15, -1, 6), FVector(20, 1, 6), FRotator(0, -90, 0), 692.000000));
	UsedBlocks->Add(make(600, FVector(-12, -39, 9), FVector(6, 1, 11), FRotator(0, 0, 0), 487.000000));
	UsedBlocks->Add(make(100, FVector(10, -35, 1), FVector(8, 14, 4), FRotator(0, -90, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(19, -32, 8), FVector(1, 1, 1), FRotator(0, 0, -90), 0.000000));
	UsedBlocks->Add(make(1000, FVector(25, -13, 8), FVector(1, 1, 1), FRotator(0, 90, -90), 0.000000));
	UsedBlocks->Add(make(1000, FVector(13, 9, 3), FVector(1, 1, 1), FRotator(0, -180, -90), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-16, -10, 2), FVector(1, 1, 1), FRotator(0, -90, -90), 0.000000));
	UsedBlocks->Add(make(1000, FVector(4, -6, 4), FVector(1, 1, 1), FRotator(0, 90, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-11, -19, 4), FVector(1, 1, 1), FRotator(0, 90, 0), 0.000000));
	UsedBlocks->Add(make(0, FVector(-6, -35, 1), FVector(19, 8, 4), FRotator(0, 180, 0), 3990.000000));
	UsedBlocks->Add(make(700, FVector(0, -35, 9), FVector(7, 7, 11), FRotator(0, 90, 0), 0.000000));
	UsedBlocks->Add(make(1, FVector(3, -39, 9), FVector(1, 1, 11), FRotator(0, 0, 0), 92.000000));
	UsedBlocks->Add(make(600, FVector(0, -39, 9), FVector(6, 1, 11), FRotator(0, -180, 0), 487.000000));
	UsedBlocks->Add(make(600, FVector(-6, -39, 9), FVector(5, 1, 11), FRotator(0, 0, 0), 436.000000));
	UsedBlocks->Add(make(1000, FVector(9, 78, 0), FVector(1, 1, 1), FRotator(0, -90, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-9, 79, 0), FVector(1, 1, 1), FRotator(0, -90, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-29, 66, 0), FVector(1, 1, 1), FRotator(0, -90, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-37, 64, 0), FVector(1, 1, 1), FRotator(0, -90, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-39, 56, 0), FVector(1, 1, 1), FRotator(0, -90, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-36, 50, 0), FVector(1, 1, 1), FRotator(0, -90, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-41, 38, 0), FVector(1, 1, 1), FRotator(0, -90, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-86, -15, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-88, -6, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-86, 1, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-84, 14, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-83, 21, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-82, 27, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-85, 32, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-89, 38, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-93, 42, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-97, 46, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-100, 52, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-105, 56, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-108, 59, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-111, 65, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-116, 71, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-120, 75, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-123, 80, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-105, 89, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 0.000000));
	UsedBlocks->Add(make(1000, FVector(-98, 86, 0), FVector(1, 1, 1), FRotator(0, -180, 0), 0.000000));
	UsedBlocks->Add(make(2, FVector(4, -27, 4), FVector(2, 7, 1), FRotator(0, 0, 0), 0.000000));
	UsedBlocks->Add(make(2, FVector(6, -27, 5), FVector(2, 7, 1), FRotator(0, 0, 0), 0.000000));
	UsedBlocks->Add(make(2, FVector(8, -27, 6), FVector(2, 7, 1), FRotator(0, 0, 0), 0.000000));
	UsedBlocks->Add(make(2, FVector(10, -27, 7), FVector(2, 7, 1), FRotator(0, 0, 0), 0.000000));
	UsedBlocks->Add(make(2, FVector(12, -27, 8), FVector(2, 7, 1), FRotator(0, 0, 0), 0.000000));
	UsedBlocks->Add(make(2, FVector(14, -27, 9), FVector(2, 7, 1), FRotator(0, 0, 0), 0.000000));
	UsedBlocks->Add(make(1, FVector(21, -31, 13), FVector(8, 7, 1), FRotator(90, 0, 90), 369.000000));
	UsedBlocks->Add(make(1, FVector(14, -31, 13), FVector(8, 7, 1), FRotator(90, 0, 90), 382.000000));
	UsedBlocks->Add(make(1, FVector(24, -27, 13), FVector(8, 7, 1), FRotator(90, 0, -180), 376.000000));
	UsedBlocks->Add(make(1, FVector(24, -20, 13), FVector(8, 7, 1), FRotator(90, 0, -180), 373.000000));
	UsedBlocks->Add(make(2, FVector(16, -27, 10), FVector(2, 7, 1), FRotator(0, 0, 0), 0.000000));
	UsedBlocks->Add(make(1, FVector(20, -27, 10), FVector(6, 6, 1), FRotator(0, -90, 0), 0.000000));
	UsedBlocks->Add(make(2, FVector(20, -23, 11), FVector(2, 7, 1), FRotator(0, -90, 0), 0.000000));
	UsedBlocks->Add(make(2, FVector(20, -21, 12), FVector(2, 7, 1), FRotator(0, -90, 0), 0.000000));
	UsedBlocks->Add(make(2, FVector(20, -19, 13), FVector(2, 7, 1), FRotator(0, -90, 0), 0.000000));
	UsedBlocks->Add(make(2, FVector(20, -17, 14), FVector(2, 7, 1), FRotator(0, -90, 0), 0.000000));
	UsedBlocks->Add(make(1, FVector(24, -13, 14), FVector(8, 7, 1), FRotator(-90, 0, 0), 383.000000));
	UsedBlocks->Add(make(2, FVector(20, -15, 15), FVector(2, 7, 1), FRotator(0, -90, 0), 0.000000));
	UsedBlocks->Add(make(2, FVector(20, -13, 16), FVector(2, 7, 1), FRotator(0, -90, 0), 0.000000));
	UsedBlocks->Add(make(2, FVector(20, -11, 17), FVector(2, 7, 1), FRotator(0, -90, 0), 0.000000));
	UsedBlocks->Add(make(1, FVector(3, -31, 9), FVector(1, 1, 11), FRotator(0, 0, 0), 84.000000));
	UsedBlocks->Add(make(1, FVector(16, -23, 9), FVector(1, 1, 11), FRotator(0, 0, 0), 92.000000));
	UsedBlocks->Add(make(1100, FVector(11, 18, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 0.000000));
	UsedBlocks->Add(make(1100, FVector(-9, 18, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 0.000000));
	UsedBlocks->Add(make(1100, FVector(-9, 38, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 0.000000));
	UsedBlocks->Add(make(1100, FVector(11, 38, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 0.000000));
	UsedBlocks->Add(make(1100, FVector(31, 18, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 0.000000));
	UsedBlocks->Add(make(1100, FVector(31, 38, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 0.000000));
	UsedBlocks->Add(make(1100, FVector(31, 58, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 0.000000));
	UsedBlocks->Add(make(1100, FVector(11, 59, 0), FVector(20, 20, 2), FRotator(0, -90, 0), 0.000000));
	UsedBlocks->Add(make(1100, FVector(-9, 59, 0), FVector(20, 20, 2), FRotator(0, -90, 0), 0.000000));
	UsedBlocks->Add(make(1050, FVector(-3, -9, 4), FVector(1, 1, 1), FRotator(0, 90, 0), 0.000000));

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