#include "SmartDialogueEditor.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "SmartDialogueActions.h"

#define LOCTEXT_NAMESPACE "SmartDialogueEditor"

void FSmartDialogueEditorModule::StartupModule()
{

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	EAssetTypeCategories::Type DialogueCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("SmartDialogue")), LOCTEXT("SmartDialogueCategory", "Smart Dialogue"));

	TSharedPtr<IAssetTypeActions> SmartDialogueAction = MakeShareable(new FSmartDialogueActions(DialogueCategoryBit));
	AssetTools.RegisterAssetTypeActions(SmartDialogueAction.ToSharedRef());
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
