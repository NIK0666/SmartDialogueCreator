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

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Core", 
            "CoreUObject", 
            "Engine", 
            "Slate", 
            "SlateCore",
            "InputCore", 
            "EditorStyle",
            "UnrealEd", 
            "AssetTools",
            "SmartDialogueCore"
        });

    }
}