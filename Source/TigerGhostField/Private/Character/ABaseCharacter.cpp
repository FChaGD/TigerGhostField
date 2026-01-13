// ABaseCharacter.cpp
// Author: kwak hyonbin
// Description: Base character class implementing stats / combat calculations.
// NOTE: Race related behavior is added (minimal mesh swap only).

#include "Character/ABaseCharacter.h"

#include "DebugHelper.h"
#include "Math/UnrealMathUtility.h"
#include "Character/Attack/AttackBase.h"
#include "Component/Factory/AttackFactory.h"
#include "Component/Factory/PotionFactory.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Potion/PotionBase.h"

// NOTE: Race form data asset
#include "Character/PlayerCharacter/PlayerData/UDA_RaceForm.h"
#include "Components/SkeletalMeshComponent.h"

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
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn)); // 캐릭터만 맞추고 싶으면 Pawn

    // 5) Ignore Actors
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    UKismetSystemLibrary::BoxTraceMultiForObjects(
        GetWorld(),                                    // WorldContext
        GetActorLocation(),                             // Start (FVector)
        GetActorLocation() + GetActorForwardVector() * 200.f, // End (FVector)
        FVector(30.f, 50.f, 60.f),                      // Half Size (FVector)
        GetActorForwardVector().Rotation(),             // Orientation (FRotator)
        ObjectTypes,                                    // Object Types
        false,                                          // Trace Complex
        ActorsToIgnore,                                 // Actors to Ignore
        EDrawDebugTrace::Persistent,                    // Debug Draw
        HitResults,                                     // Out Hits
        true                                            // Ignore Self
    );

    TSet<AActor*> UniqueActors;
    for (const FHitResult& HitResult : HitResults)
    {
        AActor* temp = HitResult.GetActor();
        if (!temp) continue;
        if (UniqueActors.Contains(temp)) continue;
        UniqueActors.Add(temp);

        ABaseCharacter* TargetCharacter = Cast<ABaseCharacter>(temp);

        float SetAmount = AttackPower * (AttackType && AttackType->IsA(UHeavyAttack::StaticClass()) ? 1.5 : 0.8);

        if (!TargetCharacter)
            continue;
        else
        {
            // switch (TargetCharacter->GetCharacterRace())
            // {
            // case Race::human :
            //     TargetCharacter->SetHp(-1*SetAmount);
            //     Debug::Print(TEXT("Human Apply Damage"));
            //     break;
            // case Race::wraith :
            //     TargetCharacter->SetMentality(-1*SetAmount);
            //     Debug::Print(TEXT("Wraith Apply Damage"));
            //     break;
            // case Race::yokai:
            //     TargetCharacter->SetStamina(-1*SetAmount);
            //     Debug::Print(TEXT("Yokai Apply Damage"));
            //     break;
            // }
        }
    }
}

void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    // English comment: Keep factory initialization (was present in HEAD).
    // English comment: This is safe even if class is not set; NewObject will return nullptr.
    AttackFactory = NewObject<UAttackFactory>(this, AttackFactoryClass);

    // English comment: TODO - StateReset
    // ToDo : StateReset

    // NOTE: Apply default race if data exists. If not, keep original mesh as-is.
    TransformToRace(DefaultRaceType);
}

void ABaseCharacter::CalculateDamage(const FAttackParams& attackParam)
{
    //NOTE : Temporarily written content
    //TODO : Modification after plan confirmation
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
    return Mentality / MaxMentality;
}

float ABaseCharacter::GetMaxMentality() const
{
    return MaxMentality;
}

float ABaseCharacter::GetMaxStamina() const
{
    return MaxStamina;
}

float ABaseCharacter::GetHp() const
{
    return Hp;
}

float ABaseCharacter::GetMentality() const
{
    return Mentality;
}

float ABaseCharacter::GetStamina() const
{
    return Stamina;
}

float ABaseCharacter::GetMaxHp() const
{
    return MaxHp;
}

void ABaseCharacter::SetHp(float InHp)
{
    Hp = FMath::Clamp(Hp + InHp, 0, MaxHp);

    Debug::Print(TEXT("Hp Set : %f"), Hp);
}

void ABaseCharacter::SetMentality(float InMentality)
{
    Mentality = FMath::Clamp(Mentality + InMentality, 0, MaxMentality);

    Debug::Print(TEXT("Mentality Set : %f"), Mentality);
}

void ABaseCharacter::SetStamina(float InStamina)
{
    Stamina = FMath::Clamp(Stamina + InStamina, 0, MaxStamina);

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
    SpiritualDef = 100 + 5 * SpiritualPowerValue;
}

void ABaseCharacter::SetSpirituallSpiritualAttackCoefficient()
{
    int SpiritualPowerValue = FMath::RoundToInt(SpiritualPower);
    SpiritualAttackCoefficient = 100 + 5 * SpiritualPowerValue;
}

void ABaseCharacter::Death()
{
    //ToDo : HPCheck
    bIsDead = true;
}

bool ABaseCharacter::TransformToRace(ERaceType TargetRaceType)
{
    // English comment: If the same race is requested but we never applied any form yet,
    // we must continue to apply it at least once.
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

    ApplyRaceFormMinimal(Form);
    CurrentRaceType = TargetRaceType;

    // English comment: Mark as applied after a successful apply.
    bHasAppliedRaceForm = true;

    return true;
}

void ABaseCharacter::ApplyRaceFormMinimal(const UDA_RaceForm* Form)
{
    if (!Form)
    {
        return;
    }

    USkeletalMeshComponent* MeshComp = GetMesh();
    if (!MeshComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Race] ApplyRaceFormMinimal: Mesh component is null"));
        return;
    }

    if (!Form->SkeletalMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Race] ApplyRaceFormMinimal: SkeletalMesh is null"));
        return;
    }

    // English comment: Minimal stable apply - mesh swap only.
    MeshComp->SetSkeletalMesh(Form->SkeletalMesh);

    UE_LOG(LogTemp, Log, TEXT("[Race] Applied minimal form (mesh only): %d"), static_cast<int32>(Form->RaceType));
}
