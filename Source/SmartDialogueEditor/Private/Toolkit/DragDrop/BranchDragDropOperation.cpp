// BranchDragDropOperation.cpp

#include "BranchDragDropOperation.h"

#include "Toolkit/SBranchesWidget.h"

TSharedRef<FBranchDragDropOperation> FBranchDragDropOperation::New(TSharedPtr<SBranchInfoWidget> InSourceBranchWidget, TSharedPtr<SBranchesWidget> InBranchesWidget)
{
	TSharedRef<FBranchDragDropOperation> Operation = MakeShareable(new FBranchDragDropOperation());
	Operation->SourceBranchWidget = InSourceBranchWidget;
	Operation->BranchesWidget = InBranchesWidget;
	Operation->Construct();
	return Operation;
}

void FBranchDragDropOperation::OnDragged(const FDragDropEvent& DragDropEvent)
{
	if (BranchesWidget)
	{
		BranchesWidget->DragProcessBranchInfo(SourceBranchWidget, DragDropEvent.GetScreenSpacePosition());
	}

	FDragDropOperation::OnDragged(DragDropEvent);
}

void FBranchDragDropOperation::OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent)
{
	if (BranchesWidget)
	{
		FVector2D MousePosition = MouseEvent.GetScreenSpacePosition();
		BranchesWidget->DragEndBranchInfo(SourceBranchWidget, MousePosition);
	}

	FDragDropOperation::OnDrop(bDropWasHandled, MouseEvent);
}
