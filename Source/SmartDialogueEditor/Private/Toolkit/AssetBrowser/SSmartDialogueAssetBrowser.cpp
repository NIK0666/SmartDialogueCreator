// 


#include "SSmartDialogueAssetBrowser.h"

#include "SmartDialogue.h"
#include "AssetData.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Widgets/Input/SSearchBox.h"
#include "EditorStyleSet.h"
#include "Widgets/Views/SListView.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

void SSmartDialogueAssetBrowser::Construct(const FArguments& InArgs)
{
	SmartDialogueEditorPtr = InArgs._SmartDialogueEditor;
	FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

	FAssetPickerConfig Config;
	Config.InitialAssetViewType = EAssetViewType::Column;
	Config.bAddFilterUI = true;
	Config.bShowPathInColumnView = true;
	Config.bSortByPathInColumnView = true;
	Config.bCanShowClasses = false;
	Config.Filter.ClassPaths.Add(USmartDialogue::StaticClass()->GetClassPathName());
	Config.Filter.ClassPaths.Add(USmartDialConfig::StaticClass()->GetClassPathName());
	Config.OnAssetDoubleClicked = FOnAssetDoubleClicked::CreateSP(this, &SSmartDialogueAssetBrowser::OnAssetDoubleClicked);

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.f)
		[
			SNew(SBorder)
			.Padding(FMargin(3))
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
			[
				ContentBrowserModule.Get().CreateAssetPicker(Config)
			]
		]
	];
}

void SSmartDialogueAssetBrowser::OnAssetDoubleClicked(const FAssetData& AssetData)
{
	if (AssetData.AssetClassPath == USmartDialogue::StaticClass()->GetClassPathName())
	{
		UObject* Asset = AssetData.GetAsset();
		if (USmartDialogue* SmartDialogueAsset = Cast<USmartDialogue>(Asset))
		{
			SmartDialogueEditorPtr->OpenNewAssetIntoEditor(SmartDialogueAsset);
		}
	}
}