#include "GameSave.h"
#include "NewGameSaveHolder.h"

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
//	c->CurrentTime = (float)n.GetTotalSeconds();
	c->PlayerUseFPSCamera = true;


	auto UsedBlocks = &c->usedBlocks;


	auto def = getDefaultGameSave(full);
	auto super = getSuperSizeGameSave(full);

	UsedBlocks->Append(def->usedBlocks);
	UsedBlocks->Append(super->usedBlocks);

	c->PlayerPosition = FVector(983, -712, 82);
	c->PlayerRotation = FRotator(0, 146, 0);

	return c;
}