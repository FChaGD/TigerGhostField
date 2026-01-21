// ABaseCharacter.cpp
// Author: kwak hyonbin
// Description: Base character class implementing stats / combat calculations.
// NOTE: Race related behavior is added (full apply: mesh/anim/transform/capsule/movement).

#include "Character/ABaseCharacter.h"

#include "DebugHelper.h"
#include "Math/UnrealMathUtility.h"
#include "Character/Attack/AttackBase.h"
#include "Component/Factory/AttackFactory.h"
#include "Component/Factory/PotionFactory.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Potion/PotionBase.h"

// Race form data asset
#include "Character/PlayerCharacter/PlayerData/UDA_RaceForm.h"

// Components
#include "Component/UI/PawnUIComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseCharacter::ABaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    ZoneAffect = CreateDefaultSubobject<UZoneAffectComponent>(TEXT("ZoneAffect"));
}

void ABaseCharacter::BoxTrace(UAttackBase* AttackType)
{
    Debug::Print(TEXT("Attack Box Trace"));

    TArray<FHitResult> HitResults;

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    UKismetSystemLibrary::BoxTraceMultiForObjects(
        GetWorld(),
        GetActorLocation(),
        GetActorLocation() + GetActorForwardVector() * 200.f,
        FVector(30.f, 50.f, 60.f),
        GetActorForwardVector().Rotation(),
        ObjectTypes,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::Persistent,
        HitResults,
        true
    );

    TSet<AActor*> UniqueActors;
    for (const FHitResult& HitResult : HitResults)
    {
        AActor* Temp = HitResult.GetActor();
        if (!Temp) continue;
        if (UniqueActors.Contains(Temp)) continue;
        UniqueActors.Add(Temp);

        ABaseCharacter* TargetCharacter = Cast<ABaseCharacter>(Temp);

        float SetAmount = AttackPower * (AttackType && AttackType->IsA(UHeavyAttack::StaticClass()) ? 1.5f : 0.8f);

        if (!TargetCharacter)
        {
            continue;
        }

        // English comment: Minimal apply damage for current implementation.
        TargetCharacter->SetHp(-1.f * SetAmount);

        // English comment: Notify UI if the target has a UI component.
        if (UPawnUIComponent* PawnUIComponent = TargetCharacter->GetPawnUIComponent())
        {
            const float MaxHpValue = TargetCharacter->GetMaxHp();
            const float HpRatio = (MaxHpValue > 0.f) ? (TargetCharacter->GetHp() / MaxHpValue) : 0.f;
            PawnUIComponent->OnCurrentHealthChanged.Broadcast(HpRatio);
        }
    }
}

void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    // English comment: Create factory only if class is set (avoid NewObject(nullptr) assert).
    AttackFactory = nullptr;
    if (AttackFactoryClass)
    {
        AttackFactory = NewObject<UAttackFactory>(this, AttackFactoryClass);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[Combat] AttackFactoryClass is null (%s)"), *GetName());
    }

    // English comment: Apply race only in real game world (avoid editor preview/CDO).
    if (!GetWorld() || !GetWorld()->IsGameWorld())
    {
        return;
    }

    // English comment: Defer 1 tick to ensure components are registered.
    GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
        {
            if (!IsValid(this)) return;

            USkeletalMeshComponent* MeshComp = GetMesh();
            if (!MeshComp || !MeshComp->IsRegistered())
            {
                return;
            }

            TransformToRace(DefaultRaceType);
        });
}

void ABaseCharacter::CalculateDamage(const FAttackParams& attackParam)
{
    Hp -= attackParam.PhysicalDamage * (1 - PhysicalDef / 100);
    SpiritualPower -= attackParam.SpiritualDamage * (1 - SpiritualDef / 100);
    SetPhysicalDef();
    SetSpirituallDef();
    SetSpirituallSpiritualAttackCoefficient();
    UE_LOG(LogTemp, Warning, TEXT("Current HP: %.2f / %.2f"), Hp, MaxHp);
    if ((SpiritualPower <= 0.f || Hp <= 0.f) && !bIsDead)
    {
        Death();
    }
}

void ABaseCharacter::SetPhysicalDef()
{
    float HpPercent = (MaxHp > 0.f) ? (Hp / MaxHp * 100.f) : 100.f;
    PhysicalDef = FMath::Clamp(55.f - 0.55f * HpPercent, 0.f, 50.f);
}

void ABaseCharacter::SetPhysicalAttackCoefficient()
{
    float StaminaPercent = (MaxStamina > 0.f) ? (Stamina / MaxStamina * 100.f) : 100.f;
    PhysicalAttackCoefficient = FMath::Clamp(0.75f * StaminaPercent + 65.f, 80.f, 140.f);
}

