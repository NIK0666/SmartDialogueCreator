// 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Docking/SDockTab.h"

class SmartDialogueEditorWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SmartDialogueEditorWindow) {}
	SLATE_END_ARGS()

	static TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& Args);

	void Construct(const FArguments& InArgs);
};