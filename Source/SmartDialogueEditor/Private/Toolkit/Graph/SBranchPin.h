// 

#pragma once

#include "CoreMinimal.h"
#include "SGraphPin.h"

class SGraphNode_Branch;

class SBranchPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SBranchPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

protected:
	// Override GetPinColor function
	virtual FSlateColor GetPinColor() const override;
};