UPawnUIComponent* ABaseCharacter::GetPawnUIComponent() const
{
    return nullptr;
}

void ABaseCharacter::AddStatTrigger(const TScriptInterface<ITriggerObserver>& Trigger)
{
    if (!Trigger)
    {
        return;
    }

    if (!StatTriggers.Contains(Trigger))
    {
        StatTriggers.Add(Trigger);
    }
}

void ABaseCharacter::ConsumeStat(uint8 Type, float Amount)
{
    switch (Type)
    {
    case 0: Hp = FMath::Clamp(Hp - Amount, 0.f, MaxHp); break;
    case 1: Stamina = FMath::Clamp(Stamina - Amount, 0.f, MaxStamina); break;
    case 2: Mentality = FMath::Clamp(Mentality - Amount, 0.f, MaxMentality); break;
    case 3: SpiritualPower = FMath::Clamp(SpiritualPower - Amount, 0.f, MaxSpiritualPower); break;
    default: break;
    }

    if (MentalityState == EMentalityState::Weak)
    {
        if (LastConsumedType == Type)
        {
            ConsecutiveSameActionCount++;
        }
        else
        {
            LastConsumedType = Type;
            ConsecutiveSameActionCount = 1;
        }
        UE_LOG(LogTemp, Warning, TEXT("Weak: Type=%d, Consecutive=%d"), Type, ConsecutiveSameActionCount);
    }

    FName StatName;
    switch (Type)
    {
    case 0: StatName = FName("Hp"); break;
    case 1: StatName = FName("Stamina"); break;
    case 2: StatName = FName("Mentality"); break;
    case 3: StatName = FName("SpiritualPower"); break;
    default: StatName = FName("Unknown"); break;
    }

    NotifyStatChanged(StatName);

    SetPhysicalDef();
    SetSpirituallDef();
    SetSpirituallSpiritualAttackCoefficient();
    SetMentalityState();
    SetRecoveryCoefficient();
    SetDamageReceivedCoefficient();
}

float ABaseCharacter::GetMentalityPercent() const
{
    return (MaxMentality > 0.f) ? (Mentality / MaxMentality) : 0.f;
}

float ABaseCharacter::GetMaxMentality() const { return MaxMentality; }
float ABaseCharacter::GetMaxStamina() const { return MaxStamina; }
float ABaseCharacter::GetHp() const { return Hp; }
float ABaseCharacter::GetMentality() const { return Mentality; }
float ABaseCharacter::GetStamina() const { return Stamina; }
float ABaseCharacter::GetMaxHp() const { return MaxHp; }

void ABaseCharacter::SetHp(float InHp)
{
    Hp = FMath::Clamp(Hp + InHp, 0.f, MaxHp);
    Debug::Print(TEXT("Hp Set : %f"), Hp);
}

void ABaseCharacter::SetMentality(float InMentality)
{
    Mentality = FMath::Clamp(Mentality + InMentality, 0.f, MaxMentality);
    Debug::Print(TEXT("Mentality Set : %f"), Mentality);
}

void ABaseCharacter::SetStamina(float InStamina)
{
    Stamina = FMath::Clamp(Stamina + InStamina, 0.f, MaxStamina);
    Debug::Print(TEXT("Stamina Set : %f"), Stamina);
}

float ABaseCharacter::GetAttackPower() const
{
    return AttackPower;
}

void ABaseCharacter::NotifyStatChanged(FName StatName)
{
    for (const TScriptInterface<ITriggerObserver>& Trigger : StatTriggers)
    {
        if (Trigger)
        {
            Trigger->OnStatChanged(StatName, this);
        }
    }
}

void ABaseCharacter::SetMentalityState()
{
    if (Mentality <= 0.f)
    {
        if (MentalityState != EMentalityState::Weak && MentalityState != EMentalityState::Fragile)
        {
            MentalityState = EMentalityState::Weak;
            ConsecutiveSameActionCount = 0;
            UE_LOG(LogTemp, Warning, TEXT("MentalityState: Weak"));
        }
    }
    else
    {
        MentalityState = EMentalityState::Normal;
    }

    if (MentalityState == EMentalityState::Weak && ConsecutiveSameActionCount >= 3)
    {
        MentalityState = EMentalityState::Fragile;
        UE_LOG(LogTemp, Warning, TEXT("MentalityState: Fragile"));
    }
}

void ABaseCharacter::SetRecoveryCoefficient()
{
    float MentalityPercent = (MaxMentality > 0.f) ? (Mentality / MaxMentality * 100.f) : 100.f;
    RecoveryCoefficient = FMath::Clamp(2.42f * MentalityPercent + 8.f, 20.f, 250.f);
}

