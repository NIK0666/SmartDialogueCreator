// SBranchesListWidget.h

#include "SVerticalListWidget.h"

class SBranchesListWidget : public SVerticalListWidget
{
	
public:
	SLATE_BEGIN_ARGS(SBranchesListWidget)
	: _Title(FText::GetEmpty())
	{}
	SLATE_ARGUMENT(FText, Title)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);

protected:
	TSharedRef<SWidget> GetItemContent(const FString& Item) override;
	void ShowSelectionMenu();
	FReply OnChangeButtonClicked();
};
