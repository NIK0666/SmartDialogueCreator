// 


#include "SSmartDialogueAssetBrowser.h"

#include "SmartDialogue.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Widgets/Input/SSearchBox.h"

void SSmartDialogueAssetBrowser::Construct(const FArguments& InArgs)
{
	SmartDialogueEditorPtr = InArgs._SmartDialogueEditor;
	UpdateAssetList();

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.Text(FText::FromString("<"))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.Text(FText::FromString(">"))
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5.0f)
		[
			SNew(SSearchBox)
			.HintText(FText::FromString("Search Assets"))
		]
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.Padding(5.0f)
		[
			SAssignNew(AssetListView, SListView<TSharedPtr<FAssetData>>)
			.ItemHeight(24)
			.ListItemsSource(&AssetList)
			.OnGenerateRow(this, &SSmartDialogueAssetBrowser::OnGenerateRowForList)
			.SelectionMode(ESelectionMode::Single)
			.OnSelectionChanged(this, &SSmartDialogueAssetBrowser::OnAssetSelected)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5.0f)
		.HAlign(HAlign_Left)
		[
			SAssignNew(AssetCountTextBlock, STextBlock)
			.Text(FText::Format(FText::FromString("{0} items"), FText::AsNumber(AssetList.Num())))
		]
	];
}

void SSmartDialogueAssetBrowser::UpdateAssetList()
{
	AssetList.Empty();

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetDataArray;
	AssetRegistryModule.Get().GetAssetsByClass(USmartDialogue::StaticClass()->GetFName(), AssetDataArray);

	for (const FAssetData& AssetData : AssetDataArray)
	{
		AssetList.Add(MakeShared<FAssetData>(AssetData));
	}

	if (AssetCountTextBlock.IsValid())
	{
		AssetCountTextBlock->SetText(FText::Format(FText::FromString("{0} items"), FText::AsNumber(AssetList.Num())));
	}
}

TSharedRef<ITableRow> SSmartDialogueAssetBrowser::OnGenerateRowForList(TSharedPtr<FAssetData> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromString(Item->AssetName.ToString()))
			]
			+ SHorizontalBox::Slot()
			.FillWidth(2.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromString(Item->PackagePath.ToString()))
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(STextBlock)
				.Text(FText::Format(FText::FromString("{0} Kb"), FText::AsNumber(FMath::CeilToInt(Item->GetPackage()->GetFileSize() / 1024.0f))))
			]
		];
}

void SSmartDialogueAssetBrowser::OnAssetSelected(TSharedPtr<FAssetData> AssetData, ESelectInfo::Type SelectInfo)
{
}
