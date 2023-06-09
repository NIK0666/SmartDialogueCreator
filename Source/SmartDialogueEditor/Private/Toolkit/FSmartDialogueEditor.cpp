// 


#include "FSmartDialogueEditor.h"

#include "DesktopPlatformModule.h"
#include "FSmartDialogueEditorCommands.h"
#include "SBranchInfoWidget.h"
#include "SBranchPhrasesWidget.h"
#include "SBranchesWidget.h"
#include "Widgets/Docking/SDockTab.h"
#include "SmartDialogue.h"
#include "SBranchPropertiesWidget.h"
#include "SDialConfigWidget.h"
#include "SSmartDialogueGraph.h"
#include "UMGStyle.h"
#include "Components/SBranchComboBox.h"
#include "Components/SCharacterComboBox.h"
#include "Graph/SmartDialogueGraph.h"
#include "AssetBrowser/SSmartDialogueAssetBrowser.h"
#include "Editor/Transactor.h"
#include "Helpers/EditorDataHelper.h"
#include "PlayPanel/SDialoguePlayerTab.h"
#include "Serializor/JsonParser.h"
#include "SmartDialogueSettings.h"
#include "Windows/WindowsPlatformApplicationMisc.h"

#define LOCTEXT_NAMESPACE "SmartDialogueEditor"

static const FName SmartDialogue_BranchesListTabId(TEXT("SmartDialogue_BranchesListTab"));
static const FName SmartDialogue_SelectedBranchPropertiesTabId(TEXT("SmartDialogue_SelectedBranchPropertiesTab"));
static const FName SmartDialogue_SelectedBranchPhrasesTabId(TEXT("SmartDialogue_SelectedBranchPhrasesTab"));
static const FName SmartDialogue_ConfigTabId(TEXT("SmartDialogue_Config"));
static const FName SmartDialogue_PlayerTabId(TEXT("SmartDialogue_Player"));
static const FName SmartDialogue_GraphTabId(TEXT("SmartDialogue_GraphTab"));
static const FName SmartDialogue_AssetBrowserTabId(TEXT("SmartDialogue_AssetBrowserTab"));



void FSmartDialogueEditor::InitSmartDialogueEditor(EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, USmartDialogue* SmartDialogue)
{
	SetDialogue(SmartDialogue);

	DialogueGraph = NewObject<USmartDialogueGraph>();
	DialogueGraph->SetEditor(this);
	DialogueGraph->LoadNodesFromAsset();
	DialogueGraph->SetFlags(RF_Transactional);
	
	FSmartDialogueEditorCommands::Register();
	ToolkitCommands = MakeShareable(new FUICommandList);

	BindCommands();

	InitAssetEditor(Mode, InitToolkitHost, GetToolkitFName(), GetDefaultTabContents(), /*bCreateDefaultStandaloneMenu=*/ true, /*bCreateDefaultToolbar=*/ true, SmartDialogue);


	TSharedPtr<FExtender> ToolbarExtender = GetToolbarExtender();
	FAssetEditorToolkit::AddToolbarExtender(ToolbarExtender);
	
	
	RegenerateMenusAndToolbars();

	if (GEditor)
	{
		GEditor->RegisterForUndo(this);
	}
}

FSmartDialogueEditor::~FSmartDialogueEditor()
{
	if (GEditor)
	{
		GEditor->UnregisterForUndo(this);
	}
}

void FSmartDialogueEditor::SetDialogue(USmartDialogue* InDialogue)
{
	Dialogue = InDialogue;
}

