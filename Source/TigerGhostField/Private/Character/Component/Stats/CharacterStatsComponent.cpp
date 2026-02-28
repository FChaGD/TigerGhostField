// CharacterStatsComponent.cpp
// Implementation of the UCharacterStatsComponent declared in the
// corresponding header.  This component encapsulates all of the
// character "numbers" and math so that ABaseCharacter remains a simple
// container/owner.

#include "Character/Component/Stats/CharacterStatsComponent.h"
#include "Character/ABaseCharacter.h" // required for NotifyStatChanged cast
#include "DebugHelper.h"
#include "Math/UnrealMathUtility.h"

UCharacterStatsComponent::UCharacterStatsComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterStatsComponent::BeginPlay()
{
    Super::BeginPlay();
    // Optionally clamp starting stats or perform initialization here.
}

float UCharacterStatsComponent::GetHp() const
{
    return Hp;
}

float UCharacterStatsComponent::GetMaxHp() const
{
    return MaxHp;
}

float UCharacterStatsComponent::GetStamina() const
{
    return Stamina;
}

float UCharacterStatsComponent::GetMaxStamina() const
{
    return MaxStamina;
}

float UCharacterStatsComponent::GetMentality() const
{
    return Mentality;
}

float UCharacterStatsComponent::GetMaxMentality() const
{
    return MaxMentality;
}

float UCharacterStatsComponent::GetAttackPower() const
{
    return AttackPower;
}

bool UCharacterStatsComponent::IsDead() const
{
    return bIsDead;
}

void UCharacterStatsComponent::SetHp(float Amount)
{
    float HpBefore = Hp;
    Hp = FMath::Clamp(Hp + Amount, 0.f, MaxHp);
    Debug::Print(TEXT("[Stats] Hp set : %f"), Hp);
    UE_LOG(LogTemp, Log, TEXT("[Stats] HP changed (Before: %.1f, After: %.1f, Delta: %.1f)"), 
        HpBefore, Hp, Amount);
    NotifyStatChanged(FName(TEXT("Hp")));
}

void UCharacterStatsComponent::SetStamina(float Amount)
{
    float StaminaBefore = Stamina;
    Stamina = FMath::Clamp(Stamina + Amount, 0.f, MaxStamina);
    Debug::Print(TEXT("[Stats] Stamina set : %f"), Stamina);
    UE_LOG(LogTemp, Log, TEXT("[Stats] Stamina changed (Before: %.1f, After: %.1f, Delta: %.1f)"), 
        StaminaBefore, Stamina, Amount);
    NotifyStatChanged(FName(TEXT("Stamina")));
}

void UCharacterStatsComponent::SetMentality(float Amount)
{
    float MentalityBefore = Mentality;
    Mentality = FMath::Clamp(Mentality + Amount, 0.f, MaxMentality);
    Debug::Print(TEXT("[Stats] Mentality set : %f"), Mentality);
    UE_LOG(LogTemp, Log, TEXT("[Stats] Mentality changed (Before: %.1f, After: %.1f, Delta: %.1f)"), 
        MentalityBefore, Mentality, Amount);
    NotifyStatChanged(FName(TEXT("Mentality")));
}

void UCharacterStatsComponent::AddStatTrigger(const TScriptInterface<ITriggerObserver>& Trigger)
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

