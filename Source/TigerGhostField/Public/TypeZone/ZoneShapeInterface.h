// Author: kwak hyonbin
// Description: Interface for zone shapes that can test whether a world point is inside.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZoneShapeInterface.generated.h"

UINTERFACE()
class UZoneShapeInterface : public UInterface
{
	GENERATED_BODY()
};

class IZoneShapeInterface
{
	GENERATED_BODY()

public:
	// Tests whether the given point (world space) is inside this shape.
	virtual bool IsPointInside(const FVector& WorldPoint) const = 0;
};