void FSmartDialogueEditor::OpenNewAssetIntoEditor(USmartDialogue* InDialogue)
{	
	if (InDialogue)
	{
		AddEditingObject(InDialogue);
	}
	
	if (Dialogue)
	{
		RemoveEditingAsset(Dialogue);
	}
	
	ResetSelectedBranch();
	SetDialogue(InDialogue);
	
	GetBranchesListPanel()->UpdateBranchesList();
	DialogueGraph->LoadNodesFromAsset();
	RegenerateMenusAndToolbars();
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

	ToolkitCommands->MapAction(
		FSmartDialogueEditorCommands::Get().ImportJSON,
		FExecuteAction::CreateSP(this, &FSmartDialogueEditor::ImportJson),
		FCanExecuteAction());

	const FSmartDialogueEditorCommands& Commands = FSmartDialogueEditorCommands::Get();
	ToolkitCommands->MapAction(
		Commands.CopyNodesInfo,
		FExecuteAction::CreateSP(this, &FSmartDialogueEditor::OnCopyNodesInfo));

	ToolkitCommands->MapAction(
	FSmartDialogueEditorCommands::Get().SortNodes,
	FExecuteAction::CreateSP(this, &FSmartDialogueEditor::SortNodes),
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
			ToolbarBuilder.AddToolBarButton(FSmartDialogueEditorCommands::Get().ImportJSON, NAME_None, FText::GetEmpty(), TAttribute<FText>(), FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Import"));
			

			ToolbarBuilder.AddToolBarWidget(
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SAssignNew(CharacterComboBox, SCharacterComboBox)
					.SmartDialogueEditor(this)
					.OnItemSelected(this, &FSmartDialogueEditor::OnHeroCharacterSelected)
					.DefaultText(GetDialogue()->GetCharacter())
				]
				+ SHorizontalBox::Slot() // Добавление слота для SBranchComboBox
				.AutoWidth()
				[
					SAssignNew(StartBranchComboBox, SBranchComboBox)
					.SmartDialogueEditor(this)
					.OnItemSelected_Lambda([this](TSharedPtr<FString> NewSelection)
					{
						UEditorDataHelper::SetAutoBranch(this, *NewSelection);
					})
					.DefaultText(GetDialogue()->GetAutoBranch())
				]
			);
			
			ToolbarBuilder.AddSeparator();
			ToolbarBuilder.AddToolBarButton(FSmartDialogueEditorCommands::Get().SortNodes, NAME_None, FText::GetEmpty(), TAttribute<FText>(),FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Adjust"));
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

void FSmartDialogueEditor::ResetSelectedBranch()
{
	SelectedBranchName = FName();
	SelectedBranchPtr = nullptr;

	OnResetSelectedBranch.Broadcast();
}

FName FSmartDialogueEditor::GetSelectedBranchName()
{
	return SelectedBranchName;
}

FSmartDialogueBranch* FSmartDialogueEditor::GetSelectedBranch()
{
	return SelectedBranchPtr;
}

FSmartDialogueBranch* FSmartDialogueEditor::GetBranch(const FName& Name)
{
	if (auto Obj = GetDialogue()->GetBranchPtr(Name))
	{
		return Obj;
	}
	return nullptr;
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

	TabManager->RegisterTabSpawner(SmartDialogue_GraphTabId, FOnSpawnTab::CreateSP(this, &FSmartDialogueEditor::SpawnTab_Graph))
		.SetDisplayName(LOCTEXT("GraphTabTitle", "Graph"))
		.SetTooltipText(LOCTEXT("GraphTabTooltip", "Open the Dialogue Graph tab."));

	TabManager->RegisterTabSpawner(SmartDialogue_AssetBrowserTabId, FOnSpawnTab::CreateSP(this, &FSmartDialogueEditor::SpawnTab_AssetBrowser))
		.SetDisplayName(LOCTEXT("AssetBrowserTabTitle", "Asset Browser"))
		.SetTooltipText(LOCTEXT("AssetBrowserTabTooltip", "Open the Asset Browser tab."));

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
			SAssignNew(BranchesWidget, SBranchesWidget)
			.SmartDialogueEditor(this)
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
			SAssignNew(PhrasesWidget, SBranchPhrasesWidget)
			.SmartDialogueEditor(this)
		];
}

TSharedRef<SDockTab> FSmartDialogueEditor::SpawnTab_Config(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == SmartDialogue_ConfigTabId);
	
	return SNew(SDockTab)
		.Label(FText::FromString("Config"))
		[
			SAssignNew(DialConfigWidget, SDialConfigWidget)
				.SmartDialogueEditor(this)
		];

}

TSharedRef<SDockTab> FSmartDialogueEditor::SpawnTab_Player(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == SmartDialogue_PlayerTabId);
	
	return SNew(SDockTab)
		.Label(FText::FromString("Player"))
		[
			SNew(SDialoguePlayerTab)
				.SmartDialogueEditor(this)
		];

}

TSharedRef<SDockTab> FSmartDialogueEditor::SpawnTab_Graph(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == SmartDialogue_GraphTabId);
	
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SSmartDialogueGraph)
			.InDialogueGraph(DialogueGraph)
			.SmartDialogueEditor(this)
		];
}

TSharedRef<SDockTab> FSmartDialogueEditor::SpawnTab_AssetBrowser(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == SmartDialogue_AssetBrowserTabId);
	
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SSmartDialogueAssetBrowser)
			.SmartDialogueEditor(this)
		];
}


