// SmartDialogueActions.cpp
#include "SmartDialogueActions.h"
#include "SmartDialogue.h"

FSmartDialogueActions::FSmartDialogueActions(EAssetTypeCategories::Type InCategory): DialogueCategoryBit(InCategory)
{
}

FText FSmartDialogueActions::GetName() const
{
	return FText::FromString(TEXT("Smart Dialogue"));
}

UClass* FSmartDialogueActions::GetSupportedClass() const
{
	return USmartDialogue::StaticClass();
}

FColor FSmartDialogueActions::GetTypeColor() const
{
	return FColor::Purple;
}

uint32 FSmartDialogueActions::GetCategories()
{
	return DialogueCategoryBit;
}