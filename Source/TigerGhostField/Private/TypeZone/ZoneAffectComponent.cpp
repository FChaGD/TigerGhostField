#include "TypeZone/ZoneAffectComponent.h"

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

	CenterProbe->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CenterProbe->SetCollisionResponseToAllChannels(ECR_Ignore);
	CenterProbe->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	CenterProbe->SetGenerateOverlapEvents(true);

	CenterProbe->OnComponentBeginOverlap.AddDynamic(this, &UZoneAffectComponent::OnProbeBeginOverlap);
	CenterProbe->OnComponentEndOverlap.AddDynamic(this, &UZoneAffectComponent::OnProbeEndOverlap);

	RefreshZoneNow();
}

void UZoneAffectComponent::OnProbeBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
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
	int32 BestPri = TNumericLimits<int32>::Lowest();

	for (const TObjectPtr<AZoneVolumeActor>& Z : Candidates)
	{
		if (!IsValid(Z)) continue;
		if (!Z->IsPointInside(Center)) continue;

		if (!Best || Z->Priority > BestPri)
		{
			Best = Z.Get();
			BestPri = Z->Priority;
		}
	}
	return Best;
}

void UZoneAffectComponent::ApplyResolvedZone(AZoneVolumeActor* NewZone)
{
	if (CurrentZone == NewZone) return;

	CurrentZone = NewZone;
	CurrentType = CurrentZone ? CurrentZone->ZoneType : EZoneType::None;

	// State transition stops here.
}

void UZoneAffectComponent::RefreshZoneNow()
{
	ApplyResolvedZone(ResolveTopZone());
}
