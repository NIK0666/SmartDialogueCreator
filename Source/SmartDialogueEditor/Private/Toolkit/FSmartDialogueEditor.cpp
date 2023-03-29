// 


#include "FSmartDialogueEditor.h"

#include "FSmartDialogueEditorCommands.h"
#include "SBranchPhrasesWidget.h"
#include "Lists/SBranchesWidget.h"
#include "Widgets/Docking/SDockTab.h"
#include "SmartDialogue.h"
#include "SBranchPropertiesWidget.h"
#include "SDialConfigWidget.h"
#include "UMGStyle.h"
#include "SmartDialogueCore/Private/SmartDialogueSettings.h"

#define LOCTEXT_NAMESPACE "SmartDialogueEditor"

static const FName SmartDialogue_BranchesListTabId(TEXT("SmartDialogue_BranchesListTab"));
static const FName SmartDialogue_SelectedBranchPropertiesTabId(TEXT("SmartDialogue_SelectedBranchPropertiesTab"));
static const FName SmartDialogue_SelectedBranchPhrasesTabId(TEXT("SmartDialogue_SelectedBranchPhrasesTab"));
static const FName SmartDialogue_ConfigTabId(TEXT("SmartDialogue_Config"));
static const FName SmartDialogue_PlayerTabId(TEXT("SmartDialogue_Player"));


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
}

USmartDialogue* FSmartDialogueEditor::GetDialogue()
{
	return Dialogue;
}