void UCharacterStatsComponent::ConsumeStat(uint8 Type, float Amount)
{
    switch (Type)
    {
    case 0:
        Hp = FMath::Clamp(Hp - Amount, 0.f, MaxHp);
        UE_LOG(LogTemp, Log, TEXT("[Stats] Consumed HP (Amount: %.1f, Remaining: %.1f)"), Amount, Hp);
        break;
    case 1:
        Stamina = FMath::Clamp(Stamina - Amount, 0.f, MaxStamina);
        UE_LOG(LogTemp, Log, TEXT("[Stats] Consumed Stamina (Amount: %.1f, Remaining: %.1f)"), Amount, Stamina);
        break;
    case 2:
        Mentality = FMath::Clamp(Mentality - Amount, 0.f, MaxMentality);
        UE_LOG(LogTemp, Log, TEXT("[Stats] Consumed Mentality (Amount: %.1f, Remaining: %.1f)"), Amount, Mentality);
        break;
    case 3:
        SpiritualPower = FMath::Clamp(SpiritualPower - Amount, 0.f, MaxSpiritualPower);
        UE_LOG(LogTemp, Log, TEXT("[Stats] Consumed SpiritualPower (Amount: %.1f, Remaining: %.1f)"), Amount, SpiritualPower);
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("[Stats] Unknown stat type consumed: %d"), Type);
        break;
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
        UE_LOG(LogTemp, Log, TEXT("[Stats] Mentality weak state detected (Stat Type: %d, Consecutive Count: %d)"), 
            Type, ConsecutiveSameActionCount);
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

void UCharacterStatsComponent::CalculateDamage(const FAttackParams& attackParam)
{
    float HpBefore = Hp;
    float SpiritBefore = SpiritualPower;
    
    Hp -= attackParam.HPDamage * (1 - PhysicalDef / 100);
    SpiritualPower -= attackParam.SpiritualPowerDamage * (1 - SpiritualDef / 100);
    
    Hp = FMath::Clamp(Hp, 0.f, MaxHp);
    SpiritualPower = FMath::Clamp(SpiritualPower, 0.f, MaxSpiritualPower);
    
    SetPhysicalDef();
    SetSpirituallDef();
    SetSpirituallSpiritualAttackCoefficient();
    
    UE_LOG(LogTemp, Log, TEXT("[Combat] Damage applied (HP: %.1f -> %.1f, Spirit: %.1f -> %.1f, PhysicalDef: %.1f%%, SpiritDef: %.1f%%)"), 
        HpBefore, Hp, SpiritBefore, SpiritualPower, PhysicalDef, SpiritualDef);
    
    if ((SpiritualPower <= 0.f || Hp <= 0.f) && !bIsDead)
    {
        bIsDead = true;
        UE_LOG(LogTemp, Warning, TEXT("[Combat] Character reached death condition (HP: %.1f, Spirit: %.1f)"), Hp, SpiritualPower);
    }
}

#include "Character/ABaseCharacter.h" // needed for cast in NotifyStatChanged

void UCharacterStatsComponent::NotifyStatChanged(FName StatName)
{
    for (const TScriptInterface<ITriggerObserver>& Trigger : StatTriggers)
    {
        if (Trigger)
        {
            // interface expects an ABaseCharacter pointer
            Trigger->OnStatChanged(StatName, Cast<ABaseCharacter>(GetOwner()));
        }
    }

    OnStatChanged.Broadcast(StatName);
}

void UCharacterStatsComponent::SetPhysicalDef()
{
    float HpPercent = (MaxHp > 0.f) ? (Hp / MaxHp * 100.f) : 100.f;
    PhysicalDef = FMath::Clamp(55.f - 0.55f * HpPercent, 0.f, 50.f);
}

void UCharacterStatsComponent::SetPhysicalAttackCoefficient()
{
    float StaminaPercent = (MaxStamina > 0.f) ? (Stamina / MaxStamina * 100.f) : 100.f;
    PhysicalAttackCoefficient = FMath::Clamp(0.75f * StaminaPercent + 65.f, 80.f, 140.f);
}

void UCharacterStatsComponent::SetMentalityState()
{
    EMentalityState OldState = MentalityState;
    
    if (Mentality <= 0.f)
    {
        if (MentalityState != EMentalityState::Weak && MentalityState != EMentalityState::Fragile)
        {
            MentalityState = EMentalityState::Weak;
            ConsecutiveSameActionCount = 0;
            UE_LOG(LogTemp, Warning, TEXT("[Stats] MentalityState changed to Weak"));
        }
    }
    else
    {
        MentalityState = EMentalityState::Normal;
    }

    if (MentalityState == EMentalityState::Weak && ConsecutiveSameActionCount >= 3)
    {
        MentalityState = EMentalityState::Fragile;
        UE_LOG(LogTemp, Warning, TEXT("[Stats] MentalityState escalated to Fragile (Consecutive count: %d)"), ConsecutiveSameActionCount);
    }
    
    if (OldState != MentalityState)
    {
        UE_LOG(LogTemp, Log, TEXT("[Stats] MentalityState transitioned (%d -> %d)"), 
            static_cast<int32>(OldState), static_cast<int32>(MentalityState));
    }
}

void UCharacterStatsComponent::SetRecoveryCoefficient()
{
    float MentalityPercent = (MaxMentality > 0.f) ? (Mentality / MaxMentality * 100.f) : 100.f;
    RecoveryCoefficient = FMath::Clamp(2.42f * MentalityPercent + 8.f, 20.f, 250.f);
}

void UCharacterStatsComponent::SetDamageReceivedCoefficient()
{
    float MentalityPercent = (MaxMentality > 0.f) ? (Mentality / MaxMentality * 100.f) : 100.f;
    DamageReceivedCoefficient = FMath::Clamp(122.f - 0.42f * MentalityPercent, 80.f, 120.f);
}

void UCharacterStatsComponent::SetSpirituallDef()
{
    int SpiritualPowerValue = FMath::RoundToInt(SpiritualPower);
    SpiritualDef = 100.f + 5.f * SpiritualPowerValue;
}

void UCharacterStatsComponent::SetSpirituallSpiritualAttackCoefficient()
{
    int SpiritualPowerValue = FMath::RoundToInt(SpiritualPower);
    SpiritualAttackCoefficient = 100.f + 5.f * SpiritualPowerValue;
}
