// SBranchPin.cpp


#include "SBranchPin.h"
#include "BranchNode.h"

void SBranchPin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	SGraphPin::Construct(SGraphPin::FArguments(), InPin);
}

FSlateColor SBranchPin::GetPinColor() const
{
	// If the pin is the second output pin, set its color to red
	if (GraphPinObj->Direction == EGPD_Output && GraphPinObj->GetOwningNode()->Pins.IndexOfByKey(GraphPinObj) == 2)
	{
		return FSlateColor(FLinearColor::Red);
	}
	// Otherwise, use the default color
	return SGraphPin::GetPinColor();
}
