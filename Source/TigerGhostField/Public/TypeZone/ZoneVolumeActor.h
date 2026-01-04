// Author: kwak hyonbin
// Description: Zone actor that owns an abstract shape component and provides zone type/priority.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TypeZone/ZoneShapeInterface.h"
#include "ZoneVolumeActor.generated.h"

UENUM(BlueprintType)
enum class EZoneType : uint8
{
	None UMETA(DisplayName = "None"),
	A    UMETA(DisplayName = "A"),
	B    UMETA(DisplayName = "B"),
	C    UMETA(DisplayName = "C"),
};

UCLASS()
class AZoneVolumeActor : public AActor
{
	GENERATED_BODY()

public:
	AZoneVolumeActor();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
	int32 Priority = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
	EZoneType ZoneType = EZoneType::None;

protected:
	// Must implement IZoneShapeInterface.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zone")
	TObjectPtr<UPrimitiveComponent> ShapePrimitive = nullptr;

public:
	// True if the point is inside the current shape.
	UFUNCTION(BlueprintCallable, Category = "Zone")
	bool IsPointInside(const FVector& WorldPoint) const;

	// Expose for overlap candidate filtering.
	UFUNCTION(BlueprintCallable, Category = "Zone")
	UPrimitiveComponent* GetShapePrimitive() const { return ShapePrimitive; }
};
