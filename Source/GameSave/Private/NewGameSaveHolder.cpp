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

	c->CurrentTime = 34920.0f;
	c->PlayerUseFPSCamera = true;
	c->IsCreativeMode = true;
	c->PlayerHealth = 1000;

	c->PlayerOxygenComponent.CurrentObjectOxygen = 150.0f;
	c->PlayerElectricityComponent.CurrentObjectEnergy = 500.0f;

	c->inventoryTags = makeDefault();

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
	auto b_12 = make(1000, FVector(19, -32, 8), FVector(1, 1, 1), FRotator(0, 0, -90), 1000, ""); b_12.HasElectricityData = true; b_12.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_12.HasRelationshipData = true; FGuid b_12_rel_id; FGuid::Parse("A76B52884FBC2B0268FA6FA273A5C030", b_12_rel_id); b_12.RelationshipInfo.ID = b_12_rel_id; UsedBlocks->Add(b_12);
	auto b_13 = make(1000, FVector(25, -13, 8), FVector(1, 1, 1), FRotator(0, 90, -90), 1000, ""); b_13.HasElectricityData = true; b_13.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_13.HasRelationshipData = true; FGuid b_13_rel_id; FGuid::Parse("703EFD1844A5E576A5932288A38E20B9", b_13_rel_id); b_13.RelationshipInfo.ID = b_13_rel_id;
	FRelationshipInfo b_13_rel0; FGuid b_13_rel0_id; FGuid::Parse("797CEC8E499C456A5E1C46B8312736AB", b_13_rel0_id); b_13_rel0.TargetID = b_13_rel0_id; b_13_rel0.RelationshipType = 0;
	b_13.RelationshipInfo.Relationships.Add(b_13_rel0); UsedBlocks->Add(b_13);
	auto b_14 = make(1000, FVector(13, 9, 3), FVector(1, 1, 1), FRotator(0, -180, -90), 1000, ""); b_14.HasElectricityData = true; b_14.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_14.HasRelationshipData = true; FGuid b_14_rel_id; FGuid::Parse("2EC04B3D4081FD6E3E1AB98141F4AD19", b_14_rel_id); b_14.RelationshipInfo.ID = b_14_rel_id; UsedBlocks->Add(b_14);
	auto b_15 = make(1000, FVector(-16, -10, 2), FVector(1, 1, 1), FRotator(0, -90, -90), 1000, ""); b_15.HasElectricityData = true; b_15.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_15.HasRelationshipData = true; FGuid b_15_rel_id; FGuid::Parse("4C5484244F03C6E7F8402D8363064A80", b_15_rel_id); b_15.RelationshipInfo.ID = b_15_rel_id; UsedBlocks->Add(b_15);
	auto b_16 = make(1000, FVector(4, -6, 4), FVector(1, 1, 1), FRotator(0, 90, 0), 1000, ""); b_16.HasElectricityData = true; b_16.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_16.HasRelationshipData = true; FGuid b_16_rel_id; FGuid::Parse("BAEA1C8045E0E3FAD809B593D9495942", b_16_rel_id); b_16.RelationshipInfo.ID = b_16_rel_id;
	FRelationshipInfo b_16_rel0; FGuid b_16_rel0_id; FGuid::Parse("797CEC8E499C456A5E1C46B8312736AB", b_16_rel0_id); b_16_rel0.TargetID = b_16_rel0_id; b_16_rel0.RelationshipType = 0;
	b_16.RelationshipInfo.Relationships.Add(b_16_rel0); UsedBlocks->Add(b_16);
	auto b_17 = make(1000, FVector(-11, -19, 4), FVector(1, 1, 1), FRotator(0, 90, 0), 1000, ""); b_17.HasElectricityData = true; b_17.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_17.HasRelationshipData = true; FGuid b_17_rel_id; FGuid::Parse("A1257C2E414F1607AB4F99A2E04D6E94", b_17_rel_id); b_17.RelationshipInfo.ID = b_17_rel_id;
	FRelationshipInfo b_17_rel0; FGuid b_17_rel0_id; FGuid::Parse("797CEC8E499C456A5E1C46B8312736AB", b_17_rel0_id); b_17_rel0.TargetID = b_17_rel0_id; b_17_rel0.RelationshipType = 0;
	b_17.RelationshipInfo.Relationships.Add(b_17_rel0); UsedBlocks->Add(b_17);
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
	auto b_45 = make(1100, FVector(11, 18, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_45.HasElectricityData = true; b_45.ElectricityInfo.CurrentObjectEnergy = 159595.000000;  UsedBlocks->Add(b_45);
	auto b_46 = make(1100, FVector(-9, 18, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_46.HasElectricityData = true; b_46.ElectricityInfo.CurrentObjectEnergy = 159709.000000;  UsedBlocks->Add(b_46);
	auto b_47 = make(1100, FVector(-9, 38, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_47.HasElectricityData = true; b_47.ElectricityInfo.CurrentObjectEnergy = 159353.000000;  UsedBlocks->Add(b_47);
	auto b_48 = make(1100, FVector(11, 38, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_48.HasElectricityData = true; b_48.ElectricityInfo.CurrentObjectEnergy = 159672.000000;  UsedBlocks->Add(b_48);
	auto b_49 = make(1100, FVector(31, 18, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_49.HasElectricityData = true; b_49.ElectricityInfo.CurrentObjectEnergy = 159338.000000;  UsedBlocks->Add(b_49);
	auto b_50 = make(1100, FVector(31, 38, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_50.HasElectricityData = true; b_50.ElectricityInfo.CurrentObjectEnergy = 159292.000000;  UsedBlocks->Add(b_50);
	auto b_51 = make(1100, FVector(31, 58, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_51.HasElectricityData = true; b_51.ElectricityInfo.CurrentObjectEnergy = 159627.000000;  UsedBlocks->Add(b_51);
	auto b_52 = make(1100, FVector(11, 59, 0), FVector(20, 20, 2), FRotator(0, -90, 0), 1100, ""); b_52.HasElectricityData = true; b_52.ElectricityInfo.CurrentObjectEnergy = 159665.000000;  UsedBlocks->Add(b_52);
	auto b_53 = make(1100, FVector(-9, 59, 0), FVector(20, 20, 2), FRotator(0, -90, 0), 1100, ""); b_53.HasElectricityData = true; b_53.ElectricityInfo.CurrentObjectEnergy = 159719.000000;  UsedBlocks->Add(b_53);
	auto b_54 = make(1050, FVector(-3, -9, 4), FVector(1, 1, 1), FRotator(0, 90, 0), 1050, "Tlacitko"); b_54.HasElectricityData = true; b_54.ElectricityInfo.CurrentObjectEnergy = 0.000000;
	b_54.HasRelationshipData = true; FGuid b_54_rel_id; FGuid::Parse("797CEC8E499C456A5E1C46B8312736AB", b_54_rel_id); b_54.RelationshipInfo.ID = b_54_rel_id;
	FRelationshipInfo b_54_rel0; FGuid b_54_rel0_id; FGuid::Parse("BAEA1C8045E0E3FAD809B593D9495942", b_54_rel0_id); b_54_rel0.TargetID = b_54_rel0_id; b_54_rel0.RelationshipType = 1;
	b_54.RelationshipInfo.Relationships.Add(b_54_rel0);
	FRelationshipInfo b_54_rel1; FGuid b_54_rel1_id; FGuid::Parse("703EFD1844A5E576A5932288A38E20B9", b_54_rel1_id); b_54_rel1.TargetID = b_54_rel1_id; b_54_rel1.RelationshipType = 1;
	b_54.RelationshipInfo.Relationships.Add(b_54_rel1);
	FRelationshipInfo b_54_rel2; FGuid b_54_rel2_id; FGuid::Parse("A1257C2E414F1607AB4F99A2E04D6E94", b_54_rel2_id); b_54_rel2.TargetID = b_54_rel2_id; b_54_rel2.RelationshipType = 1;
	b_54.RelationshipInfo.Relationships.Add(b_54_rel2);
	FRelationshipInfo b_54_rel3; FGuid b_54_rel3_id; FGuid::Parse("A9F299C54AB0F2B6ABEBC280C23BF218", b_54_rel3_id); b_54_rel3.TargetID = b_54_rel3_id; b_54_rel3.RelationshipType = 1;
	b_54.RelationshipInfo.Relationships.Add(b_54_rel3); UsedBlocks->Add(b_54);
	auto b_55 = make(1200, FVector(9, -2, 5), FVector(3, 3, 3), FRotator(0, 0, 0), 1200, ""); b_55.HasElectricityData = true; b_55.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_55);
	auto b_56 = make(600, FVector(24, -7, 11), FVector(5, 1, 4), FRotator(0, 90, 0), 600, ""); b_56.HasElectricityData = true; b_56.ElectricityInfo.CurrentObjectEnergy = 8000.000000;  UsedBlocks->Add(b_56);
	auto b_57 = make(600, FVector(24, -7, 15), FVector(5, 1, 4), FRotator(0, 90, 0), 600, ""); b_57.HasElectricityData = true; b_57.ElectricityInfo.CurrentObjectEnergy = 8000.000000;  UsedBlocks->Add(b_57);
	auto b_58 = make(600, FVector(24, -2, 11), FVector(5, 1, 4), FRotator(0, 90, 0), 600, ""); b_58.HasElectricityData = true; b_58.ElectricityInfo.CurrentObjectEnergy = 8000.000000;  UsedBlocks->Add(b_58);
	auto b_59 = make(600, FVector(24, -2, 15), FVector(5, 1, 4), FRotator(0, 90, 0), 600, ""); b_59.HasElectricityData = true; b_59.ElectricityInfo.CurrentObjectEnergy = 8000.000000;  UsedBlocks->Add(b_59);
	auto b_60 = make(600, FVector(24, 3, 11), FVector(5, 1, 4), FRotator(0, 90, 0), 600, ""); b_60.HasElectricityData = true; b_60.ElectricityInfo.CurrentObjectEnergy = 8000.000000;  UsedBlocks->Add(b_60);
	auto b_61 = make(600, FVector(24, 3, 15), FVector(5, 1, 4), FRotator(0, 90, 0), 600, ""); b_61.HasElectricityData = true; b_61.ElectricityInfo.CurrentObjectEnergy = 8000.000000;  UsedBlocks->Add(b_61);
	auto b_62 = make(1000, FVector(23, -20, 12), FVector(1, 1, 1), FRotator(0, 90, 90), 1000, ""); b_62.HasElectricityData = true; b_62.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_62.HasRelationshipData = true; FGuid b_62_rel_id; FGuid::Parse("053D69F542E0BAEAD45D9D9ED75F3844", b_62_rel_id); b_62.RelationshipInfo.ID = b_62_rel_id; UsedBlocks->Add(b_62);
	auto b_63 = make(1150, FVector(-12, 2, 4), FVector(3, 3, 2), FRotator(0, 0, 0), 1150, ""); b_63.HasElectricityData = true; b_63.ElectricityInfo.CurrentObjectEnergy = 18000.000000;
	b_63.HasRelationshipData = true; FGuid b_63_rel_id; FGuid::Parse("F26AD3854CEC7BCC30043DB8B94ECD9E", b_63_rel_id); b_63.RelationshipInfo.ID = b_63_rel_id; UsedBlocks->Add(b_63);
	auto b_64 = make(1150, FVector(-12, -1, 4), FVector(3, 3, 2), FRotator(0, 0, 0), 1150, ""); b_64.HasElectricityData = true; b_64.ElectricityInfo.CurrentObjectEnergy = 18000.000000;
	b_64.HasRelationshipData = true; FGuid b_64_rel_id; FGuid::Parse("F26AD3854CEC7BCC30043DB8B94ECD9E", b_64_rel_id); b_64.RelationshipInfo.ID = b_64_rel_id; UsedBlocks->Add(b_64);
	auto b_65 = make(1150, FVector(-9, 2, 4), FVector(3, 3, 2), FRotator(0, 0, 0), 1150, ""); b_65.HasElectricityData = true; b_65.ElectricityInfo.CurrentObjectEnergy = 18000.000000;
	b_65.HasRelationshipData = true; FGuid b_65_rel_id; FGuid::Parse("F26AD3854CEC7BCC30043DB8B94ECD9E", b_65_rel_id); b_65.RelationshipInfo.ID = b_65_rel_id; UsedBlocks->Add(b_65);
	auto b_66 = make(1150, FVector(-9, -1, 4), FVector(3, 3, 2), FRotator(0, 0, 0), 1150, ""); b_66.HasElectricityData = true; b_66.ElectricityInfo.CurrentObjectEnergy = 18000.000000;
	b_66.HasRelationshipData = true; FGuid b_66_rel_id; FGuid::Parse("F26AD3854CEC7BCC30043DB8B94ECD9E", b_66_rel_id); b_66.RelationshipInfo.ID = b_66_rel_id; UsedBlocks->Add(b_66);
	auto b_67 = make(1200, FVector(-10, -29, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_67.HasElectricityData = true; b_67.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_67);
	auto b_68 = make(1200, FVector(-5, -27, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_68.HasElectricityData = true; b_68.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_68);
	auto b_69 = make(101, FVector(21, -5, 4), FVector(1, 4, 1), FRotator(0, 0, 0), 101, ""); b_69.HasElectricityData = true; b_69.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_69);
	auto b_70 = make(101, FVector(21, 1, 4), FVector(1, 4, 1), FRotator(0, 0, 0), 101, ""); b_70.HasElectricityData = true; b_70.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_70);
	auto b_71 = make(1200, FVector(22, -29, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_71.HasElectricityData = true; b_71.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_71);
	auto b_72 = make(1200, FVector(19, -29, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_72.HasElectricityData = true; b_72.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_72);
	auto b_73 = make(1200, FVector(16, -29, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_73.HasElectricityData = true; b_73.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_73);
	auto b_74 = make(1200, FVector(13, -29, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_74.HasElectricityData = true; b_74.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_74);
	auto b_75 = make(1200, FVector(22, -29, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_75.HasElectricityData = true; b_75.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_75);
	auto b_76 = make(1200, FVector(19, -29, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_76.HasElectricityData = true; b_76.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_76);
	auto b_77 = make(1200, FVector(22, -23, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_77.HasElectricityData = true; b_77.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_77);
	auto b_78 = make(1200, FVector(22, -23, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_78.HasElectricityData = true; b_78.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_78);
	auto b_79 = make(1200, FVector(22, -20, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_79.HasElectricityData = true; b_79.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_79);
	auto b_80 = make(1200, FVector(22, -20, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_80.HasElectricityData = true; b_80.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_80);
	auto b_81 = make(1200, FVector(22, -17, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_81.HasElectricityData = true; b_81.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_81);
	auto b_82 = make(1200, FVector(22, -17, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_82.HasElectricityData = true; b_82.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_82);
	auto b_83 = make(1200, FVector(22, -14, 5), FVector(3, 3, 3), FRotator(0, 0, 0), 1200, ""); b_83.HasElectricityData = true; b_83.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_83);
	auto b_84 = make(1200, FVector(22, -14, 8), FVector(3, 3, 3), FRotator(0, 0, 0), 1200, ""); b_84.HasElectricityData = true; b_84.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_84);
	auto b_85 = make(1500, FVector(7, -9, 4), FVector(2, 2, 2), FRotator(0, 0, 0), 1500, ""); b_85.HasOxygenData = true; b_85.OxygenInfo.CurrentObjectOxygen = 0.000000;  UsedBlocks->Add(b_85);
	auto b_86 = make(1400, FVector(22, -10, 5), FVector(4, 3, 4), FRotator(0, 90, 0), 1400, "Plnicka"); b_86.HasElectricityData = true; b_86.ElectricityInfo.CurrentObjectEnergy = 150.000000; b_86.HasOxygenData = true; b_86.OxygenInfo.CurrentObjectOxygen = 150.000000;
	b_86.HasRelationshipData = true; FGuid b_86_rel_id; FGuid::Parse("A9F299C54AB0F2B6ABEBC280C23BF218", b_86_rel_id); b_86.RelationshipInfo.ID = b_86_rel_id;
	FRelationshipInfo b_86_rel0; FGuid b_86_rel0_id; FGuid::Parse("797CEC8E499C456A5E1C46B8312736AB", b_86_rel0_id); b_86_rel0.TargetID = b_86_rel0_id; b_86_rel0.RelationshipType = 0;
	b_86.RelationshipInfo.Relationships.Add(b_86_rel0); UsedBlocks->Add(b_86);
	auto b_87 = make(1050, FVector(23, -11, 11), FVector(1, 1, 1), FRotator(0, 90, 90), 1050, "Vypinac plnicka"); b_87.HasElectricityData = true; b_87.ElectricityInfo.CurrentObjectEnergy = 0.000000;
	b_87.HasRelationshipData = true; FGuid b_87_rel_id; FGuid::Parse("6EEA645949C55B8F90F421AB704204C1", b_87_rel_id); b_87.RelationshipInfo.ID = b_87_rel_id; UsedBlocks->Add(b_87);
	c->PlayerPosition = FVector(253, -175, 163);
	c->PlayerRotation = FRotator(0, -12, 0);

	/*c->PlayerPosition = FVector(-214, -227, 163);
	c->PlayerRotation = FRotator(0, -32, 0);
*/
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



	c->PlayerOxygenComponent.CurrentObjectOxygen = 150.0f;
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
	auto b_12 = make(1000, FVector(19, -32, 8), FVector(1, 1, 1), FRotator(0, 0, -90), 1000, ""); b_12.HasElectricityData = true; b_12.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_12.HasRelationshipData = true; FGuid b_12_rel_id; FGuid::Parse("A76B52884FBC2B0268FA6FA273A5C030", b_12_rel_id); b_12.RelationshipInfo.ID = b_12_rel_id; UsedBlocks->Add(b_12);
	auto b_13 = make(1000, FVector(25, -13, 8), FVector(1, 1, 1), FRotator(0, 90, -90), 1000, ""); b_13.HasElectricityData = true; b_13.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_13.HasRelationshipData = true; FGuid b_13_rel_id; FGuid::Parse("703EFD1844A5E576A5932288A38E20B9", b_13_rel_id); b_13.RelationshipInfo.ID = b_13_rel_id;
	FRelationshipInfo b_13_rel0; FGuid b_13_rel0_id; FGuid::Parse("797CEC8E499C456A5E1C46B8312736AB", b_13_rel0_id); b_13_rel0.TargetID = b_13_rel0_id; b_13_rel0.RelationshipType = 0;
	b_13.RelationshipInfo.Relationships.Add(b_13_rel0); UsedBlocks->Add(b_13);
	auto b_14 = make(1000, FVector(13, 9, 3), FVector(1, 1, 1), FRotator(0, -180, -90), 1000, ""); b_14.HasElectricityData = true; b_14.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_14.HasRelationshipData = true; FGuid b_14_rel_id; FGuid::Parse("2EC04B3D4081FD6E3E1AB98141F4AD19", b_14_rel_id); b_14.RelationshipInfo.ID = b_14_rel_id; UsedBlocks->Add(b_14);
	auto b_15 = make(1000, FVector(-16, -10, 2), FVector(1, 1, 1), FRotator(0, -90, -90), 1000, ""); b_15.HasElectricityData = true; b_15.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_15.HasRelationshipData = true; FGuid b_15_rel_id; FGuid::Parse("4C5484244F03C6E7F8402D8363064A80", b_15_rel_id); b_15.RelationshipInfo.ID = b_15_rel_id; UsedBlocks->Add(b_15);
	auto b_16 = make(1000, FVector(4, -6, 4), FVector(1, 1, 1), FRotator(0, 90, 0), 1000, ""); b_16.HasElectricityData = true; b_16.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_16.HasRelationshipData = true; FGuid b_16_rel_id; FGuid::Parse("BAEA1C8045E0E3FAD809B593D9495942", b_16_rel_id); b_16.RelationshipInfo.ID = b_16_rel_id;
	FRelationshipInfo b_16_rel0; FGuid b_16_rel0_id; FGuid::Parse("797CEC8E499C456A5E1C46B8312736AB", b_16_rel0_id); b_16_rel0.TargetID = b_16_rel0_id; b_16_rel0.RelationshipType = 0;
	b_16.RelationshipInfo.Relationships.Add(b_16_rel0); UsedBlocks->Add(b_16);
	auto b_17 = make(1000, FVector(-11, -19, 4), FVector(1, 1, 1), FRotator(0, 90, 0), 1000, ""); b_17.HasElectricityData = true; b_17.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_17.HasRelationshipData = true; FGuid b_17_rel_id; FGuid::Parse("A1257C2E414F1607AB4F99A2E04D6E94", b_17_rel_id); b_17.RelationshipInfo.ID = b_17_rel_id;
	FRelationshipInfo b_17_rel0; FGuid b_17_rel0_id; FGuid::Parse("797CEC8E499C456A5E1C46B8312736AB", b_17_rel0_id); b_17_rel0.TargetID = b_17_rel0_id; b_17_rel0.RelationshipType = 0;
	b_17.RelationshipInfo.Relationships.Add(b_17_rel0); UsedBlocks->Add(b_17);
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
	auto b_45 = make(1100, FVector(11, 18, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_45.HasElectricityData = true; b_45.ElectricityInfo.CurrentObjectEnergy = 159405.000000;  UsedBlocks->Add(b_45);
	auto b_46 = make(1100, FVector(-9, 18, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_46.HasElectricityData = true; b_46.ElectricityInfo.CurrentObjectEnergy = 159709.000000;  UsedBlocks->Add(b_46);
	auto b_47 = make(1100, FVector(-9, 38, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_47.HasElectricityData = true; b_47.ElectricityInfo.CurrentObjectEnergy = 159353.000000;  UsedBlocks->Add(b_47);
	auto b_48 = make(1100, FVector(11, 38, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_48.HasElectricityData = true; b_48.ElectricityInfo.CurrentObjectEnergy = 159672.000000;  UsedBlocks->Add(b_48);
	auto b_49 = make(1100, FVector(31, 18, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_49.HasElectricityData = true; b_49.ElectricityInfo.CurrentObjectEnergy = 159662.000000;  UsedBlocks->Add(b_49);
	auto b_50 = make(1100, FVector(31, 38, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_50.HasElectricityData = true; b_50.ElectricityInfo.CurrentObjectEnergy = 159708.000000;  UsedBlocks->Add(b_50);
	auto b_51 = make(1100, FVector(31, 58, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_51.HasElectricityData = true; b_51.ElectricityInfo.CurrentObjectEnergy = 159627.000000;  UsedBlocks->Add(b_51);
	auto b_52 = make(1100, FVector(11, 59, 0), FVector(20, 20, 2), FRotator(0, -90, 0), 1100, ""); b_52.HasElectricityData = true; b_52.ElectricityInfo.CurrentObjectEnergy = 159665.000000;  UsedBlocks->Add(b_52);
	auto b_53 = make(1100, FVector(-9, 59, 0), FVector(20, 20, 2), FRotator(0, -90, 0), 1100, ""); b_53.HasElectricityData = true; b_53.ElectricityInfo.CurrentObjectEnergy = 159719.000000;  UsedBlocks->Add(b_53);
	auto b_54 = make(1050, FVector(-3, -9, 4), FVector(1, 1, 1), FRotator(0, 90, 0), 1050, "Tlacitko"); b_54.HasElectricityData = true; b_54.ElectricityInfo.CurrentObjectEnergy = 0.000000;
	b_54.HasRelationshipData = true; FGuid b_54_rel_id; FGuid::Parse("797CEC8E499C456A5E1C46B8312736AB", b_54_rel_id); b_54.RelationshipInfo.ID = b_54_rel_id;
	FRelationshipInfo b_54_rel0; FGuid b_54_rel0_id; FGuid::Parse("BAEA1C8045E0E3FAD809B593D9495942", b_54_rel0_id); b_54_rel0.TargetID = b_54_rel0_id; b_54_rel0.RelationshipType = 1;
	b_54.RelationshipInfo.Relationships.Add(b_54_rel0);
	FRelationshipInfo b_54_rel1; FGuid b_54_rel1_id; FGuid::Parse("703EFD1844A5E576A5932288A38E20B9", b_54_rel1_id); b_54_rel1.TargetID = b_54_rel1_id; b_54_rel1.RelationshipType = 1;
	b_54.RelationshipInfo.Relationships.Add(b_54_rel1);
	FRelationshipInfo b_54_rel2; FGuid b_54_rel2_id; FGuid::Parse("A1257C2E414F1607AB4F99A2E04D6E94", b_54_rel2_id); b_54_rel2.TargetID = b_54_rel2_id; b_54_rel2.RelationshipType = 1;
	b_54.RelationshipInfo.Relationships.Add(b_54_rel2); UsedBlocks->Add(b_54);
	auto b_55 = make(1200, FVector(9, -2, 5), FVector(3, 3, 3), FRotator(0, 0, 0), 1200, ""); b_55.HasElectricityData = true; b_55.ElectricityInfo.CurrentObjectEnergy = 100000.000000; b_55.HasOxygenData = true; b_55.OxygenInfo.CurrentObjectOxygen = 0.000000;  UsedBlocks->Add(b_55);
	auto b_56 = make(600, FVector(24, -7, 11), FVector(5, 1, 4), FRotator(0, 90, 0), 600, ""); b_56.HasElectricityData = true; b_56.ElectricityInfo.CurrentObjectEnergy = 8000.000000; b_56.HasOxygenData = true; b_56.OxygenInfo.CurrentObjectOxygen = 0.000000;  UsedBlocks->Add(b_56);
	auto b_57 = make(600, FVector(24, -7, 15), FVector(5, 1, 4), FRotator(0, 90, 0), 600, ""); b_57.HasElectricityData = true; b_57.ElectricityInfo.CurrentObjectEnergy = 8000.000000; b_57.HasOxygenData = true; b_57.OxygenInfo.CurrentObjectOxygen = 0.000000;  UsedBlocks->Add(b_57);
	auto b_58 = make(600, FVector(24, -2, 11), FVector(5, 1, 4), FRotator(0, 90, 0), 600, ""); b_58.HasElectricityData = true; b_58.ElectricityInfo.CurrentObjectEnergy = 8000.000000; b_58.HasOxygenData = true; b_58.OxygenInfo.CurrentObjectOxygen = 0.000000;  UsedBlocks->Add(b_58);
	auto b_59 = make(600, FVector(24, -2, 15), FVector(5, 1, 4), FRotator(0, 90, 0), 600, ""); b_59.HasElectricityData = true; b_59.ElectricityInfo.CurrentObjectEnergy = 8000.000000; b_59.HasOxygenData = true; b_59.OxygenInfo.CurrentObjectOxygen = 0.000000;  UsedBlocks->Add(b_59);
	auto b_60 = make(600, FVector(24, 3, 11), FVector(5, 1, 4), FRotator(0, 90, 0), 600, ""); b_60.HasElectricityData = true; b_60.ElectricityInfo.CurrentObjectEnergy = 8000.000000; b_60.HasOxygenData = true; b_60.OxygenInfo.CurrentObjectOxygen = 0.000000;  UsedBlocks->Add(b_60);
	auto b_61 = make(600, FVector(24, 3, 15), FVector(5, 1, 4), FRotator(0, 90, 0), 600, ""); b_61.HasElectricityData = true; b_61.ElectricityInfo.CurrentObjectEnergy = 8000.000000; b_61.HasOxygenData = true; b_61.OxygenInfo.CurrentObjectOxygen = 0.000000;  UsedBlocks->Add(b_61);
	auto b_62 = make(1000, FVector(23, -20, 12), FVector(1, 1, 1), FRotator(0, 90, 90), 1000, ""); b_62.HasElectricityData = true; b_62.ElectricityInfo.CurrentObjectEnergy = 200.000000; b_62.HasOxygenData = true; b_62.OxygenInfo.CurrentObjectOxygen = 0.000000;
	b_62.HasRelationshipData = true; FGuid b_62_rel_id; FGuid::Parse("053D69F542E0BAEAD45D9D9ED75F3844", b_62_rel_id); b_62.RelationshipInfo.ID = b_62_rel_id; UsedBlocks->Add(b_62);
	auto b_63 = make(1150, FVector(-12, 2, 4), FVector(3, 3, 2), FRotator(0, 0, 0), 1150, ""); b_63.HasElectricityData = true; b_63.ElectricityInfo.CurrentObjectEnergy = 18000.000000;
	b_63.HasRelationshipData = true; FGuid b_63_rel_id; FGuid::Parse("F26AD3854CEC7BCC30043DB8B94ECD9E", b_63_rel_id); b_63.RelationshipInfo.ID = b_63_rel_id; UsedBlocks->Add(b_63);
	auto b_64 = make(1150, FVector(-12, -1, 4), FVector(3, 3, 2), FRotator(0, 0, 0), 1150, ""); b_64.HasElectricityData = true; b_64.ElectricityInfo.CurrentObjectEnergy = 18000.000000;
	b_64.HasRelationshipData = true; FGuid b_64_rel_id; FGuid::Parse("F26AD3854CEC7BCC30043DB8B94ECD9E", b_64_rel_id); b_64.RelationshipInfo.ID = b_64_rel_id; UsedBlocks->Add(b_64);
	auto b_65 = make(1150, FVector(-9, 2, 4), FVector(3, 3, 2), FRotator(0, 0, 0), 1150, ""); b_65.HasElectricityData = true; b_65.ElectricityInfo.CurrentObjectEnergy = 18000.000000;
	b_65.HasRelationshipData = true; FGuid b_65_rel_id; FGuid::Parse("F26AD3854CEC7BCC30043DB8B94ECD9E", b_65_rel_id); b_65.RelationshipInfo.ID = b_65_rel_id; UsedBlocks->Add(b_65);
	auto b_66 = make(1150, FVector(-9, -1, 4), FVector(3, 3, 2), FRotator(0, 0, 0), 1150, ""); b_66.HasElectricityData = true; b_66.ElectricityInfo.CurrentObjectEnergy = 18000.000000;
	b_66.HasRelationshipData = true; FGuid b_66_rel_id; FGuid::Parse("F26AD3854CEC7BCC30043DB8B94ECD9E", b_66_rel_id); b_66.RelationshipInfo.ID = b_66_rel_id; UsedBlocks->Add(b_66);
	auto b_67 = make(1200, FVector(-10, -29, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_67.HasElectricityData = true; b_67.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_67);
	auto b_68 = make(1200, FVector(-5, -27, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_68.HasElectricityData = true; b_68.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_68);
	auto b_69 = make(101, FVector(21, -5, 4), FVector(1, 4, 1), FRotator(0, 0, 0), 101, ""); b_69.HasElectricityData = true; b_69.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_69);
	auto b_70 = make(101, FVector(21, 1, 4), FVector(1, 4, 1), FRotator(0, 0, 0), 101, ""); b_70.HasElectricityData = true; b_70.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_70);
	auto b_71 = make(1200, FVector(22, -29, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_71.HasElectricityData = true; b_71.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_71);
	auto b_72 = make(1200, FVector(19, -29, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_72.HasElectricityData = true; b_72.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_72);
	auto b_73 = make(1200, FVector(16, -29, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_73.HasElectricityData = true; b_73.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_73);
	auto b_74 = make(1200, FVector(13, -29, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_74.HasElectricityData = true; b_74.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_74);
	auto b_75 = make(1200, FVector(22, -29, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_75.HasElectricityData = true; b_75.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_75);
	auto b_76 = make(1200, FVector(19, -29, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_76.HasElectricityData = true; b_76.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_76);
	auto b_77 = make(1200, FVector(22, -23, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_77.HasElectricityData = true; b_77.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_77);
	auto b_78 = make(1200, FVector(22, -23, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_78.HasElectricityData = true; b_78.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_78);
	auto b_79 = make(1200, FVector(22, -20, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_79.HasElectricityData = true; b_79.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_79);
	auto b_80 = make(1200, FVector(22, -20, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_80.HasElectricityData = true; b_80.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_80);
	auto b_81 = make(1200, FVector(22, -17, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_81.HasElectricityData = true; b_81.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_81);
	auto b_82 = make(1200, FVector(22, -17, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_82.HasElectricityData = true; b_82.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_82);
	auto b_83 = make(1200, FVector(22, -14, 5), FVector(3, 3, 3), FRotator(0, 0, 0), 1200, ""); b_83.HasElectricityData = true; b_83.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_83);
	auto b_84 = make(1200, FVector(22, -14, 8), FVector(3, 3, 3), FRotator(0, 0, 0), 1200, ""); b_84.HasElectricityData = true; b_84.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_84);
	auto b_85 = make(1500, FVector(7, -9, 4), FVector(2, 2, 2), FRotator(0, 0, 0), 1500, ""); b_85.HasOxygenData = true; b_85.OxygenInfo.CurrentObjectOxygen = 0.000000;  UsedBlocks->Add(b_85);



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