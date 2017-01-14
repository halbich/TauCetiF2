using UnrealBuildTool;

public class Blocks : ModuleRules
{
    public Blocks(TargetInfo Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Commons", "GameSave" });
        PrivateDependencyModuleNames.AddRange(new string[] { "RenderCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { "PakFile", "StreamingFile" });


        PublicIncludePaths.AddRange(new string[] { "Blocks/Public", "Blocks/Public/Definitions"});

        PrivateIncludePaths.AddRange(new string[] { "Blocks/Private" });


    }
}