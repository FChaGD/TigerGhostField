#include "TypeZone/ZoneVolumeActor.h"
#include "ZoneCapsuleShapeComponent.h"

AZoneVolumeActor::AZoneVolumeActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Variation: Capsule shape (single variation for now).
	ShapePrimitive = CreateDefaultSubobject<UZoneCapsuleShapeComponent>(TEXT("ZoneShape"));
	SetRootComponent(ShapePrimitive);

	// Used for candidate filtering via overlap; final inclusion uses IsPointInside.
	ShapePrimitive->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShapePrimitive->SetCollisionResponseToAllChannels(ECR_Ignore);
	ShapePrimitive->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ShapePrimitive->SetGenerateOverlapEvents(true);
}

bool AZoneVolumeActor::IsPointInside(const FVector& WorldPoint) const
{
	if (!ShapePrimitive) return false;

	const IZoneShapeInterface* Shape = Cast<IZoneShapeInterface>(ShapePrimitive);
	if (!Shape) return false;

	return Shape->IsPointInside(WorldPoint);
}
