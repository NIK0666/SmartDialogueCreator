// 

#pragma once

#include "Input/DragAndDrop.h"

class SPhraseListRow;
class SBranchPhrasesWidget;

class FPhraseDragDropOperation : public FDragDropOperation
{
public:
	DRAG_DROP_OPERATOR_TYPE(FBranchDragDropOperation, FDragDropOperation)
	TSharedPtr<SPhraseListRow> SourcePhraseWidget;

	static TSharedRef<FPhraseDragDropOperation> New(TSharedPtr<SPhraseListRow> InSourcePhraseWidget, TSharedPtr<SBranchPhrasesWidget> InPhrasesWidget);
	
	virtual void OnDragged(const FDragDropEvent& DragDropEvent) override;
	virtual void OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent) override;
	
private:
	TSharedPtr<SBranchPhrasesWidget> PhrasesWidget = nullptr;

};