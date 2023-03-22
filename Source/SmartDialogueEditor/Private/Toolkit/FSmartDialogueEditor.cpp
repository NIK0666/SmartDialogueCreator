// 


#include "FSmartDialogueEditor.h"

#include "FSmartDialogueEditorCommands.h"
#include "Widgets/Docking/SDockTab.h"
#include "SmartDialogue.h"
#include "SBranchInfoWidget.h"

#define LOCTEXT_NAMESPACE "SmartDialogueEditor"

static const FName SmartDialogue_BranchesListTabId(TEXT("SmartDialogue_BranchesListTab"));
static const FName SmartDialogue_SelectedBranchPropertiesTabId(TEXT("SmartDialogue_SelectedBranchPropertiesTab"));
static const FName SmartDialogue_SelectedBranchPhrasesTabId(TEXT("SmartDialogue_SelectedBranchPhrasesTab"));


void FSmartDialogueEditor::InitSmartDialogueEditor(EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, USmartDialogue* SmartDialogue)
{
	SetDialogue(SmartDialogue);

	FSmartDialogueEditorCommands::Register();
	ToolkitCommands = MakeShareable(new FUICommandList);

	BindCommands();

	InitAssetEditor(Mode, InitToolkitHost, GetToolkitFName(), GetDefaultTabContents(), /*bCreateDefaultStandaloneMenu=*/ true, /*bCreateDefaultToolbar=*/ true, SmartDialogue);


	TSharedPtr<FExtender> ToolbarExtender = GetToolbarExtender();
	FAssetEditorToolkit::AddToolbarExtender(ToolbarExtender);

	RegenerateMenusAndToolbars();
}

void FSmartDialogueEditor::SetDialogue(USmartDialogue* InDialogue)
{
	Dialogue = InDialogue;

	if (Dialogue)
	{
		DialogueBranchWidgets.Reset();

		for (FSmartDialogueBranch& Branch : Dialogue->Branches)
		{
			DialogueBranchWidgets.Add(SNew(SBranchInfoWidget)
				.Branch(Branch)
				.Editor(SharedThis(this)));
		}
	}
}

void FSmartDialogueEditor::BindCommands()
{
	ToolkitCommands->MapAction(
		FSmartDialogueEditorCommands::Get().AddNewBranch,
		FExecuteAction::CreateSP(this, &FSmartDialogueEditor::AddNewBranch),
		FCanExecuteAction());
}

TSharedPtr<FExtender> FSmartDialogueEditor::GetToolbarExtender()
{
	TSharedPtr<FExtender> Extender = MakeShared<FExtender>();
	Extender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		ToolkitCommands,
		FToolBarExtensionDelegate::CreateLambda([this](FToolBarBuilder& ToolbarBuilder)
		{
			ToolbarBuilder.AddToolBarButton(FSmartDialogueEditorCommands::Get().AddNewBranch, NAME_None, FText::GetEmpty(), TAttribute<FText>(), FSlateIcon(FEditorStyle::GetStyleSetName(), "Icons.Plus"));
		})
	);

	return Extender;
}

FName FSmartDialogueEditor::GetToolkitFName() const
{
	return FName("SmartDialogueEditor");
}

FText FSmartDialogueEditor::GetToolkitName() const
{
	const bool bDirtyState = Dialogue->GetOutermost()->IsDirty();

	FFormatNamedArguments Args;
	Args.Add(TEXT("AssetName"), FText::FromString(Dialogue->GetName()));
	Args.Add(TEXT("DirtyState"), bDirtyState ? FText::FromString( TEXT( "*" ) ) : FText::GetEmpty());
	return FText::Format(LOCTEXT("CustomAssetTypeEditorAppLabel", "{SpriteName}{DirtyState}"), Args);
}

FText FSmartDialogueEditor::GetBaseToolkitName() const
{
	return LOCTEXT("SmartDialogueEditorAppLabel", "Smart Dialogue Editor");
}

FString FSmartDialogueEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("SmartDialogueEditor");
}

FLinearColor FSmartDialogueEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

TSharedRef<SDockTab> FSmartDialogueEditor::HandleTabManagerSpawnTabDialogueBranches(const FSpawnTabArgs& Args)
{
    check(Args.GetTabId().TabType == SmartDialogue_BranchesListTabId);

    // TODO: Create the DialogueBranchesList widget here
    return SNew(SDockTab)
        .TabRole(ETabRole::PanelTab)
        [
            SNew(SVerticalBox)
            // Add your DialogueBranchesList widget here
        ];
}

TSharedRef<SDockTab> FSmartDialogueEditor::HandleTabManagerSpawnTabDialogueBranchDetails(const FSpawnTabArgs& Args)
{
    check(Args.GetTabId().TabType == SmartDialogue_SelectedBranchPropertiesTabId);

    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.bAllowSearch = true;
    DetailsViewArgs.bHideSelectionTip = true;
    DetailsViewArgs.bLockable = false;
    DetailsViewArgs.bShowOptions = true;
    DetailsViewArgs.bShowModifiedPropertiesOption = false;
    DetailsViewArgs.bUpdatesFromSelection = false;
    DetailsViewArgs.NotifyHook = this;

    DialogueBranchDetailsView = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor").CreateDetailView(DetailsViewArgs);

    return SNew(SDockTab)
        .TabRole(ETabRole::PanelTab)
        [
            DialogueBranchDetailsView.ToSharedRef()
        ];
}

