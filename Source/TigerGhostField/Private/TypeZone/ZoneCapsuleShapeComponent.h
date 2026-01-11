// Author: kwak hyonbin
// Description: Capsule shape component implementing the zone shape interface.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "TypeZone/ZoneShapeInterface.h"
#include "ZoneCapsuleShapeComponent.generated.h"

UCLASS(ClassGroup = (Zone), meta = (BlueprintSpawnableComponent))
class UZoneCapsuleShapeComponent : public UCapsuleComponent, public IZoneShapeInterface
{
	GENERATED_BODY()

public:
	virtual bool IsPointInside(const FVector& WorldPoint) const override;
};