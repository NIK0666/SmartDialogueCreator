// SmartDialogueActions.cpp
#include "SmartDialogueActions.h"
#include "SmartDialogue.h"

FSmartDialogueActions::FSmartDialogueActions(EAssetTypeCategories::Type InCategory)
{
	DialogueCategoryBit = InCategory;
}

FText FSmartDialogueActions::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_SmartDialogue", "Smart Dialogue");
}

UClass* FSmartDialogueActions::GetSupportedClass() const
{
	return USmartDialogue::StaticClass();
}

FColor FSmartDialogueActions::GetTypeColor() const
{
	return FColor(255, 50, 50);
}

uint32 FSmartDialogueActions::GetCategories()
{
	return DialogueCategoryBit;
}