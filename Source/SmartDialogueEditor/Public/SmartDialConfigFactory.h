// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SmartDialConfigFactory.generated.h"

/**
 * 
 */
UCLASS()
class SMARTDIALOGUEEDITOR_API USmartDialConfigFactory : public UFactory
{
	GENERATED_BODY()
	
	USmartDialConfigFactory();
	
public:	
	UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn);
};
