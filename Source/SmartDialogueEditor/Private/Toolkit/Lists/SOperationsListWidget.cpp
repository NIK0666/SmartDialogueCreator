// 


#include "SOperationsListWidget.h"

#include "SOperationsListItemWidget.h"

void SOperationsListWidget::Construct(const FArguments& InArgs)
{
	SBaseListWidget::Construct(SBaseListWidget::FArguments()
		.Title(InArgs._Title)
		.Editor(InArgs._Editor));
	bIsExecution = InArgs._bIsExecution;
}

TSharedRef<SWidget> SOperationsListWidget::GetItemContent(const FListItemData& Item)
{
	return SNew(SOperationsListItemWidget)
		.Item(Item)
		.Editor(Editor)
		.bIsExecution(bIsExecution);
}