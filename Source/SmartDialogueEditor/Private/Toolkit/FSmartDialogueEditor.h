// 

#pragma once

#include "CoreMinimal.h"
#include "SmartDialConfig.h"
#include "SmartDialogueData.h"
#include "Graph/BranchNode.h"
#include "Lists/Rows/SPhraseListRow.h"
#include "Toolkits/AssetEditorToolkit.h"

class SCharacterComboBox;
class SBranchComboBox;
class SBranchInfoWidget;
class SBranchesWidget;
class SBranchPhrasesWidget;
class SDialConfigWidget;
class USmartDialogueGraph;

class USmartDialogue;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnBranchItemAdded, FSmartDialogueBranch&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBranchItemRemoved, FName&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBranchSelected, FSmartDialogueBranch&);
DECLARE_MULTICAST_DELEGATE(FOnResetSelectedBranch);


class SMARTDIALOGUEEDITOR_API FSmartDialogueEditor final : public FAssetEditorToolkit, public FNotifyHook, public FEditorUndoClient
{
public:
	void InitSmartDialogueEditor(EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost >& InitToolkitHost, USmartDialogue* SmartDialogue);
	~FSmartDialogueEditor();
	void SetDialogue(USmartDialogue* InDialogue);
	void OpenNewAssetIntoEditor(USmartDialogue* InDialogue);
	USmartDialogue* GetDialogue();
	void BindCommands();
	TSharedPtr<FExtender> GetToolbarExtender();

	FOnBranchItemAdded OnBranchItemAdded;
	FOnBranchItemRemoved OnBranchItemRemoved;
	FOnBranchSelected OnBranchSelected;
	FOnResetSelectedBranch OnResetSelectedBranch;
	
	USmartDialConfig* GetDialogueConfig();
	TArray<FCharacterData> GetAllCharacters();
	TArray<FVariableData> GetAllVariables();
	TArray<FName> GetBranchIDs();

	
	void RemoveBranch(SBranchInfoWidget* BranchInfoWidget);
	void RemovePhrase(int32 PhraseIndex);
	
	TArray<TSharedPtr<FString>> GetAllBranchesList();
	TArray<TSharedPtr<FString>> GetAllVariablesList();
	TArray<TSharedPtr<FString>> GetAllCharactersList();
	TArray<TSharedPtr<FString>> GetOperations(bool bIsExecution);


	virtual FName GetToolkitFName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	void SetSelectedBranchName(FName NewValue);
	FName GetSelectedBranchName();
	FSmartDialogueBranch* GetSelectedBranch();
	FSmartDialogueBranch* GetBranch(const FName& Name);
	
	void ResetSelectedBranch();
	void AddNewBranch();

	TSharedPtr<SBranchesWidget> GetBranchesListPanel() { return BranchesWidget; }
	TSharedPtr<SBranchPhrasesWidget> GetPhrasesListPanel() { return PhrasesWidget; }


	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;

	virtual void PostRedo(bool bSuccess) override;
	virtual void PostUndo(bool bSuccess) override;

private:
	
	TSharedRef<SDockTab> HandleTabManagerSpawnTabDialogueBranches(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> HandleTabManagerSpawnTabDialogueBranchDetails(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> HandleTabManagerSpawnTabDialoguePhrasesDetails(const FSpawnTabArgs& Args);

	TSharedRef<SDockTab> SpawnTab_BranchesList(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_SelectedBranchProperties(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_SelectedBranchPhrases(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Config(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Player(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Graph(const FSpawnTabArgs& SpawnTabArgs);
	TSharedRef<SDockTab> SpawnTab_AssetBrowser(const FSpawnTabArgs& Args);

	TSharedRef<SWidget> CreateSelectedBranchPropertiesWidget();
	TSharedRef<FTabManager::FLayout> GetDefaultTabContents();

	void OnHeroCharacterSelected(TSharedPtr<FString> NewSelection);

	void ShowBranches();
	void PlayDialogue();
	void ShowConfig();

	void RefreshEditor();
	
	void ImportJson();
	void OnCopyNodesInfo();
	void SortNodes();

private:
	TSharedPtr<SDockTab> BranchesListTab;
	TSharedPtr<SDockTab> SelectedBranchPropertiesTab;
	TSharedPtr<SDockTab> SelectedBranchPhrasesTab;

	
	USmartDialogue* Dialogue;
	USmartDialConfig* DialConfig;
	FSmartDialogueBranch* SelectedBranchPtr;
	FName SelectedBranchName;

	
	
	TSharedPtr<STreeView<TSharedPtr<FSmartDialogueBranch>>> DialogueBranchesList;
	TSharedPtr<IDetailsView> DialogueBranchDetailsView;
	TSharedPtr<IDetailsView> DialoguePhrasesDetailsView;

	TSharedPtr<SCharacterComboBox> CharacterComboBox;
	TSharedPtr<SBranchComboBox> StartBranchComboBox;
	TSharedPtr<SBranchesWidget> BranchesWidget;
	TSharedPtr<SBranchPhrasesWidget> PhrasesWidget;
	TSharedPtr<SDialConfigWidget> DialConfigWidget;
	
	TArray<TSharedPtr<FString>> CachedVariablesList;
	TArray<TSharedPtr<FString>> CachedCharactersList;
	TArray<TSharedPtr<FString>> CachedBranchesList;

	UPROPERTY()
	USmartDialogueGraph* DialogueGraph = nullptr;

};