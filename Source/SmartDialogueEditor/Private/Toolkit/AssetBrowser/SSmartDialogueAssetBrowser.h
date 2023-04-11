// 

#pragma once

#include "CoreMinimal.h"
#include "Toolkit/FSmartDialogueEditor.h"

class SSmartDialogueAssetBrowser : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSmartDialogueAssetBrowser) {}
	SLATE_ARGUMENT(FSmartDialogueEditor*, SmartDialogueEditor)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	FSmartDialogueEditor* SmartDialogueEditorPtr = nullptr;
	TArray<TSharedPtr<FAssetData>> AssetList;
	TSharedPtr<SListView<TSharedPtr<FAssetData>>> AssetListView;
	TSharedPtr<STextBlock> AssetCountTextBlock;

	void UpdateAssetList();
	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FAssetData> Item, const TSharedRef<STableViewBase>& OwnerTable);
	void OnAssetSelected(TSharedPtr<FAssetData> AssetData, ESelectInfo::Type SelectInfo);
};

