#include "TypeZone/ZoneVolumeActor.h"
#include "ZoneCapsuleShapeComponent.h"

static int32 GZoneSpawnCounter = 0;

AZoneVolumeActor::AZoneVolumeActor()
{
	PrimaryActorTick.bCanEverTick = false;

	ShapePrimitive = CreateDefaultSubobject<UZoneCapsuleShapeComponent>(TEXT("ZoneShape"));
	SetRootComponent(ShapePrimitive);

	// English comment: Use profile only. Do not override responses here.
	ShapePrimitive->SetCollisionProfileName(TEXT("ZoneVolume"));
	ShapePrimitive->SetGenerateOverlapEvents(true);
}

bool AZoneVolumeActor::IsPointInside(const FVector& WorldPoint) const
{
	if (!ShapePrimitive) return false;

	const IZoneShapeInterface* Shape = Cast<IZoneShapeInterface>(ShapePrimitive);
	if (!Shape) return false;

	return Shape->IsPointInside(WorldPoint);
}
void AZoneVolumeActor::BeginPlay()
{
	Super::BeginPlay();

	if (!ShapePrimitive)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ZoneVolume] No ShapePrimitive"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[ZoneVolume] %s Profile=%s Enabled=%d ObjType=%d GenOverlap=%d"),
		*GetName(),
		*ShapePrimitive->GetCollisionProfileName().ToString(),
		(int32)ShapePrimitive->GetCollisionEnabled(),
		(int32)ShapePrimitive->GetCollisionObjectType(),
		ShapePrimitive->GetGenerateOverlapEvents() ? 1 : 0
	);
}
