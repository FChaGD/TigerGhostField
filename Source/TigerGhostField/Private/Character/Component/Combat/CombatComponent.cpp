// CombatComponent.cpp

#include "Character/Component/Combat/CombatComponent.h"
#include "Character/ABaseCharacter.h"
#include "Character/Attack/AttackBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"
#include "DebugHelper.h"

UCombatComponent::UCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BoxTrace(UAttackBase* AttackType)
{
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Combat] BoxTrace called with null owner"));
        return;
    }

    FVector Start = Owner->GetActorLocation();
    FVector End = Start + Owner->GetActorForwardVector() * 200.f;

    TArray<FHitResult> HitResults;
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(Owner);

    UE_LOG(LogTemp, Log, TEXT("[Combat] BoxTrace initiated (Attacker: %s, AttackType: %s, Start: %.0f,%.0f,%.0f -> End: %.0f,%.0f,%.0f)"), 
        *Owner->GetName(), AttackType ? *AttackType->GetName() : TEXT("None"), 
        Start.X, Start.Y, Start.Z, End.X, End.Y, End.Z);

    UKismetSystemLibrary::BoxTraceMultiForObjects(
        Owner->GetWorld(),
        Start,
        End,
        FVector(30.f, 50.f, 60.f),
        Owner->GetActorForwardVector().Rotation(),
        ObjectTypes,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::Persistent,
        HitResults,
        true
    );

    TSet<AActor*> UniqueActors;
    int32 HitCount = 0;
    
    for (const FHitResult& HitResult : HitResults)
    {
        AActor* Temp = HitResult.GetActor();
        if (!Temp || UniqueActors.Contains(Temp))
        {
            continue;
        }
        UniqueActors.Add(Temp);

        ABaseCharacter* TargetCharacter = Cast<ABaseCharacter>(Temp);
        if (!TargetCharacter)
        {
            UE_LOG(LogTemp, Verbose, TEXT("[Combat] BoxTrace hit non-character actor: %s"), *Temp->GetName());
            continue;
        }

        float SetAmount = 0.f;
        float DamageMultiplier = 1.0f;
        
        if (AttackType)
        {
            DamageMultiplier = AttackType->IsA(UHeavyAttack::StaticClass()) ? 1.5f : 0.8f;
            SetAmount = TargetCharacter->GetAttackPower() * DamageMultiplier;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[Combat] BoxTrace hit target %s but AttackType is null"), *TargetCharacter->GetName());
            SetAmount = TargetCharacter->GetAttackPower();
        }

        TargetCharacter->SetHp(-1.f * SetAmount);
        HitCount++;
        
        UE_LOG(LogTemp, Log, TEXT("[Combat] Hit target (Victim: %s, Damage: %.1f, Multiplier: %.2f, Remaining HP: %.1f/%.1f)"), 
            *TargetCharacter->GetName(), SetAmount, DamageMultiplier, TargetCharacter->GetHp(), TargetCharacter->GetMaxHp());

        if (UPawnUIComponent* PawnUIComponent = TargetCharacter->GetPawnUIComponent())
        {
            const float MaxHpValue = TargetCharacter->GetMaxHp();
            const float HpRatio = (MaxHpValue > 0.f) ? (TargetCharacter->GetHp() / MaxHpValue) : 0.f;
            PawnUIComponent->OnCurrentHealthChanged.Broadcast(HpRatio);
            UE_LOG(LogTemp, Verbose, TEXT("[Combat] UI health broadcast (Target: %s, HPRatio: %.2f)"), *TargetCharacter->GetName(), HpRatio);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("[Combat] BoxTrace completed (Total hits: %d, Unique targets: %d)"), HitResults.Num(), HitCount);
}
