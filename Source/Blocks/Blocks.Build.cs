using UnrealBuildTool;

public class Blocks : ModuleRules
{
    public Blocks(TargetInfo Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { "RenderCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { "PakFile", "StreamingFile" });


        PublicIncludePaths.AddRange(new string[] { "Blocks/Public" });

        PrivateIncludePaths.AddRange(new string[] { "Blocks/Private" });


    }
}