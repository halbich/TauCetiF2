using UnrealBuildTool;

public class Blocks : ModuleRules
{
    public Blocks(ReadOnlyTargetRules ROTRules) : base(ROTRules)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UMG", "Slate", "SlateCore", "Commons", "GameSave" });
        PrivateDependencyModuleNames.AddRange(new string[] { "RenderCore", "PakFile", "StreamingFile", "RHI" });

        PublicIncludePaths.AddRange(new string[] { "Blocks/Public", "Blocks/Public/Definitions", "Blocks/Public/Interfaces" });

        PrivateIncludePaths.AddRange(new string[] { "Blocks/Private" });
    }
}