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
	void OnAssetDoubleClicked(const FAssetData& AssetData);

private:
	FSmartDialogueEditor* SmartDialogueEditorPtr = nullptr;
};