TSharedRef<SWidget> FSmartDialogueEditor::CreateSelectedBranchPropertiesWidget()
{
	return SNew(SBranchPropertiesWidget)
		.SmartDialogueEditor(this);
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
					->AddTab(SmartDialogue_GraphTabId, ETabState::ClosedTab)
					
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
					->AddTab(SmartDialogue_AssetBrowserTabId, ETabState::ClosedTab)
				)
			)
		);
}

void FSmartDialogueEditor::OnHeroCharacterSelected(TSharedPtr<FString> NewSelection)
{
	UEditorDataHelper::SetCharacter(this, *NewSelection.Get());
}

void FSmartDialogueEditor::AddNewBranch()
{
	if (GetDialogue())
	{
		FSmartDialogueBranch NewBranch;
		NewBranch.Name = GetDialogue()->GenerateBranchName();
		NewBranch.Text = FText::GetEmpty();
		UEditorDataHelper::AddNewBranch(this, NewBranch);

		OnBranchItemAdded.Broadcast(NewBranch);
	}
}

void FSmartDialogueEditor::ShowConfig()
{
	// Переключение на вкладку Config
	TabManager->TryInvokeTab(SmartDialogue_ConfigTabId);
}

void FSmartDialogueEditor::RefreshEditor()
{
	SelectedBranchPtr = nullptr;
	GetBranchesListPanel()->UpdateBranchesList();
	CharacterComboBox->SetItemValue(GetDialogue()->GetCharacter());
	if (DialConfigWidget)
	{
		DialConfigWidget->UpdateData();
	}
	
}

void FSmartDialogueEditor::ImportJson()
{
	// Открываем диалог выбора файла
	FString DefaultPath = FPaths::ProjectContentDir();
	FString FileTypes = TEXT("JSON Files (*.json)|*.json");
	TArray<FString> FileNames;

	if (FDesktopPlatformModule::Get()->OpenFileDialog(
			nullptr,
			TEXT("Выберите JSON файл"),
			DefaultPath,
			TEXT(""),
			FileTypes,
			EFileDialogFlags::None,
			FileNames))
		{

		if (FileNames.IsEmpty())
		{
			return;
		}
		
		// Создаем объект парсера
		UJsonParser* JsonParser = NewObject<UJsonParser>();

		// Проверяем, был ли открыт ассет диалога в редакторе
		if (GetDialogue())
		{
			// Запускаем парсинг JSON файла и передаем параметры
			bool bParseSuccessful = JsonParser->ParseJson(FileNames[0], GetDialogue());

			if (bParseSuccessful)
			{
				RefreshEditor();
				
			}
			else
			{
				// Выводим сообщение об ошибке
				FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Ошибка при парсинге JSON файла.")));
			}
		}
		else
		{
			// Выводим сообщение об ошибке
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Ассет диалога не открыт в редакторе.")));
		}
	}
}

void FSmartDialogueEditor::OnCopyNodesInfo()
{
	FString NodesInfo = DialogueGraph->GetNodesInformation();

	if (!NodesInfo.IsEmpty())
	{
		FPlatformApplicationMisc::ClipboardCopy(*NodesInfo);
	}
}

