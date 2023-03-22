// 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Toolkit/FSmartDialogueEditor.h"

class SMARTDIALOGUEEDITOR_API SBranchesListWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBranchesListWidget) {}
	SLATE_ARGUMENT(TSharedPtr<FSmartDialogueEditor>, SmartDialogueEditor)
	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);
	void UpdateBranchesList();

	void BranchItemAdded(FSmartDialogueBranch& AddedBranch);
	void BranchItemDeleted(FSmartDialogueBranch& DeletedBranch);

private:
	TSharedPtr<FSmartDialogueEditor> SmartDialogueEditor;
};