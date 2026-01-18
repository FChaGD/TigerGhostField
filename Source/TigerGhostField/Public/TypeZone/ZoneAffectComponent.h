// Author: kwak hyonbin
// Description: Character-side zone caching using a tiny center probe and candidate overlaps.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "TypeZone/ZoneVolumeActor.h"
#include "ZoneAffectComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UZoneAffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UZoneAffectComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Zone")
	void RefreshZoneNow();

	UFUNCTION(BlueprintCallable, Category = "Zone")
	EZoneType GetCurrentZoneType() const { return CurrentType; }

	UFUNCTION(BlueprintCallable, Category = "Zone")
	AZoneVolumeActor* GetCurrentZone() const { return CurrentZone; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
	float ProbeRadius = 3.0f;

private:
	UPROPERTY()
	TObjectPtr<USphereComponent> CenterProbe = nullptr;

	UPROPERTY()
	TSet<TObjectPtr<AZoneVolumeActor>> Candidates;

	UPROPERTY()
	TObjectPtr<AZoneVolumeActor> CurrentZone = nullptr;

	// Current Attribute Type
	UPROPERTY()
	EZoneType CurrentType = EZoneType::None;

private:
	UFUNCTION()
	void OnProbeBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnProbeEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	AZoneVolumeActor* ResolveTopZone() const;
	void ApplyResolvedZone(AZoneVolumeActor* NewZone);
};
