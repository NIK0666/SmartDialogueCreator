#include "SmartDialogueEditor.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "SmartDialogueActions.h"

#define LOCTEXT_NAMESPACE "SmartDialogueEditor"

void FSmartDialogueEditorModule::StartupModule()
{

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type SmartDialogueCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Smart Dialogue")), LOCTEXT("Smart Dialogue", "Smart Dialogue"));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FSmartDialogueActions(SmartDialogueCategoryBit)));

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
