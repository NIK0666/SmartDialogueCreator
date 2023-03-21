#include "SmartDialogueEditor.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "SmartDialogueActions.h"

#define LOCTEXT_NAMESPACE "SmartDialogueEditor"

void FSmartDialogueEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	TSharedPtr<FAssetTypeActions_Base> SmartDialogueActions = MakeShareable(new FSmartDialogueActions());
	AssetTools.RegisterAssetTypeActions(SmartDialogueActions.ToSharedRef());
	RegisteredAssetTypeActions.Add(SmartDialogueActions);
}

void FSmartDialogueEditorModule::ShutdownModule()
{
	for (auto& Actions : RegisteredAssetTypeActions)
	{
		FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get().UnregisterAssetTypeActions(Actions.ToSharedRef());
	}
	RegisteredAssetTypeActions.Empty();
}

IMPLEMENT_PRIMARY_GAME_MODULE(FSmartDialogueEditorModule, SmartDialogueEditor, "SmartDialogueEditor");
