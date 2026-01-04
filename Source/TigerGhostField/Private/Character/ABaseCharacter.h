// ABaseCharacter.h
// Author: kwak hyonbin
// Description: Base character class providing core properties such as health and stamina,
//              and implementing the ICharacterInterface for handling combat interactions.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/ICharacterInterface.h"
#include "Utill/ITriggerObserver.h"
#include "TypeZone/ZoneAffectComponent.h"
#include "Character/Attack/AttackTypes.h"
#include "Character/PlayerCharacter/PlayerData/RaceTypes.h"

// NOTE: This must be the LAST include in this header.
#include "ABaseCharacter.generated.h"

// Forward declarations
struct FAttackClassSet;
class UAttackBase;
class UAttackFactory;
class UDA_RaceForm;

// Mentality state definition
UENUM(BlueprintType)
enum class EMentalityState : uint8
{
    Normal   UMETA(DisplayName = "Normal"),
    Weak     UMETA(DisplayName = "Weak"),
    Fragile  UMETA(DisplayName = "Fragile")
};

UCLASS()
class ABaseCharacter : public ACharacter, public ICharacterInterface
{
    GENERATED_BODY()

private:
    // ------------------------------
    // Stat point values
    int HpStatPoint = 0;
    int StaminaStatPoint = 0;
    int MentalityStatPoint = 0;
    int SpiritualPowerStatPoint = 0;
    int AttackPowerStatPoint = 0;

    // ------------------------------
    // Health related values
    float Hp = 100.f;
    float MaxHp = 100.f;

    // Physical defense value
    float PhysicalDef = 0.f;
    void SetPhysicalDef();

    // ------------------------------
    // Stamina related values
    float Stamina = 100.f;
    float MaxStamina = 100.f;

    // Physical attack coefficient
    float PhysicalAttackCoefficient = 100.f;
    void SetPhysicalAttackCoefficient();

    // ------------------------------
    // Mentality related values
    float Mentality = 100.f;
    float MaxMentality = 100.f;

    // Last consumed action type
    uint8 LastConsumedType = 255;

    // Count of consecutive identical actions
    int32 ConsecutiveSameActionCount = 0;

    // Current mentality state
    EMentalityState MentalityState = EMentalityState::Normal;

public:
    // Registers a stat trigger observer
    void AddStatTrigger(const TScriptInterface<ITriggerObserver>& Trigger);

    // Consumes a stat by type and amount
    void ConsumeStat(uint8 Type, float Amount);

private:
    // Notifies observers that a stat value has changed
    void NotifyStatChanged(FName StatName);

    // Updates mentality state based on current conditions
    void SetMentalityState();

    // Recovery coefficient applied to regeneration logic
    float RecoveryCoefficient = 100.f;
    void SetRecoveryCoefficient();

    // Damage received coefficient applied to incoming damage
    float DamageReceivedCoefficient = 100.f;
    void SetDamageReceivedCoefficient();

    // ------------------------------
    // Spiritual power related values
    float SpiritualPower = 100.f;
    float MaxSpiritualPower = 100.f;

    // Spiritual defense value
    float SpiritualDef = 0.f;
    void SetSpirituallDef();

    // Spiritual attack coefficient
    float SpiritualAttackCoefficient = 100.f;
    void SetSpirituallSpiritualAttackCoefficient();

    // ------------------------------
    // Base attack power
    float AttackPower = 100.f;

    // ------------------------------
    // Death state flag
    bool bIsDead = false;

    // Handles character death logic
    virtual void Death();

    // Registered stat trigger observers
    UPROPERTY()
    TArray<TScriptInterface<ITriggerObserver>> StatTriggers;

protected:
    // Currently executing attack instance
    UPROPERTY(Transient)
    UAttackBase* CurrentAttack = nullptr;

public:
    // Constructor that sets default values for the character's properties
    ABaseCharacter();

protected:
    // Zone affect component reference
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zone")
    TObjectPtr<UZoneAffectComponent> ZoneAffect = nullptr;

    // Returns the current zone type affecting this character
    UFUNCTION(BlueprintCallable, Category = "Zone")
    EZoneType GetZoneType() const
    {
        return ZoneAffect ? ZoneAffect->GetCurrentZoneType() : EZoneType::None;
    }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Attack factory class reference
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UAttackFactory> AttackFactoryClass;

    // Attack factory instance
    UAttackFactory* AttackFactory = nullptr;

public:
    // Calculates effective damage of an incoming attack based on defense state
    virtual void CalculateDamage(const FAttackParams& attackParam) override;

#pragma region Race
protected:
    // Default race type assigned to this character
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Race", meta = (AllowPrivateAccess = "true"))
    ERaceType DefaultRaceType = ERaceType::Human;

    // Current race type applied to this character
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Race", meta = (AllowPrivateAccess = "true"))
    ERaceType CurrentRaceType = ERaceType::Human;

    // Owned race forms mapped by race type (if missing, transform is canceled)
    UPROPERTY(EditDefaultsOnly, Category = "Race", meta = (AllowPrivateAccess = "true"))
    TMap<ERaceType, TObjectPtr<UDA_RaceForm>> OwnedRaceForms;

protected:
    // Transforms this character to the target race type if data is available
    UFUNCTION(BlueprintCallable, Category = "Race")
    bool TransformToRace(ERaceType TargetRaceType);

    // Applies the minimum stable subset of a race form (mesh only for now)
    void ApplyRaceFormMinimal(const UDA_RaceForm* Form);
#pragma endregion
};
