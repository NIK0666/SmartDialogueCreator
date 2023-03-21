// AssetTypeActions_SmartDialogue.cpp
#include "AssetTypeActions_SmartDialogue.h"
#include "SmartDialogue.h"
#include "Toolkit/FSmartDialogueEditor.h"

#define LOCTEXT_NAMESPACE "SmartDialogueEditor"

FAssetTypeActions_SmartDialogue::FAssetTypeActions_SmartDialogue(EAssetTypeCategories::Type InCategory): DialogueCategoryBit(InCategory)
{
}

FText FAssetTypeActions_SmartDialogue::GetName() const
{
    return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_SmartDialogue", "Smart Dialogue");
}

UClass* FAssetTypeActions_SmartDialogue::GetSupportedClass() const
{
	return USmartDialogue::StaticClass();
}

FColor FAssetTypeActions_SmartDialogue::GetTypeColor() const
{
	return FColor::Purple;
}

uint32 FAssetTypeActions_SmartDialogue::GetCategories()
{
	return DialogueCategoryBit;
}

void FAssetTypeActions_SmartDialogue::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (UObject* Object : InObjects)
	{
		if (USmartDialogue* SmartDialogue = Cast<USmartDialogue>(Object))
		{
			TSharedRef<FSmartDialogueEditor> SmartDialogueEditor(new FSmartDialogueEditor());
			SmartDialogueEditor->InitSmartDialogueEditor(Mode, EditWithinLevelEditor, SmartDialogue);
		}
	}
}

#undef LOCTEXT_NAMESPACE