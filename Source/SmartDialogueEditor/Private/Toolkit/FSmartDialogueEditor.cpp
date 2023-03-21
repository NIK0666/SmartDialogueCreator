// 


#include "FSmartDialogueEditor.h"

#include "SmartDialogue.h"

#define LOCTEXT_NAMESPACE "SmartDialogueEditor"

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

#undef LOCTEXT_NAMESPACE