TSharedRef<SDockTab> FSmartDialogueEditor::HandleTabManagerSpawnTabDialoguePhrasesDetails(const FSpawnTabArgs& Args)
{
    check(Args.GetTabId().TabType == SmartDialogue_SelectedBranchPhrasesTabId);

    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.bAllowSearch = true;
    DetailsViewArgs.bHideSelectionTip = true;
    DetailsViewArgs.bLockable = false;
    DetailsViewArgs.bShowOptions = true;
    DetailsViewArgs.bShowModifiedPropertiesOption = false;
    DetailsViewArgs.bUpdatesFromSelection = false;
    DetailsViewArgs.NotifyHook = this;

    DialoguePhrasesDetailsView = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor").CreateDetailView(DetailsViewArgs);

    return SNew(SDockTab)
        .TabRole(ETabRole::PanelTab)
        [
            DialoguePhrasesDetailsView.ToSharedRef()
        ];
}

void FSmartDialogueEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = TabManager->AddLocalWorkspaceMenuCategory(FText::FromString("Smart Dialogue Editor"));

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(SmartDialogue_BranchesListTabId, FOnSpawnTab::CreateSP(this, &FSmartDialogueEditor::SpawnTab_BranchesList))
		.SetDisplayName(FText::FromString("Branches List"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner(SmartDialogue_SelectedBranchPropertiesTabId, FOnSpawnTab::CreateSP(this, &FSmartDialogueEditor::SpawnTab_SelectedBranchProperties))
		.SetDisplayName(FText::FromString("Properties"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner(SmartDialogue_SelectedBranchPhrasesTabId, FOnSpawnTab::CreateSP(this, &FSmartDialogueEditor::SpawnTab_SelectedBranchPhrases))
		.SetDisplayName(FText::FromString("Phrases"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FSmartDialogueEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(SmartDialogue_BranchesListTabId);
	InTabManager->UnregisterTabSpawner(SmartDialogue_SelectedBranchPropertiesTabId);
	InTabManager->UnregisterTabSpawner(SmartDialogue_SelectedBranchPhrasesTabId);
}

TSharedRef<SDockTab> FSmartDialogueEditor::SpawnTab_BranchesList(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == SmartDialogue_BranchesListTabId);

	return SNew(SDockTab)
		.Label(FText::FromString("Branches List"))
		[
			CreateBranchesListWidget()
		];
}

TSharedRef<SDockTab> FSmartDialogueEditor::SpawnTab_SelectedBranchProperties(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == SmartDialogue_SelectedBranchPropertiesTabId);

	return SNew(SDockTab) 
		.Label(FText::FromString("Properties"))
		[
			CreateSelectedBranchPropertiesWidget()
		];
}

TSharedRef<SDockTab> FSmartDialogueEditor::SpawnTab_SelectedBranchPhrases(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == SmartDialogue_SelectedBranchPhrasesTabId);

	return SNew(SDockTab)
		.Label(FText::FromString("Phrases"))
		[
			CreateSelectedBranchPhrasesWidget()
		];
}

TSharedRef<SWidget> FSmartDialogueEditor::CreateBranchesListWidget()
{
	return SNew(SListView<TSharedRef<SBranchInfoWidget>>)
		.SelectionMode(ESelectionMode::None)
		.ListItemsSource(&DialogueBranchWidgets)
		.OnGenerateRow(this, &FSmartDialogueEditor::OnGenerateRowForBranchList);
}

TSharedRef<SWidget> FSmartDialogueEditor::CreateSelectedBranchPropertiesWidget()
{
	// Создайте виджет на основе SDetailsView или другого подходящего виджета для представления свойств выбранной ветки.
	// Здесь вы также можете настроить отображение и редактирование свойств.
	// ...
	return SNullWidget::NullWidget;

}

TSharedRef<SWidget> FSmartDialogueEditor::CreateSelectedBranchPhrasesWidget()
{
	// Создайте виджет на основе SListView или другого подходящего виджета для представления фраз выбранной ветки диалога.
	// Здесь вы также можете настроить отображение элементов списка и обработчики событий.
	// ...
	return SNullWidget::NullWidget;

}

TSharedRef<FTabManager::FLayout> FSmartDialogueEditor::GetDefaultTabContents()
{
	return FTabManager::NewLayout("SmartDialogueEditor_DefaultLayout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->SetSizeCoefficient(0.5f)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(SmartDialogue_BranchesListTabId, ETabState::OpenedTab)
				)
			)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Vertical)
				->SetSizeCoefficient(0.5f)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(SmartDialogue_SelectedBranchPropertiesTabId, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(SmartDialogue_SelectedBranchPhrasesTabId, ETabState::OpenedTab)
				)
			)
		);

	// GetTabManager()->RestoreFrom(DefaultLayout, TSharedPtr<SWindow>());
}

TSharedRef<ITableRow> FSmartDialogueEditor::OnGenerateRowForBranchList(TSharedRef<SBranchInfoWidget> InWidget, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedRef<SBranchInfoWidget>>, OwnerTable)
		.Content()
		[
			InWidget
		];
}

void FSmartDialogueEditor::AddNewBranch()
{
	if (Dialogue)
	{
		FSmartDialogueBranch NewBranch;
		NewBranch.Name = "New Branch";
		NewBranch.Text = "New Branch Text";
		Dialogue->Branches.Add(NewBranch);

		TSharedRef<SBranchInfoWidget> NewBranchWidget = SNew(SBranchInfoWidget)
			.Branch(NewBranch)
			.Editor(SharedThis(this));
		DialogueBranchWidgets.Add(NewBranchWidget);

		OnBranchListModified.ExecuteIfBound();
	}
}

#undef LOCTEXT_NAMESPACE
