// 


#include "SSmartDialogueGraph.h"

#include "FSmartDialogueEditor.h"
#include "Graph/SmartDialogueGraph.h"

void SSmartDialogueGraph::Construct(const FArguments& InArgs)
{
	SmartDialogueEditor = InArgs._SmartDialogueEditor;

	SGraphEditor::FGraphEditorEvents InEvents = {};

	
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