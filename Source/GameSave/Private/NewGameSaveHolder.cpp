﻿#include "GameSave.h"
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

	c->PlayerPosition = FVector(-600, 0, 90);
	c->CurrentTime = 34920.0f;
	c->PlayerUseFPSCamera = true;
	c->IsCreativeMode = true;

	c->PlayerOxygenComponent.CurrentFillingValue = 150.0f;
	c->PlayerElectricityComponent.CurrentObjectEnergy = 500.0f;

	c->inventoryTags = makeDefault();

	c->buildableBlocks.Add(makeBuildable(EmptyHandID, FVector(1, 1, 1)));
	c->inventoryBuildableBlocks.Add(makeInventoryBuildable(EmptyHandID, FVector(1, 1, 1)));
	c->buildableBlocks.Add(makeBuildable(DeleteID, FVector(1, 1, 1)));

	////TODO
	///*auto door = makeBuildable(EBlockName::Door, FVector(7, 7, 11));
	//door->AdditionalFlags.Add(GetNameForTag(ENamedTag::DoorOpening), (int32)EDoorOpening::Right);
	//door->Name = TEXT("Dveře");
	//buildable->Add(door);*/

	auto cb = makeBuildable(CubeBaseID, FVector(20, 20, 4));
	//cb.AdditionalFlags.Add(TEXT("MyTag"), 0);
	c->buildableBlocks.Add(cb);

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

	UsedBlocks->Add(make(0, FVector(-27, -21, 1), FVector(20, 20, 4), FRotator(0, -90, 0)));
	UsedBlocks->Add(make(1, FVector(-27, -22, 4), FVector(6, 6, 1), FRotator(0, -90, 0)));
	UsedBlocks->Add(make(2, FVector(-27, -20, 7), FVector(4, 4, 1), FRotator(-90, 0, -90)));
	UsedBlocks->Add(make(2, FVector(-27, -25, 7), FVector(4, 4, 1), FRotator(-90, 0, -90)));
	UsedBlocks->Add(make(2, FVector(-24, -22, 7), FVector(4, 4, 1), FRotator(-90, 0, 180)));
	UsedBlocks->Add(make(2, FVector(-29, -22, 7), FVector(4, 4, 1), FRotator(-90, 0, 180)));
	UsedBlocks->Add(make(101, FVector(-29, -25, 7), FVector(4, 1, 1), FRotator(-90, 0, 180)));
	UsedBlocks->Add(make(101, FVector(-24, -25, 7), FVector(4, 1, 1), FRotator(-90, 0, -90)));
	UsedBlocks->Add(make(101, FVector(-24, -20, 7), FVector(4, 1, 1), FRotator(-90, 0, 0)));
	UsedBlocks->Add(make(101, FVector(-29, -20, 7), FVector(4, 1, 1), FRotator(-90, 0, 90)));
	UsedBlocks->Add(make(101, FVector(-29, -22, 9), FVector(4, 1, 1), FRotator(0, -90, -90)));
	UsedBlocks->Add(make(101, FVector(-26, -20, 9), FVector(4, 1, 1), FRotator(0, 180, -90)));
	UsedBlocks->Add(make(101, FVector(-24, -23, 9), FVector(4, 1, 1), FRotator(0, 90, -90)));
	UsedBlocks->Add(make(101, FVector(-27, -25, 9), FVector(4, 1, 1), FRotator(0, 0, -90)));
	UsedBlocks->Add(make(2, FVector(-26, -22, 9), FVector(4, 4, 1), FRotator(0, -180, 0)));
	UsedBlocks->Add(make(201, FVector(-29, -20, 9), FVector(1, 1, 1), FRotator(0, 90, 0)));
	UsedBlocks->Add(make(201, FVector(-29, -25, 9), FVector(1, 1, 1), FRotator(0, -180, 0)));
	UsedBlocks->Add(make(201, FVector(-24, -25, 9), FVector(1, 1, 1), FRotator(0, -90, 0)));
	UsedBlocks->Add(make(201, FVector(-24, -20, 9), FVector(1, 1, 1), FRotator(0, 0, 0)));

	auto ox1 = make(1500, FVector(-32, -12, 4), FVector(2, 2, 2), FRotator(0, -90, 0));
	makeOxygen(ox1, 5);
	UsedBlocks->Add(ox1);

	auto ox2 = make(1500, FVector(-18, -12, 4), FVector(2, 2, 2), FRotator(0, -90, 0));
	makeOxygen(ox2, 9);
	UsedBlocks->Add(ox2);

	auto oxInventory1 = makeInventoryBuildable(OxygenTankID, FVector(2, 2, 2));
	makeOxygen(oxInventory1, 7.5f);
	c->inventoryBuildableBlocks.Add(oxInventory1);

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

	for (size_t x = 24995; x < 25000; x++)
	{
		for (size_t y = 24995; y < 25000; y++)
		{
			for (size_t z = 0; z < 1; z++)
			{
				UsedBlocks->Add(make(1, FVector(-1 * x, -1 * y, z+1), FVector(1, 1, 1), FRotator(0, 0, 0)));
				UsedBlocks->Add(make(1, FVector(x, y, z), FVector(1, 1, 1), FRotator(0, 0, 0)));
			}
		}
	}

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

	//auto UsedBlocks = &c->UsedBlocks;

	// TODO
	/*UsedBlocks->Add(make(EBlockName::WindowCube, FVector(-11, 3, 0), FVector(4, 4, 1), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-10, 6, 0), FVector(1, 1, 4), FRotator(90, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-10, 1, 0), FVector(1, 1, 4), FRotator(90, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-13, 4, 0), FVector(1, 1, 4), FRotator(90, 90, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-8, 4, 0), FVector(1, 1, 4), FRotator(90, 90, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-8, 1, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-13, 1, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-13, 6, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-13, 1, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-8, 1, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::WindowCube, FVector(-10, 1, 2), FVector(4, 4, 1), FRotator(180, 0, 90)));
	UsedBlocks->Add(make(EBlockName::WindowCube, FVector(-13, 4, 2), FVector(4, 4, 1), FRotator(180, 90, 90)));
	UsedBlocks->Add(make(EBlockName::WindowCube, FVector(-11, 6, 2), FVector(4, 4, 1), FRotator(180, 180, 90)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-10, 6, 5), FVector(1, 1, 4), FRotator(90, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-13, 4, 5), FVector(1, 1, 4), FRotator(90, 90, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-8, 6, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::WindowCube, FVector(-11, 3, 5), FVector(4, 4, 1), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-8, 3, 5), FVector(1, 1, 4), FRotator(0, 180, 270)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-10, 1, 5), FVector(1, 1, 4), FRotator(0, 270, 270)));
	UsedBlocks->Add(make(EBlockName::WindowCube, FVector(-8, 3, 3), FVector(4, 4, 1), FRotator(0, 90, 90)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-13, 6, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-8, 6, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));*/

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