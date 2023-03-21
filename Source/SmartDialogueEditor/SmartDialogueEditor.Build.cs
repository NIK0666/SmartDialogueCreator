using UnrealBuildTool;

public class SmartDialogueEditor : ModuleRules
{
    public SmartDialogueEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "SmartDialogueCore",
                "CoreUObject", 
                "Engine", 
                "InputCore", 
                "UnrealEd", 
                "Slate", 
                "SlateCore", 
                "AssetTools", 
                "ContentBrowser", 
                "WorkspaceMenuStructure",
            }
        );
    }
}