void FSmartDialogueEditor::SortNodes()
{
	if (DialogueGraph)
	{
		DialogueGraph->ArrangeNodes();
	}
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
	if (GetDialogue())
	{
		Result = Dialogue->GetVariables();
	}
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

void FSmartDialogueEditor::RemoveBranch(SBranchInfoWidget* BranchInfoWidget)
{
	if (BranchInfoWidget)
	{
		FName BranchName = BranchInfoWidget->GetBranchName();

		if (BranchName == SelectedBranchName)
		{
			ResetSelectedBranch();
		}
		UEditorDataHelper::RemoveBranch(this, BranchName);
		BranchesWidget->RemoveRow(BranchInfoWidget);
		OnBranchItemRemoved.Broadcast(BranchName);
	}
	
}

void FSmartDialogueEditor::RemovePhrase(int32 PhraseIndex)
{
	SelectedBranchPtr->Phrases.RemoveAt(PhraseIndex);
	PhrasesWidget->RemovePhraseRow(PhraseIndex);
}


TArray<TSharedPtr<FString>> FSmartDialogueEditor::GetAllBranchesList()
{
	if (CachedBranchesList.Num() == 0)
	{
		CachedBranchesList.Add(MakeShared<FString>(""));
	}

	if (GetDialogue())
	{
		TSet<FString> NewBranchesSet;

		for (const auto& Branch : GetDialogue()->GetBranches())
		{
			NewBranchesSet.Add(Branch.Key.ToString());
		}

		// Удаляем старые элементы из CachedBranchesList
		for (int32 i = CachedBranchesList.Num() - 1; i >= 1; i--)
		{
			if (!NewBranchesSet.Contains(*CachedBranchesList[i]))
			{
				CachedBranchesList.RemoveAt(i);
			}
		}

		// Добавляем новые элементы в CachedBranchesList
		for (const FString& NewBranch : NewBranchesSet)
		{
			bool bFound = false;
			for (const TSharedPtr<FString>& CachedBranch : CachedBranchesList)
			{
				if (*CachedBranch == NewBranch)
				{
					bFound = true;
					break;
				}
			}

			if (!bFound)
			{
				CachedBranchesList.Add(MakeShared<FString>(NewBranch));
			}
		}
	}

	return CachedBranchesList;
}

TArray<TSharedPtr<FString>> FSmartDialogueEditor::GetAllVariablesList()
{
	if (CachedVariablesList.Num() == 0)
	{
		CachedVariablesList.Add(MakeShared<FString>(""));
	}
	
	TArray<FVariableData> Variables = GetAllVariables();
	TSet<FString> NewVariablesSet;

	for (const FVariableData& Var : Variables)
	{
		NewVariablesSet.Add(Var.Key);
	}

	// Удаляем старые элементы
	for (int32 i = CachedVariablesList.Num() - 1; i >= 1; i--)
	{
		if (!NewVariablesSet.Contains(*CachedVariablesList[i]))
		{
			CachedVariablesList.RemoveAt(i);
		}
	}

	// Добавляем новые элементы
	for (const FString& NewVariable : NewVariablesSet)
	{
		bool bFound = false;
		for (const TSharedPtr<FString>& CachedVariable : CachedVariablesList)
		{
			if (*CachedVariable == NewVariable)
			{
				bFound = true;
				break;
			}
		}

		if (!bFound)
		{
			CachedVariablesList.Add(MakeShared<FString>(NewVariable));
		}
	}

	return CachedVariablesList;
}

TArray<TSharedPtr<FString>> FSmartDialogueEditor::GetAllCharactersList()
{
	if (CachedCharactersList.Num() == 0)
	{
		CachedCharactersList.Add(MakeShared<FString>(""));
	}
	
	TArray<FCharacterData> Characters = GetAllCharacters();
	TSet<FString> NewCharactersSet;

	for (const FCharacterData& Character : Characters)
	{
		NewCharactersSet.Add(Character.Id);
	}

	// Удаляем старые элементы
	for (int32 i = CachedCharactersList.Num() - 1; i >= 1; i--)
	{
		if (!NewCharactersSet.Contains(*CachedCharactersList[i]))
		{
			CachedCharactersList.RemoveAt(i);
		}
	}

	// Добавляем новые элементы
	for (const FString& NewCharacter : NewCharactersSet)
	{
		bool bFound = false;
		for (const TSharedPtr<FString>& CachedCharacter : CachedCharactersList)
		{
			if (*CachedCharacter == NewCharacter)
			{
				bFound = true;
				break;
			}
		}

		if (!bFound)
		{
			CachedCharactersList.Add(MakeShared<FString>(NewCharacter));
		}
	}

	return CachedCharactersList;
}

void FSmartDialogueEditor::PostRedo(bool bSuccess)
{
	const FTransaction* Transaction = GEditor->Trans->GetTransaction(GEditor->Trans->GetQueueLength() - GEditor->Trans->GetUndoCount());
	if (DialConfigWidget && Transaction->ContainsObject(GetDialogueConfig()))
	{
		DialConfigWidget->UpdateData();
	}
	FEditorUndoClient::PostRedo(bSuccess);
}

void FSmartDialogueEditor::PostUndo(bool bSuccess)
{
	const FTransaction* Transaction = GEditor->Trans->GetTransaction(GEditor->Trans->GetQueueLength() - GEditor->Trans->GetUndoCount());
	if (DialConfigWidget && Transaction->ContainsObject(GetDialogueConfig()))
	{
		DialConfigWidget->UpdateData(true, true, false, true);
	}
	if (Transaction->ContainsObject(GetDialogue()))
	{
		SetSelectedBranchName(GetSelectedBranchName());
		
		if (BranchesWidget)
		{
			BranchesWidget->UpdateBranchesList();
		}
		if (DialogueGraph)
		{
			DialogueGraph->LoadNodesFromAsset();
		}
		DialConfigWidget->UpdateData(false, false, true, false);
	}
	FEditorUndoClient::PostUndo(bSuccess);
}



#undef LOCTEXT_NAMESPACE
