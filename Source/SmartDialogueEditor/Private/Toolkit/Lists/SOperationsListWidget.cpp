// 


#include "SOperationsListWidget.h"

#include "Items/SOperationsListItemWidget.h"

void SOperationsListWidget::Construct(const FArguments& InArgs)
{
	bIsExecution = InArgs._bIsExecution;
	SBaseListWidget::Construct(SBaseListWidget::FArguments()
		.Title(InArgs._Title)
		.Editor(InArgs._Editor));
}

TSharedRef<SWidget> SOperationsListWidget::GetItemContent(const FListItemData& Item)
{
	return SNew(SOperationsListItemWidget)
		.Item(Item)
		.Editor(Editor)
		.bIsExecution(bIsExecution);
}