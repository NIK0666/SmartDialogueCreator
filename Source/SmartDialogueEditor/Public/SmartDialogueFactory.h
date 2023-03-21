// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SmartDialogueFactory.generated.h"

/**
 * 
 */
UCLASS()
class SMARTDIALOGUEEDITOR_API USmartDialogueFactory : public UFactory
{
	GENERATED_BODY()
	
	USmartDialogueFactory();
	
public:	
	UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn);
};
