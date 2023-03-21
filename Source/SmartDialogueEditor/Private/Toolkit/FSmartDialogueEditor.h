// 

#pragma once

#include "CoreMinimal.h"
#include "SmartDialogueData.h"
#include "Toolkits/AssetEditorToolkit.h"

class USmartDialogue;

class SMARTDIALOGUEEDITOR_API FSmartDialogueEditor final : public FAssetEditorToolkit, public FNotifyHook
{
public:
	virtual FName GetToolkitFName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	
	void InitSmartDialogueEditor(EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost >& InitToolkitHost, USmartDialogue* SmartDialogue);

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	
private:

	TSharedRef<SDockTab> HandleTabManagerSpawnTabDialogueBranches(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> HandleTabManagerSpawnTabDialogueBranchDetails(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> HandleTabManagerSpawnTabDialoguePhrasesDetails(const FSpawnTabArgs& Args);

	TSharedRef<SDockTab> SpawnTab_BranchesList(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_SelectedBranchProperties(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_SelectedBranchPhrases(const FSpawnTabArgs& Args);

	TSharedRef<SWidget> CreateBranchesListWidget();
	TSharedRef<SWidget> CreateSelectedBranchPropertiesWidget();
	TSharedRef<SWidget> CreateSelectedBranchPhrasesWidget();
	TSharedRef<FTabManager::FLayout> GetDefaultTabContents();


	TSharedPtr<SDockTab> BranchesListTab;
	TSharedPtr<SDockTab> SelectedBranchPropertiesTab;
	TSharedPtr<SDockTab> SelectedBranchPhrasesTab;
	
	USmartDialogue* EditingObject;
	
	TSharedPtr<STreeView<TSharedPtr<FSmartDialogueBranch>>> DialogueBranchesList;
	TSharedPtr<IDetailsView> DialogueBranchDetailsView;
	TSharedPtr<IDetailsView> DialoguePhrasesDetailsView;
};
