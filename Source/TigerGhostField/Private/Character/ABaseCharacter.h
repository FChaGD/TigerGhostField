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

#include "Character/Component/Stats/CharacterStatsComponent.h" // brings EMentalityState

// NOTE: This must be the LAST include in this header.
#include "ABaseCharacter.generated.h"

// Forward declarations
struct FAttackClassSet;

class UAttackBase;
class UAttackFactory;
class UPotionBase;
class UPotionFactory;
class UDA_RaceForm;

class UCombatComponent;
class URaceFormComponent;


UCLASS()
class TIGERGHOSTFIELD_API ABaseCharacter : public ACharacter, public ICharacterInterface, public IPawnUIInterface
{
    GENERATED_BODY()

private:
    // Components owned by the base character.  Each component encapsulates a
    // distinct slice of functionality (stats, combat helpers, race handling).
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
    UCharacterStatsComponent* StatsComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
    UCombatComponent* CombatComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
    URaceFormComponent* RaceComponent;

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
    
    bool IsDead() const;

    // utility accessors for underlying components
    UCharacterStatsComponent* GetStatsComponent() const { return StatsComponent; }
    UCombatComponent* GetCombatComponent() const { return CombatComponent; }
    URaceFormComponent* GetRaceComponent() const { return RaceComponent; }

private:
    // Death logic remains in the actor so that subclasses can override
    virtual void Death();

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
    /**
     * Convenience wrapper that delegates to the attached race component.
     * The underlying component owns all race-related data (forms, current
     * state, default type, etc.).
     */
    UFUNCTION(BlueprintCallable, Category = "Race")
    bool TransformToRace(ERaceType TargetRaceType);

    /**
     * Directly apply a form asset to the owner.  Exposed mainly for
     * backwards compatibility – most callers should use TransformToRace.
     */
    UFUNCTION(BlueprintCallable, Category = "Race")
    void ApplyRaceForm(const UDA_RaceForm* Form);

    UFUNCTION(BlueprintCallable, Category = "Race")
    ERaceType GetCurrentRaceType() const;

    /**
     * Returns the 'fallback' race type; originally stored on the actor but
     * now owned by the race component.  Provided for any callers that
     * still expect a default value.
     */
    UFUNCTION(BlueprintCallable, Category = "Race")
    ERaceType GetDefaultRaceType() const;
#pragma endregion
};
