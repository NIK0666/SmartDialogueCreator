// 

#pragma once

#include "CoreMinimal.h"
#include "SmartDialogueData.h"
#include "Toolkits/AssetEditorToolkit.h"

class SBranchInfoWidget;
class SBranchesWidget;
class USmartDialogue;

DECLARE_DELEGATE_OneParam(FOnBranchItemAdded, FSmartDialogueBranch&);
DECLARE_DELEGATE_OneParam(FOnBranchItemDeleted, FSmartDialogueBranch&);


class SMARTDIALOGUEEDITOR_API FSmartDialogueEditor final : public FAssetEditorToolkit, public FNotifyHook
{
public:
	
	void InitSmartDialogueEditor(EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost >& InitToolkitHost, USmartDialogue* SmartDialogue);
	void SetDialogue(USmartDialogue* InDialogue);
	USmartDialogue* GetDialogue();
	void BindCommands();
	TSharedPtr<FExtender> GetToolbarExtender();

	FOnBranchItemAdded OnBranchItemAdded;
	FOnBranchItemDeleted OnBranchItemDeleted;

	
	virtual FName GetToolkitFName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	
private:

	TSharedRef<SDockTab> HandleTabManagerSpawnTabDialogueBranches(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> HandleTabManagerSpawnTabDialogueBranchDetails(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> HandleTabManagerSpawnTabDialoguePhrasesDetails(const FSpawnTabArgs& Args);

	TSharedRef<SDockTab> SpawnTab_BranchesList(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_SelectedBranchProperties(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_SelectedBranchPhrases(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Config(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Player(const FSpawnTabArgs& Args);

	TSharedRef<SWidget> CreateBranchesListWidget();
	TSharedRef<SWidget> CreateSelectedBranchPropertiesWidget();
	TSharedRef<SWidget> CreateSelectedBranchPhrasesWidget();
	TSharedRef<FTabManager::FLayout> GetDefaultTabContents();

	void AddNewBranch();
	void PlayDialogue();
	void ShowConfig();

	

	TSharedPtr<SDockTab> BranchesListTab;
	TSharedPtr<SDockTab> SelectedBranchPropertiesTab;
	TSharedPtr<SDockTab> SelectedBranchPhrasesTab;
	
	USmartDialogue* Dialogue;
	
	TSharedPtr<STreeView<TSharedPtr<FSmartDialogueBranch>>> DialogueBranchesList;
	TSharedPtr<IDetailsView> DialogueBranchDetailsView;
	TSharedPtr<IDetailsView> DialoguePhrasesDetailsView;
};
