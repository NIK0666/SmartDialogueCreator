#include "SmartDialogueEditor.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "SmartDialogueActions.h"

#define LOCTEXT_NAMESPACE "SmartDialogueEditor"

void FSmartDialogueEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	// Регистрируем категорию Smart Dialogue
	EAssetTypeCategories::Type SmartDialogueCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Smart Dialogue")), FText::FromString(TEXT("Smart Dialogue")));

	// Регистрируем тип ассета
	TSharedPtr<IAssetTypeActions> SmartDialogueAssetType = MakeShareable(new FSmartDialogueActions(SmartDialogueCategoryBit));
	AssetTools.RegisterAssetTypeActions(SmartDialogueAssetType.ToSharedRef());
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
