// 


#include "FSmartDialogueEditor.h"

#include "SmartDialogue.h"

#define LOCTEXT_NAMESPACE "SmartDialogueEditor"


void FSmartDialogueEditor::InitSmartDialogueEditor(EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, USmartDialogue* SmartDialogue)
{

	EditingObject = SmartDialogue;
	
	TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_SmartDialogueEditor_Layout")
	->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
		);

	InitAssetEditor(Mode, InitToolkitHost, GetToolkitFName(), StandaloneDefaultLayout, /*bCreateDefaultStandaloneMenu=*/ true, /*bCreateDefaultToolbar=*/ true, SmartDialogue);

}

FName FSmartDialogueEditor::GetToolkitFName() const
{
	return FName("SmartDialogueEditor");
}

FText FSmartDialogueEditor::GetToolkitName() const
{
	const bool bDirtyState = EditingObject->GetOutermost()->IsDirty();

	FFormatNamedArguments Args;
	Args.Add(TEXT("AssetName"), FText::FromString(EditingObject->GetName()));
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
    check(Args.GetTabId().TabType == DialogueBranchesTabId);

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
    check(Args.GetTabId().TabType == DialogueBranchDetailsTabId);

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
    check(Args.GetTabId().TabType == DialoguePhrasesDetailsTabId);

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

#undef LOCTEXT_NAMESPACE
