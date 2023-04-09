//FBranchesConnectionDrawingPolicy.h

#pragma once

#include "CoreMinimal.h"
#include "ConnectionDrawingPolicy.h"

class FBranchesConnectionDrawingPolicy : public FConnectionDrawingPolicy
{
public:
	FBranchesConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements)
		: FConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements)
	{
	}

	bool IsHoveredPin(UEdGraphPin* Pin) const;
	virtual void DrawSplineWithArrow(const FVector2D& StartPoint, const FVector2D& EndPoint, const FConnectionParams& Params) override;
};
