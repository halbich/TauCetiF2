#pragma once

// Získání hodnoty
//FInternationalization::ForUseOnlyByLocMacroAndGraphNodeTextLiterals_CreateText(*SourceString, *Namespace, *KeyString);



#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.SaveSystemDefault", "Default game")
#define LC_SystemSave_Empty					NSLOCTEXT("TauCetiF2LocSpace", "LC.SaveSystemEmpty", "Empty game")
#define LC_SystemSave_NoSaves				NSLOCTEXT("TauCetiF2LocSpace", "LC.NoSavedSavesFound", "No saves found!")

#define LC_DoorOpen							NSLOCTEXT("TauCetiF2LocSpace", "LC.DoorOpen", "Open door")
#define LC_DoorClose						NSLOCTEXT("TauCetiF2LocSpace", "LC.DoorClose", "Close door")

#define LC_DoorOpening						NSLOCTEXT("TauCetiF2LocSpace", "LC.DoorOpening", "Door opening")
#define LC_DoorOpeningLeft					NSLOCTEXT("TauCetiF2LocSpace", "LC.DoorOpeningLeft", "Left")
#define LC_DoorOpeningRight					NSLOCTEXT("TauCetiF2LocSpace", "LC.DoorOpeningRight", "Right")



#define LC_Settings							NSLOCTEXT("TauCetiF2LocSpace", "LC.Settings", "Settings")

#define LC_Settings_Epic					NSLOCTEXT("TauCetiF2LocSpace", "LC.SettingsEpic", "Epic")
#define LC_Settings_High					NSLOCTEXT("TauCetiF2LocSpace", "LC.SettingsHigh", "High")
#define LC_Settings_Med						NSLOCTEXT("TauCetiF2LocSpace", "LC.SettingsMedium", "Med")
#define LC_Settings_Low						NSLOCTEXT("TauCetiF2LocSpace", "LC.SettingsLow", "Low")

#define LC_Settings_VSync					NSLOCTEXT("TauCetiF2LocSpace", "LC.VSync", "V-Sync")
#define LC_Settings_AutoQuality				NSLOCTEXT("TauCetiF2LocSpace", "LC.SetAutomaticQuality", "Set quality automatically")

#define LC_Settings_Far						NSLOCTEXT("TauCetiF2LocSpace", "LC.SettingsFar", "Far")
#define LC_Settings_Near					NSLOCTEXT("TauCetiF2LocSpace", "LC.SettingsNear", "Near")

#define LC_Settings_FS						NSLOCTEXT("TauCetiF2LocSpace", "LC.FullScreen", "Fullscreen")
#define LC_Settings_FS0						NSLOCTEXT("TauCetiF2LocSpace", "LC.SettingsFS0", "Yes")
#define LC_Settings_FS1						NSLOCTEXT("TauCetiF2LocSpace", "LC.SettingsFS1", "Window")
#define LC_Settings_FS2						NSLOCTEXT("TauCetiF2LocSpace", "LC.SettingsFS2", "No")

#define LC_Settings_AA						NSLOCTEXT("TauCetiF2LocSpace", "LC.AntiAliasing", "Anti-Aliasing")
#define LC_Settings_Shadows					NSLOCTEXT("TauCetiF2LocSpace", "LC.Shadows", "Shadows")
#define LC_Settings_PP						NSLOCTEXT("TauCetiF2LocSpace", "LC.PostProcess", "Post Processing")
#define LC_Settings_Resolution				NSLOCTEXT("TauCetiF2LocSpace", "LC.ScreenResolution", "Screen Resolution")
#define LC_Settings_Overal					NSLOCTEXT("TauCetiF2LocSpace", "LC.OveralSettings", "Overal settings")
#define LC_Settings_Custom					NSLOCTEXT("TauCetiF2LocSpace", "LC.SettingsCustom", "Custom")


#define LC_Inventory_GroupName				NSLOCTEXT("TauCetiF2LocSpace", "LC.InventoryGroupName", "Group name")

//TODO
/*
#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.DeleteGroup", "Delete group")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.Tags", "Tags")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.Quit", "Quit")



#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.AboutText", "TODO Text")


#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.NewGame", "New game")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.Textures", "Textures")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.CloseBtn", "Close")


#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.SaveNameMustBeNonEmpty", "Name must not be empty!")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.ResolutionScale", "Resolution Scale")


#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.BtnCancel", "Cancel")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.ReallyDeleteSavedGame", "Do you really want to delete this game?")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.SaveGame", "Save game")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.About", "About")


#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.NewTagHint", "New tag")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.ReallyQuitGame", "Do you really want to quit game?")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.MainMenu", "Main menu")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.NameShort", "Name")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.NewSaveName", "New save name")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.BtnOK", "OK")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "{x} - {y} - {z}", "{x} - {y} - {z}")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.SaveSavedDate", "Saved date:")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.Delete", "Delete")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.ReallyQuitCurrentGameWithUnsavedItemsLost", "Do you really want to quit game? All unsaved progress will be lost!")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.LoadingDone", "Loading done")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.Efects", "Effects")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.BtnYes", "Yes")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.ReallyGotoMainMenuWithUnsavedItemsLost", "Do you really want to go to main menu? All unsaved progress will be lost!")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.ViewDistance", "View distance")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.InventoryGroupEnabled", "Group enabled")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.AddParams", "Parameters")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.UseItem", "Use item")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "Nepovedlo se pøidat", "Nepovedlo se pøidat")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.ReallyQuitGameWithUnsavedItemsLost", "Do you really want to quit game? All unsaved progress will be lost!")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.NewTagsHint", "New tags")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.SavePlayedTime", "Played time:")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.ReallyOverwriteSavedGame", "Do you really want to overwrite this game?")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.LanguageName", "Language")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.SelectItem", "Select item")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "No Tags to set", "No Params to set")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.NewGroupHint", "New group name")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.LoadBtn", "Load")


#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.SaveName", "Save name:")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.BlockNameHint", "Block name")


#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.BtnNo", "No")




#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.GroupNameHint", "Group name")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.LoadGame", "Load game")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.NewSaveNameHint", "Save name")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "Save OK", "Save OK")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.SaveBtn", "Save")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "{Count}%    {Message}", "{Count}%    {Message}")



#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.BeginPlayBtn", "Begin play")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.Load", "Load")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.LoadingGame", "Loading game")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.StartNewGame", "Start new game")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.DontCollapseGroup", "Don't collapse group")


#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "{0}%", "{0}%")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.LoadingPreparingWorld", "Preparing world")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.Properties", "Properties")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.Terminal", "Terminal")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "{0} ({1})", "{0} ({1})")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "LC.BlockConstructor", "Block constructor")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "Save fail", "Save fail")

#define LC_SystemSave_Default				NSLOCTEXT("TauCetiF2LocSpace", "Pøidáno", "Pøidáno")

#define LC_Generic_NDF						NSLOCTEXT("TauCetiF2LocSpace", "NDF", "NDF")
*/