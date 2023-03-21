// 


#include "SmartDialogueFactory.h"

#include "SmartDialogueFactory.h"
#include "SmartDialogue.h"

USmartDialogueFactory::USmartDialogueFactory()
{
	SupportedClass = USmartDialogue::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* USmartDialogueFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	USmartDialogue* NewSmartDialogue = NewObject<USmartDialogue>(InParent, Class, Name, Flags);

	// Add any additional initialization code here

	return NewSmartDialogue;
}