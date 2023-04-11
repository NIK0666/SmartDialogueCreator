// 


#include "PhraseDragDropOperation.h"

#include "Toolkit/SBranchPhrasesWidget.h"

TSharedRef<FPhraseDragDropOperation> FPhraseDragDropOperation::New(TSharedPtr<SPhraseListRow> InSourcePhraseWidget, TSharedPtr<SBranchPhrasesWidget> InPhrasesWidget)
{
	TSharedRef<FPhraseDragDropOperation> Operation = MakeShareable(new FPhraseDragDropOperation());
	Operation->SourcePhraseWidget = InSourcePhraseWidget;
	Operation->PhrasesWidget = InPhrasesWidget;
	Operation->Construct();
	return Operation;
}

void FPhraseDragDropOperation::OnDragged(const FDragDropEvent& DragDropEvent)
{
	if (PhrasesWidget)
	{
		PhrasesWidget->DragProcess(SourcePhraseWidget, DragDropEvent.GetScreenSpacePosition());
	}
	FDragDropOperation::OnDragged(DragDropEvent);
}

void FPhraseDragDropOperation::OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent)
{
	if (PhrasesWidget)
	{
		PhrasesWidget->DragEnd(SourcePhraseWidget, MouseEvent.GetScreenSpacePosition());
	}
	FDragDropOperation::OnDrop(bDropWasHandled, MouseEvent);
}