void FSmartDialogueEditor::BindCommands()
{
	ToolkitCommands->MapAction(
		FSmartDialogueEditorCommands::Get().AddNewBranch,
		FExecuteAction::CreateSP(this, &FSmartDialogueEditor::AddNewBranch),
		FCanExecuteAction());
	
	ToolkitCommands->MapAction(
	FSmartDialogueEditorCommands::Get().ShowBranches,
	FExecuteAction::CreateSP(this, &FSmartDialogueEditor::ShowBranches),
	FCanExecuteAction());
	
	ToolkitCommands->MapAction(
	FSmartDialogueEditorCommands::Get().PlayDialogue,
	FExecuteAction::CreateSP(this, &FSmartDialogueEditor::PlayDialogue),
	FCanExecuteAction());

	ToolkitCommands->MapAction(
		FSmartDialogueEditorCommands::Get().ShowConfig,
		FExecuteAction::CreateSP(this, &FSmartDialogueEditor::ShowConfig),
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
			ToolbarBuilder.AddSeparator();
			ToolbarBuilder.AddToolBarButton(FSmartDialogueEditorCommands::Get().ShowBranches, NAME_None, FText::GetEmpty(), TAttribute<FText>(), FSlateIcon(FUMGStyle::Get().GetStyleSetName(), "ClassIcon.MultiLineEditableText"));
			ToolbarBuilder.AddToolBarButton(FSmartDialogueEditorCommands::Get().PlayDialogue, NAME_None, FText::GetEmpty(), TAttribute<FText>(), FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Toolbar.Play"));
			ToolbarBuilder.AddToolBarButton(FSmartDialogueEditorCommands::Get().ShowConfig, NAME_None, FText::GetEmpty(), TAttribute<FText>(), FSlateIcon(FAppStyle::Get().GetStyleSetName(), "Icons.Settings"));
			ToolbarBuilder.AddSeparator();
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
	return FText::Format(LOCTEXT("CustomAssetTypeEditorAppLabel", "{AssetName}{DirtyState}"), Args);
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

void FSmartDialogueEditor::SetSelectedBranchName(FName NewValue)
{
	SelectedBranchName = NewValue;
	if (GetDialogue())
	{
		if (auto Obj = GetDialogue()->GetBranchPtr(NewValue))
		{
			SelectedBranchPtr = Obj;
			OnBranchSelected.Broadcast(*Obj);
		}
	}	
}

FName FSmartDialogueEditor::GetSelectedBranchName()
{
	return SelectedBranchName;
}

FSmartDialogueBranch* FSmartDialogueEditor::GetSelectedBranch()
{
	return SelectedBranchPtr;
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
		.SetDisplayName(FText::FromString("Branch Properties"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner(SmartDialogue_SelectedBranchPhrasesTabId, FOnSpawnTab::CreateSP(this, &FSmartDialogueEditor::SpawnTab_SelectedBranchPhrases))
		.SetDisplayName(FText::FromString("Phrases"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner(SmartDialogue_ConfigTabId, FOnSpawnTab::CreateSP(this, &FSmartDialogueEditor::SpawnTab_Config))
		.SetDisplayName(FText::FromString("Config"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner(SmartDialogue_PlayerTabId, FOnSpawnTab::CreateSP(this, &FSmartDialogueEditor::SpawnTab_Player))
		.SetDisplayName(FText::FromString("Player"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FSmartDialogueEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(SmartDialogue_BranchesListTabId);
	InTabManager->UnregisterTabSpawner(SmartDialogue_SelectedBranchPropertiesTabId);
	InTabManager->UnregisterTabSpawner(SmartDialogue_SelectedBranchPhrasesTabId);
	InTabManager->UnregisterTabSpawner(SmartDialogue_ConfigTabId);
	InTabManager->UnregisterTabSpawner(SmartDialogue_PlayerTabId);

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
		.Label(FText::FromString("Branch Properties"))
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

TSharedRef<SDockTab> FSmartDialogueEditor::SpawnTab_Config(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == SmartDialogue_ConfigTabId);
	
	return SNew(SDockTab)
		.Label(FText::FromString("Config"))
		[
			SNew(SDialConfigWidget)
			// Создайте виджет для содержимого вкладки Config
		];

}

TSharedRef<SDockTab> FSmartDialogueEditor::SpawnTab_Player(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == SmartDialogue_PlayerTabId);

	return SNew(SDockTab)
		.Label(FText::FromString("Player"))
		[
			SNullWidget::NullWidget
			// Создайте виджет для содержимого вкладки Player
		];

}

TSharedRef<SWidget> FSmartDialogueEditor::CreateBranchesListWidget()
{
	return SNew(SBranchesWidget)
		.SmartDialogueEditor(SharedThis(this));
}

TSharedRef<SWidget> FSmartDialogueEditor::CreateSelectedBranchPropertiesWidget()
{
	return SNew(SBranchPropertiesWidget)
		.SmartDialogueEditor(SharedThis(this));
}

TSharedRef<SWidget> FSmartDialogueEditor::CreateSelectedBranchPhrasesWidget()
{
	return SNew(SBranchPhrasesWidget)
		.SmartDialogueEditor(SharedThis(this));

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
					->AddTab(SmartDialogue_ConfigTabId, ETabState::ClosedTab)
					->AddTab(SmartDialogue_PlayerTabId, ETabState::ClosedTab)
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

void FSmartDialogueEditor::AddNewBranch()
{
	if (GetDialogue())
	{
		FSmartDialogueBranch NewBranch;
		NewBranch.Name = GetDialogue()->GenerateBranchName();
		NewBranch.Text = FText::GetEmpty();
		GetDialogue()->AddNewBranch(NewBranch);

		OnBranchItemAdded.ExecuteIfBound(NewBranch);
	}
}

void FSmartDialogueEditor::ShowConfig()
{
	// Переключение на вкладку Config
	TabManager->TryInvokeTab(SmartDialogue_ConfigTabId);
}

void FSmartDialogueEditor::ShowBranches()
{
	TabManager->TryInvokeTab(SmartDialogue_BranchesListTabId);
}

void FSmartDialogueEditor::PlayDialogue()
{
	// Переключение на вкладку Player
	TabManager->TryInvokeTab(SmartDialogue_PlayerTabId);
}

USmartDialConfig* FSmartDialogueEditor::GetDialogueConfig()
{
	if (DialConfig)
	{
		return DialConfig;
	}
	
	const USmartDialogueSettings* Settings = GetDefault<USmartDialogueSettings>();
	FSoftObjectPath DialoguesConfigAssetPath = Settings->DialoguesConfigAsset;
	if (DialoguesConfigAssetPath.IsValid())
	{
		FStringAssetReference AssetRef(DialoguesConfigAssetPath);
		UObject* LoadedAsset = AssetRef.TryLoad();

		DialConfig =  Cast<USmartDialConfig>(LoadedAsset);
		return DialConfig;
	}
	return nullptr;
}

TArray<FCharacterData> FSmartDialogueEditor::GetAllCharacters()
{
	if (GetDialogueConfig())
	{
		return GetDialogueConfig()->GetCharacters();
	}
	return {};
}

TArray<TSharedPtr<FString>> FSmartDialogueEditor::GetOperations(bool bIsExecution)
{
	TArray<TSharedPtr<FString>> Result;

	const UEnum* EnumPtr = nullptr;
	if (bIsExecution)
	{
		EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESmartDialogueOperation"), true);
	}
	else
	{
		EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESmartDialogueEqualOperation"), true);
	}

	if (EnumPtr)
	{
		for (int32 i = 0; i < EnumPtr->NumEnums() - 1; i++)
		{
			FString DisplayName = EnumPtr->GetDisplayNameTextByIndex(i).ToString();
			Result.Add(MakeShareable(new FString(DisplayName)));
		}
	}

	return Result;
}


TArray<FVariableData> FSmartDialogueEditor::GetAllVariables()
{
	TArray<FVariableData> Result;
	//
	// if (GetDialogue())
	// {
	// 	Result = Dialogue->GetVariables(); //TODO Получить все локальные переменные
	// }
	if (GetDialogueConfig())
	{
		Result.Append(GetDialogueConfig()->GetVariables());
	}
	
	return Result;
}

TArray<FName> FSmartDialogueEditor::GetBranchIDs()
{
	TArray<FName> BranchIDs;
	
	if (GetDialogue())
	{
		for (const auto& Branch : GetDialogue()->GetBranches())
		{
			BranchIDs.Add(Branch.Key);
		}
	}
	return BranchIDs;
}

TArray<TSharedPtr<FString>> FSmartDialogueEditor::GetAllBranchesList(bool bFirstEmpty)
{
	TArray<TSharedPtr<FString>> BranchesList;

	if (bFirstEmpty)
	{
		BranchesList.Add(MakeShared<FString>(""));
	}

	if (GetDialogue())
	{
		for (const auto& Branch : GetDialogue()->GetBranches())
		{
			// Создаем новый объект FString и инициализируем его идентификатором ветки
			TSharedPtr<FString> BranchIDString = MakeShared<FString>(Branch.Key.ToString());

			// Добавляем указатель на объект FString в массив
			BranchesList.Add(BranchIDString);
		}
	}

	return BranchesList;
}

TArray<TSharedPtr<FString>> FSmartDialogueEditor::GetAllVariablesList(bool bFirstEmpty)
{
	TArray<TSharedPtr<FString>> VarsList;
	if (bFirstEmpty)
	{
		VarsList.Add(MakeShared<FString>(""));
	}
	
	TArray<FVariableData> Variables = GetAllVariables();

	for (const FVariableData& Var : Variables)
	{
		VarsList.Add(MakeShared<FString>(Var.Key));
	}

	return VarsList;
}

TArray<TSharedPtr<FString>> FSmartDialogueEditor::GetAllCharactersList(bool bFirstEmpty)
{
	TArray<TSharedPtr<FString>> CharsList;
	if (bFirstEmpty)
	{
		CharsList.Add(MakeShared<FString>(""));
	}
	
	TArray<FCharacterData> Characters = GetAllCharacters();
	for (const FCharacterData& Character : Characters)
	{
		CharsList.Add(MakeShared<FString>(Character.Id));
	}

	return CharsList;
}

#undef LOCTEXT_NAMESPACE
