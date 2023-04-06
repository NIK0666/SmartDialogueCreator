// SSmartDialogueGraph.h

#include "SSmartDialogueGraph.h"

#include "FSmartDialogueEditor.h"
#include "Graph/SmartDialogueGraph.h"

void SSmartDialogueGraph::Construct(const FArguments& InArgs)
{
	SmartDialogueEditor = InArgs._SmartDialogueEditor;
	DialogueGraph = InArgs._InDialogueGraph;

	SGraphEditor::FGraphEditorEvents InEvents = {};
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &SSmartDialogueGraph::HandleSelectedNodesChanged);
	// InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FEnvironmentQueryEditor::OnSelectedNodesChanged);
	
	if (InArgs._InDialogueGraph)
	{
		GraphEditorWidget = SNew(SGraphEditor)
			.GraphToEdit(InArgs._InDialogueGraph)
			.AssetEditorToolkit(SmartDialogueEditor->AsWeak())
			.GraphEvents(InEvents)
			.IsEditable(true);

		if (GraphEditorWidget.IsValid())
		{
			ChildSlot
			[
				GraphEditorWidget.ToSharedRef()
			];
		}
	}
}

void SSmartDialogueGraph::HandleSelectedNodesChanged(const TSet<UObject*>& NewSelection)
{
	if (DialogueGraph)
	{
		DialogueGraph->HandleSelectedNodesChanged(NewSelection);
	}
}
