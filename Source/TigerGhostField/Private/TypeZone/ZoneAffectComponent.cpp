#include "TypeZone/ZoneAffectComponent.h"

#include "Components/SphereComponent.h"
#include "TypeZone/ZoneVolumeActor.h"

UZoneAffectComponent::UZoneAffectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UZoneAffectComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (!Owner) return;

	CenterProbe = NewObject<USphereComponent>(Owner, TEXT("CenterProbe"));
	if (!CenterProbe) return;

	CenterProbe->RegisterComponent();
	CenterProbe->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	CenterProbe->SetRelativeLocation(FVector::ZeroVector);
	CenterProbe->SetSphereRadius(ProbeRadius);

	CenterProbe->SetCollisionProfileName(TEXT("CenterProbe"));
	CenterProbe->SetGenerateOverlapEvents(true);

	CenterProbe->OnComponentBeginOverlap.AddDynamic(this, &UZoneAffectComponent::OnProbeBeginOverlap);
	CenterProbe->OnComponentEndOverlap.AddDynamic(this, &UZoneAffectComponent::OnProbeEndOverlap);

	RefreshZoneNow();
	UE_LOG(LogTemp, Warning, TEXT("[ZoneProbe] BeginPlay Owner=%s CenterProbe=%s Radius=%.1f"),
		GetOwner() ? *GetOwner()->GetName() : TEXT("None"),
		CenterProbe ? *CenterProbe->GetName() : TEXT("None"),
		ProbeRadius);

}

void UZoneAffectComponent::OnProbeBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("[ZoneProbe] BeginOverlap OtherActor=%s OtherComp=%s"),
		OtherActor ? *OtherActor->GetName() : TEXT("None"),
		OtherComp ? *OtherComp->GetName() : TEXT("None"));


	if (AZoneVolumeActor* Zone = Cast<AZoneVolumeActor>(OtherActor))
	{
		Candidates.Add(Zone);
		RefreshZoneNow();
	}
}

void UZoneAffectComponent::OnProbeEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (AZoneVolumeActor* Zone = Cast<AZoneVolumeActor>(OtherActor))
	{
		Candidates.Remove(Zone);
		RefreshZoneNow();
	}
}

AZoneVolumeActor* UZoneAffectComponent::ResolveTopZone() const
{
	if (!CenterProbe) return nullptr;

	const FVector Center = CenterProbe->GetComponentLocation();

	AZoneVolumeActor* Best = nullptr;
	int32 BestLevel = TNumericLimits<int32>::Lowest();
	int32 BestOrder = TNumericLimits<int32>::Lowest();

	for (const TObjectPtr<AZoneVolumeActor>& Z : Candidates)
	{
		if (!IsValid(Z)) continue;
		if (!Z->IsPointInside(Center)) continue;

		const int32 L = Z->ZoneLevel;
		const int32 O = Z->SpawnOrder;

		// English comment: Higher level wins. If same level, later spawn order wins.
		const bool bBetter =
			(!Best) ||
			(L > BestLevel) ||
			(L == BestLevel && O > BestOrder);

		if (bBetter)
		{
			Best = Z.Get();
			BestLevel = L;
			BestOrder = O;
		}
	}

	return Best;
}

void UZoneAffectComponent::ApplyResolvedZone(AZoneVolumeActor* NewZone)
{
	if (CurrentZone == NewZone) return;

	CurrentZone = NewZone;
	CurrentType = CurrentZone ? CurrentZone->ZoneType : EZoneType::None;

	// English comment: Optional debug.
	UE_LOG(LogTemp, Warning, TEXT("[Zone] Resolved => %s Type=%d Level=%d Order=%d"),
		CurrentZone ? *CurrentZone->GetName() : TEXT("None"),
		static_cast<int32>(CurrentType),
		CurrentZone ? CurrentZone->ZoneLevel : -1,
		CurrentZone ? CurrentZone->SpawnOrder : -1);
}

void UZoneAffectComponent::RefreshZoneNow()
{
	ApplyResolvedZone(ResolveTopZone());
}
