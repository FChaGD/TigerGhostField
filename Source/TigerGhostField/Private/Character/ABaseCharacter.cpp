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
#include "Character/PlayerCharacter/PlayerData/Race/UDA_RaceForm.h"

// Components
#include "Component/UI/PawnUIComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
// Newly extracted subcomponents
#include "Character/Component/Stats/CharacterStatsComponent.h"
#include "Character/Component/Combat/CombatComponent.h"
#include "Character/Component/Race/RaceFormComponent.h"
ABaseCharacter::ABaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    UE_LOG(LogTemp, Log, TEXT("[BaseCharacter] Constructor called"));

    // component ownership
    StatsComponent = CreateDefaultSubobject<UCharacterStatsComponent>(TEXT("StatsComponent"));
    CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
    RaceComponent = CreateDefaultSubobject<URaceFormComponent>(TEXT("RaceComponent"));
    ZoneAffect = CreateDefaultSubobject<UZoneAffectComponent>(TEXT("ZoneAffect"));
    UE_LOG(LogTemp, Log, TEXT("[BaseCharacter] Components created: Stats, Combat, Race, Zone"));
}

void ABaseCharacter::BoxTrace(UAttackBase* AttackType)
{
    if (CombatComponent)
    {
        UE_LOG(LogTemp, Log, TEXT("[Combat] BoxTrace initiated (Actor: %s, AttackType: %s)"), 
            *GetName(), AttackType ? *AttackType->GetClass()->GetName() : TEXT("None"));
        CombatComponent->BoxTrace(AttackType);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[Combat] CombatComponent is null on %s"), *GetName());
    }
}

void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Log, TEXT("[BaseCharacter] BeginPlay started for %s"), *GetName());

    // Ensure factories are created; combat systems may rely on them.
    AttackFactory = nullptr;
    if (AttackFactoryClass)
    {
        AttackFactory = NewObject<UAttackFactory>(this, AttackFactoryClass);
        UE_LOG(LogTemp, Log, TEXT("[Combat] AttackFactory created successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[Combat] AttackFactoryClass is not set for %s"), *GetName());
    }

    PotionFactory = nullptr;
    if (PotionFactoryClass)
    {
        PotionFactory = NewObject<UPotionFactory>(this, PotionFactoryClass);
        UE_LOG(LogTemp, Log, TEXT("[Combat] PotionFactory created successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[Combat] PotionFactoryClass is not set for %s"), *GetName());
    }

    UE_LOG(LogTemp, Log, TEXT("[BaseCharacter] BeginPlay completed for %s"), *GetName());
}

void ABaseCharacter::CalculateDamage(const FAttackParams& attackParam)
{
    if (StatsComponent)
    {
        float HpBefore = StatsComponent->GetHp();
        StatsComponent->CalculateDamage(attackParam);
        float HpAfter = StatsComponent->GetHp();
        UE_LOG(LogTemp, Log, TEXT("[Combat] Damage calculated (Actor: %s, HPDamage: %.1f, Before: %.1f, After: %.1f)"), 
            *GetName(), attackParam.HPDamage, HpBefore, HpAfter);
        
        if (StatsComponent->IsDead())
        {
            UE_LOG(LogTemp, Warning, TEXT("[Combat] Character died: %s"), *GetName());
            Death();
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[Combat] StatsComponent is null on %s"), *GetName());
    }
}
UPawnUIComponent* ABaseCharacter::GetPawnUIComponent() const
{
    // Base returns null; derived classes may override to provide their own UI
    return nullptr;
}

void ABaseCharacter::AddStatTrigger(const TScriptInterface<ITriggerObserver>& Trigger)
{
    if (StatsComponent)
    {
        StatsComponent->AddStatTrigger(Trigger);
    }
}

void ABaseCharacter::ConsumeStat(uint8 Type, float Amount)
{
    if (StatsComponent)
    {
        StatsComponent->ConsumeStat(Type, Amount);
    }
}

float ABaseCharacter::GetMentalityPercent() const
{
    return StatsComponent ? StatsComponent->GetMentality() / StatsComponent->GetMaxMentality() : 0.f;
}

float ABaseCharacter::GetMaxMentality() const { return StatsComponent ? StatsComponent->GetMaxMentality() : 0.f; }
float ABaseCharacter::GetMaxStamina() const { return StatsComponent ? StatsComponent->GetMaxStamina() : 0.f; }
float ABaseCharacter::GetHp() const { return StatsComponent ? StatsComponent->GetHp() : 0.f; }
float ABaseCharacter::GetMentality() const { return StatsComponent ? StatsComponent->GetMentality() : 0.f; }
float ABaseCharacter::GetStamina() const { return StatsComponent ? StatsComponent->GetStamina() : 0.f; }
float ABaseCharacter::GetMaxHp() const { return StatsComponent ? StatsComponent->GetMaxHp() : 0.f; }

void ABaseCharacter::SetHp(float InHp)
{
    if (StatsComponent) StatsComponent->SetHp(InHp);
}

void ABaseCharacter::SetMentality(float InMentality)
{
    if (StatsComponent) StatsComponent->SetMentality(InMentality);
}

void ABaseCharacter::SetStamina(float InStamina)
{
    if (StatsComponent) StatsComponent->SetStamina(InStamina);
}

float ABaseCharacter::GetAttackPower() const
{
    return StatsComponent ? StatsComponent->GetAttackPower() : 0.f;
}

bool ABaseCharacter::IsDead() const
{
    return StatsComponent ? StatsComponent->IsDead() : false;
}

void ABaseCharacter::Death()
{
    UE_LOG(LogTemp, Warning, TEXT("[BaseCharacter] Death event triggered for %s"), *GetName());
    // subclasses can override to perform visual/audio death effects
}

bool ABaseCharacter::TransformToRace(ERaceType TargetRaceType)
{
    if (!RaceComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("[Race] RaceComponent is null on %s"), *GetName());
        return false;
    }
    
    ERaceType CurrentRace = RaceComponent->GetCurrentRaceType();
    UE_LOG(LogTemp, Log, TEXT("[Race] Transform initiated (Actor: %s, Current: %d, Target: %d)"), 
        *GetName(), static_cast<int32>(CurrentRace), static_cast<int32>(TargetRaceType));
    
    bool bSuccess = RaceComponent->TransformToRace(TargetRaceType);
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("[Race] Transform succeeded to race %d"), static_cast<int32>(TargetRaceType));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[Race] Transform failed to race %d"), static_cast<int32>(TargetRaceType));
    }
    return bSuccess;
}

void ABaseCharacter::ApplyRaceForm(const UDA_RaceForm* Form)
{
    if (RaceComponent)
    {
        RaceComponent->ApplyRaceForm(Form);
    }
}

ERaceType ABaseCharacter::GetCurrentRaceType() const
{
    return RaceComponent ? RaceComponent->GetCurrentRaceType() : ERaceType::Human;
}

ERaceType ABaseCharacter::GetDefaultRaceType() const
{
    // forward to component; if component is missing fall back to human.
    return RaceComponent ? RaceComponent->GetDefaultRaceType() : ERaceType::Human;
}
