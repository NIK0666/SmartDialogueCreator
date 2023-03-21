#include "SmartDialogueEditorWindow.h"
#include "Widgets/Text/STextBlock.h"

void SmartDialogueEditorWindow::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SpawnTab(FSpawnTabArgs(TSharedPtr<SWindow>(), FTabId()))
	];
}

TSharedRef<SDockTab> SmartDialogueEditorWindow::SpawnTab(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		.Content()
		[
			SNew(STextBlock)
				.Text(NSLOCTEXT("SmartDialogue", "DialogueEditorEmpty", "Smart Dialogue Editor"))
		];
}