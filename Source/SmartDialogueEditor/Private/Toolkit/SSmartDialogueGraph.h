// 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

class FSmartDialogueEditor;
class USmartDialogueGraph;
class SGraphEditor;

class SMARTDIALOGUEEDITOR_API SSmartDialogueGraph : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSmartDialogueGraph):
	_InDialogueGraph(nullptr),
	_SmartDialogueEditor(nullptr)
	{} 	
	SLATE_ARGUMENT(USmartDialogueGraph*, InDialogueGraph)
	SLATE_ARGUMENT(FSmartDialogueEditor*, SmartDialogueEditor)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);

private:

	void HandleSelectedNodesChanged(const TSet<UObject*>& NewSelection);
	
	
	TSharedPtr<SGraphEditor> GraphEditorWidget;
	USmartDialogueGraph* DialogueGraph;
	FSmartDialogueEditor* SmartDialogueEditor;

};