void ABaseCharacter::SetDamageReceivedCoefficient()
{
    float MentalityPercent = (MaxMentality > 0.f) ? (Mentality / MaxMentality * 100.f) : 100.f;
    DamageReceivedCoefficient = FMath::Clamp(122.f - 0.42f * MentalityPercent, 80.f, 120.f);
}

void ABaseCharacter::SetSpirituallDef()
{
    int SpiritualPowerValue = FMath::RoundToInt(SpiritualPower);
    SpiritualDef = 100.f + 5.f * SpiritualPowerValue;
}

void ABaseCharacter::SetSpirituallSpiritualAttackCoefficient()
{
    int SpiritualPowerValue = FMath::RoundToInt(SpiritualPower);
    SpiritualAttackCoefficient = 100.f + 5.f * SpiritualPowerValue;
}

void ABaseCharacter::Death()
{
    bIsDead = true;
}

bool ABaseCharacter::TransformToRace(ERaceType TargetRaceType)
{
    if (TargetRaceType == CurrentRaceType && bHasAppliedRaceForm)
    {
        return true;
    }

    const TObjectPtr<UDA_RaceForm>* FoundPtr = OwnedRaceForms.Find(TargetRaceType);
    if (!FoundPtr || !(*FoundPtr))
    {
        UE_LOG(LogTemp, Warning, TEXT("[Race] Transform canceled: No data for target race"));
        return false;
    }

    const UDA_RaceForm* Form = (*FoundPtr);
    if (!Form->IsValidForm())
    {
        UE_LOG(LogTemp, Warning, TEXT("[Race] Transform canceled: Invalid form data"));
        return false;
    }

    ApplyRaceForm(Form);
    CurrentRaceType = TargetRaceType;
    bHasAppliedRaceForm = true;

    return true;
}

void ABaseCharacter::ApplyRaceForm(const UDA_RaceForm* Form)
{
    if (!Form)
    {
        return;
    }

    USkeletalMeshComponent* MeshComp = GetMesh();
    if (!MeshComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Race] ApplyRaceForm: Mesh component is null"));
        return;
    }

    if (Form->SkeletalMesh)
    {
#if ENGINE_MAJOR_VERSION >= 5
        MeshComp->SetSkeletalMesh(Form->SkeletalMesh, true);
#else
        MeshComp->SetSkeletalMesh(Form->SkeletalMesh);
        MeshComp->InitAnim(true);
#endif
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[Race] ApplyRaceForm: SkeletalMesh is null"));
    }

    if (Form->AnimClass)
    {
        MeshComp->SetAnimationMode(EAnimationMode::AnimationBlueprint);
        MeshComp->SetAnimInstanceClass(Form->AnimClass);
        MeshComp->InitAnim(true);
    }

    if (UCapsuleComponent* Cap = GetCapsuleComponent())
    {
        Cap->SetCapsuleSize(Form->CapsuleRadius, Form->CapsuleHalfHeight, true);

        FVector Loc = Form->MeshRelativeLocation;
        Loc.Z = -Form->CapsuleHalfHeight;
        MeshComp->SetRelativeLocation(Loc);
    }
    else
    {
        MeshComp->SetRelativeLocation(Form->MeshRelativeLocation);
    }

    MeshComp->SetRelativeRotation(Form->MeshRelativeRotation);
    MeshComp->SetRelativeScale3D(Form->MeshRelativeScale);

    if (UCharacterMovementComponent* Move = GetCharacterMovement())
    {
        Move->MaxWalkSpeed = Form->MaxWalkSpeed;
        Move->JumpZVelocity = Form->JumpZVelocity;
        Move->RotationRate = FRotator(0.f, Form->RotationRateYaw, 0.f);
    }

    UE_LOG(LogTemp, Warning, TEXT("[Race] Applied: Mesh=%s AnimClass=%s Loc=%s Rot=%s Scale=%s CapR=%.1f CapHH=%.1f Speed=%.1f Jump=%.1f RotYaw=%.1f"),
        *GetNameSafe(MeshComp->SkeletalMesh),
        *GetNameSafe(MeshComp->GetAnimClass()),
        *MeshComp->GetRelativeLocation().ToString(),
        *MeshComp->GetRelativeRotation().ToString(),
        *MeshComp->GetRelativeScale3D().ToString(),
        GetCapsuleComponent() ? GetCapsuleComponent()->GetUnscaledCapsuleRadius() : -1.f,
        GetCapsuleComponent() ? GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() : -1.f,
        GetCharacterMovement() ? GetCharacterMovement()->MaxWalkSpeed : -1.f,
        GetCharacterMovement() ? GetCharacterMovement()->JumpZVelocity : -1.f,
        GetCharacterMovement() ? GetCharacterMovement()->RotationRate.Yaw : -1.f
    );
}
