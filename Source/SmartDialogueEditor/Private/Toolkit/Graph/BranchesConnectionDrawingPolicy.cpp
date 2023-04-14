// FBranchesConnectionDrawingPolicy.cpp

#include "BranchesConnectionDrawingPolicy.h"

#include "BranchNode.h"
#include "SmartDialogueData.h"

bool FBranchesConnectionDrawingPolicy::IsHoveredPin(UEdGraphPin* Pin) const
{	
	return (Pin && HoveredPins.Contains(Pin));
}

void FBranchesConnectionDrawingPolicy::DrawSplineWithArrow(const FVector2D& StartPoint, const FVector2D& EndPoint, const FConnectionParams& Params)
{
	FConnectionParams ModifiedParams = Params;
	UEdGraphPin* StartPin = Params.AssociatedPin1;
	if (UBranchNode* StartNode = Cast<UBranchNode>(StartPin->GetOwningNode()))
	{
		if (StartPin->PinName == FName(""))
		{
			ModifiedParams.WireColor.R = 1.f;
			ModifiedParams.WireColor.G = 0.f;
			ModifiedParams.WireColor.B = 0.f;
			ModifiedParams.WireColor.A = 0.5f;
		}
		else if (StartPin->PinName == UEdGraphSchema_K2::PN_Then)
		{
			if (StartNode->GetBranchPtr() && !StartNode->GetBranchPtr()->Choice)
			{
				ModifiedParams.WireColor.R = 0.25f;
				ModifiedParams.WireColor.G = 1.f;
				ModifiedParams.WireColor.B = 0.25f;
			}
		}
	}
	
	// Check if either of the pins is hovered
	if (IsHoveredPin(Params.AssociatedPin1) || IsHoveredPin(Params.AssociatedPin2))
	{
		// Increase the wire thickness
		ModifiedParams.WireThickness *= 3.0f;
	}



	// Call the base implementation with the modified parameters
	FConnectionDrawingPolicy::DrawSplineWithArrow(StartPoint, EndPoint, ModifiedParams);
}
