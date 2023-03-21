#include "SmartDialogueEditor.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "AssetTypeActions_SmartDialogue.h"

#define LOCTEXT_NAMESPACE "SmartDialogueEditor"

void FSmartDialogueEditorModule::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("Load Module: SmartDialogueEditor"));

	// Register asset types
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	SmartDialogueCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("SmartDialogueAsset")), LOCTEXT("SmartDialogueAssetCategory", "SmartDialogue"));

	TSharedRef<IAssetTypeActions> Action = MakeShareable(new FAssetTypeActions_SmartDialogue(SmartDialogueCategoryBit));
	AssetTools.RegisterAssetTypeActions(Action);
	RegisteredAssetTypeActions.Add(Action);
}

void FSmartDialogueEditorModule::ShutdownModule()
{
	UE_LOG(LogTemp, Log, TEXT("Unload Module: CustomAssetEditor"));

	// Unregister all the asset types that we registered
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (int32 Index = 0; Index < RegisteredAssetTypeActions.Num(); ++Index)
		{
			AssetTools.UnregisterAssetTypeActions(RegisteredAssetTypeActions[Index].ToSharedRef());
		}
	}
	RegisteredAssetTypeActions.Empty();
}

IMPLEMENT_PRIMARY_GAME_MODULE(FSmartDialogueEditorModule, SmartDialogueEditor, "SmartDialogueEditor");

#undef LOCTEXT_NAMESPACE