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

	virtual void BeginPlay();
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
	int32 Priority = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
	EZoneType ZoneType = EZoneType::None;

	// English comment: Higher level wins when multiple zones overlap.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
	int32 ZoneLevel = 0;

	// English comment: Auto-assigned increasing order to break ties for same level.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zone")
	int32 SpawnOrder = 0;

protected:
	// Must implement IZoneShapeInterface.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zone")
	TObjectPtr<UPrimitiveComponent> ShapePrimitive = nullptr;

public:
	UFUNCTION(BlueprintCallable, Category = "Zone")
	bool IsPointInside(const FVector& WorldPoint) const;

	UFUNCTION(BlueprintCallable, Category = "Zone")
	UPrimitiveComponent* GetShapePrimitive() const { return ShapePrimitive; }
};
