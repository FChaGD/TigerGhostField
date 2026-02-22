// ABaseCharacter.h
// Author: kwak hyonbin
// Description: Base character class providing core properties such as health and stamina,
//              and implementing the ICharacterInterface for handling combat interactions.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/ICharacterInterface.h"
#include "Interface/PawnUIInterface.h"
#include "Utill/ITriggerObserver.h"
#include "TypeZone/ZoneAffectComponent.h"
#include "Character/Attack/AttackTypes.h"
#include "Character/PlayerCharacter/PlayerData/Race/RaceTypes.h"

// NOTE: This must be the LAST include in this header.
#include "ABaseCharacter.generated.h"

// Forward declarations
struct FAttackClassSet;

class UAttackBase;
class UAttackFactory;
class UPotionBase;
class UPotionFactory;
class UDA_RaceForm;

UENUM(BlueprintType)
enum class EMentalityState : uint8
{
    Normal   UMETA(DisplayName = "Normal"),
    Weak     UMETA(DisplayName = "Weak"),
    Fragile  UMETA(DisplayName = "Fragile")
};

UCLASS()
class ABaseCharacter : public ACharacter, public ICharacterInterface, public IPawnUIInterface
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
    //~Begin IPawnUIInterface Interface
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
    //~End IPawnUIInterface Interface
    
    // English comment: Registers a stat trigger observer.
    void AddStatTrigger(const TScriptInterface<ITriggerObserver>& Trigger);

    // English comment: Consumes a stat by type and amount.
    void ConsumeStat(uint8 Type, float Amount);

    // English comment: Stat getters/setters (kept for existing cpp usage).
    float GetMentalityPercent() const;
    float GetMaxMentality() const;
    float GetMaxStamina() const;
    float GetMaxHp() const;

    float GetHp() const;
    float GetMentality() const;
    float GetStamina() const;

    void SetHp(float InHp);
    void SetMentality(float InMentality);
    void SetStamina(float InStamina);

    float GetAttackPower() const;
    
    bool IsDead() const                                     { return bIsDead; };

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
    // Base attack power (HEAD priority)
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
    // Zone affect component reference
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zone")
    TObjectPtr<UZoneAffectComponent> ZoneAffect = nullptr;

    // English comment: Returns the current zone type affecting this character.
    UFUNCTION(BlueprintCallable, Category = "Zone")
    EZoneType GetZoneType() const
    {
        return ZoneAffect ? ZoneAffect->GetCurrentZoneType() : EZoneType::None;
    }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

protected:
    // ------------------------------
    // Attack / Potion (BaseAttack priority)

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
    TSubclassOf<UAttackFactory> AttackFactoryClass;

    UPROPERTY()
    UAttackFactory* AttackFactory = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
    TSubclassOf<UPotionFactory> PotionFactoryClass;

    UPROPERTY()
    UPotionFactory* PotionFactory = nullptr;

    UPROPERTY()
    ULightAttack* CurrentLightAttack = nullptr;

    UPROPERTY()
    UHeavyAttack* CurrentHeavyAttack = nullptr;

    UPROPERTY()
    UPotionBase* CurrentPotion = nullptr;

public:
    /** Constructor that sets default values for the character's properties. */
    ABaseCharacter();

    // English comment: Temp attack damage apply (exists in your cpp).
    void BoxTrace(UAttackBase* AttackType);

public:
    // Calculates effective damage of an incoming attack based on defense state
    virtual void CalculateDamage(const FAttackParams& attackParam) override;

#pragma region Race
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Race", meta = (AllowPrivateAccess = "true"))
    ERaceType DefaultRaceType = ERaceType::Human;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Race", meta = (AllowPrivateAccess = "true"))
    ERaceType CurrentRaceType = ERaceType::Human;

    UPROPERTY(EditDefaultsOnly, Category = "Race", meta = (AllowPrivateAccess = "true"))
    TMap<ERaceType, TObjectPtr<UDA_RaceForm>> OwnedRaceForms;

protected:
    UFUNCTION(BlueprintCallable, Category = "Race")
    bool TransformToRace(ERaceType TargetRaceType);

    // English comment: Applies race form (mesh/anim/transform/capsule/movement).
    void ApplyRaceForm(const UDA_RaceForm* Form);

    UPROPERTY(VisibleInstanceOnly, Category = "Race")
    bool bHasAppliedRaceForm = false;
public:
    ERaceType GetCurrentRaceType() const { return CurrentRaceType; }
#pragma endregion
};
