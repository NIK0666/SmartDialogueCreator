// BranchDragDropOperation.h

#pragma once

#include "Input/DragAndDrop.h"

class SBranchesWidget;
class SBranchInfoWidget;

class FBranchDragDropOperation : public FDragDropOperation
{
public:
	DRAG_DROP_OPERATOR_TYPE(FBranchDragDropOperation, FDragDropOperation)
	TSharedPtr<SBranchInfoWidget> SourceBranchWidget;

	static TSharedRef<FBranchDragDropOperation> New(TSharedPtr<SBranchInfoWidget> InSourceBranchWidget, TSharedPtr<SBranchesWidget> InBranchesWidget);
	
	virtual void OnDragged(const FDragDropEvent& DragDropEvent) override;
	virtual void OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent) override;
	
private:
	TSharedPtr<SBranchesWidget> BranchesWidget = nullptr;

};