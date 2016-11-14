using UnrealBuildTool;

public class GameWorld : ModuleRules
{
    public GameWorld(TargetInfo Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { "RenderCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { "PakFile", "StreamingFile" });


        PublicIncludePaths.AddRange(new string[] { "GameWorld/Public" });

        PrivateIncludePaths.AddRange(new string[] { "GameWorld/Private" });


    }
}