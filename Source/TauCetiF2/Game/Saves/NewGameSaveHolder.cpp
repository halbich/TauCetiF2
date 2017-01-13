

#include "TauCetiF2.h"
#include "NewGameSaveHolder.h"


UNewGameSaveHolder* UNewGameSaveHolder::Instance()
{

	auto instance = NewObject<UNewGameSaveHolder>();
	instance->init();
	return instance;

	/*static UNewGameSaveHolder* instance = nullptr;

	if (instance == nullptr || !instance->IsValidLowLevel())
	{
		UClass *saveClass = UNewGameSaveHolder::StaticClass();
		instance = (UNewGameSaveHolder*)ConstructObject<UObject>(saveClass);

		instance->init();
	}

	return instance;*/

}

void UNewGameSaveHolder::init()
{
	fillingFunctions[(uint8)ENamedHardcodedLevel::MainMenu] = &UNewGameSaveHolder::getMainMenuSave;
	fillingFunctions[(uint8)ENamedHardcodedLevel::DefaultLevel] = &UNewGameSaveHolder::getDefaultGameSave;
	fillingFunctions[(uint8)ENamedHardcodedLevel::EmptyLevel] = &UNewGameSaveHolder::getEmptyGameSave;


	mainMenuSave = (this->* (fillingFunctions[(uint8)ENamedHardcodedLevel::MainMenu]))();

	for (uint8 i = ((uint8)ENamedHardcodedLevel::MainMenu + 1); i < (uint8)ENamedHardcodedLevel::HardcodedLevelsMax; i++)
	{
		newGameSaves.Add((this->* (fillingFunctions[i]))());
	}

}


TArray<USaveGameCarrier*> UNewGameSaveHolder::GetNewSaveGamesList()
{
	return Instance()->newGameSaves;
}


USaveGameCarrier * UNewGameSaveHolder::DEBUG_GetTestSave()
{
	auto i = Instance();
	if (i && i->IsValidLowLevel())
	{
		if (i->newGameSaves.Num() > 0)
		{
			auto sav = i->newGameSaves[0];
			if (sav && sav->IsValidLowLevel())
				return sav;
		}
	}
	return nullptr;
}

USaveGameCarrier* UNewGameSaveHolder::GetSaveForMainMenu()
{
	return Instance()->mainMenuSave;

}


USaveGameCarrier* UNewGameSaveHolder::getDefaultGameSave()
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = TEXT("Bìžná hra");
	c->SystemSaveName = LC_SystemSave_Default;
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::DefaultLevel;


	c->PlayerPosition = FVector(-600, 0, 90);
	c->PartOfDay = 0.5f;
	c->PlayerUseFPSCamera = true;

	c->InventoryTags = UInventoryTags::GetDefault();

	c->BuildableBlocks = UBuildableBlockInfo::GetSystemActions();

	auto buildable = &c->BuildableBlocks;

	//TODO
	/*auto door = makeBuildable(EBlockName::Door, FVector(7, 7, 11));
	door->AdditionalFlags.Add(GetNameForTag(ENamedTag::DoorOpening), (int32)EDoorOpening::Right);
	door->Name = TEXT("Dveøe");
	buildable->Add(door);*/

	/*buildable->Add(makeBuildable(EBlockName::BaseCube, FVector(20, 20, 4)));
	buildable->Add(makeBuildable(EBlockName::BaseRamp, FVector(7, 7, 4)));*/

	auto UsedBlocks = &c->UsedBlocks;

	return c;

}

USaveGameCarrier* UNewGameSaveHolder::getEmptyGameSave()
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = TEXT("Prázdná hra");
	c->SystemSaveName = LC_SystemSave_Empty;
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::DefaultLevel;

	c->PlayerPosition = FVector(0, 0, 90);
	c->PartOfDay = 0.5f;
	c->PlayerUseFPSCamera = false;

	c->InventoryTags = UInventoryTags::GetDefault();

	c->BuildableBlocks = UBuildableBlockInfo::GetSystemActions();
	return c;

}

USaveGameCarrier* UNewGameSaveHolder::getMainMenuSave()
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = TEXT("Main Menu Save");
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::MainMenu;


	c->PlayerPosition = FVector(-600, 0, 90);
	c->PartOfDay = 0.5f;
	c->PlayerUseFPSCamera = true;




	auto UsedBlocks = &c->UsedBlocks;

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