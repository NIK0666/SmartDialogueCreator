
// MyGraphEdge.h
#pragma once

#include "CoreMinimal.h"
#include "MyGraphEdge.generated.h"

class UMyGraphNode;

UCLASS(Blueprintable)
class SMARTDIALOGUECORE_API UMyGraphEdge : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY()
    UMyGraphNode* NodeA;

    UPROPERTY()
    UMyGraphNode* NodeB;
};