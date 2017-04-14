// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TauCetiF2 : ModuleRules
{
    public TauCetiF2(TargetInfo Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "Slate", "SlateCore",
            "Commons", "Blocks", "GameSave", "Inventory" });

        PublicIncludePaths.AddRange(new string[] { "TauCetiF2/Public" });

        PrivateIncludePaths.AddRange(new string[] { "TauCetiF2/Private" });
    }
}