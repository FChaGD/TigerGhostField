#include "TypeZone/ZoneCapsuleShapeComponent.h"

bool UZoneCapsuleShapeComponent::IsPointInside(const FVector& WorldPoint) const
{
	// Convert point into capsule local space to handle rotation.
	const FTransform T = GetComponentTransform();
	const FVector Local = T.InverseTransformPosition(WorldPoint);

	const float R = GetScaledCapsuleRadius();
	const float HalfH = GetScaledCapsuleHalfHeight();

	// Capsule axis is local Z.
	const float ClampedZ = FMath::Clamp(Local.Z, -HalfH, HalfH);
	const FVector Closest(Local.X, Local.Y, ClampedZ);

	return Closest.SizeSquared() <= (R * R);
}
