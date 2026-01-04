// Author: kwak hyonbin
// Description: Base character class providing core properties such as health and stamina,
//              and implementing the ICharacterInterface for handling combat interactions.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/ICharacterInterface.h"
#include "Utill/ITriggerObserver.h"
#include "TypeZone/ZoneAffectComponent.h"
#include "ABaseCharacter.generated.h"

UENUM(BlueprintType)
enum class EMentalityState : uint8
{
	Normal UMETA(DisplayName = "Normal"),   // Default state
	Weak   UMETA(DisplayName = "Weak"),     // Mentality reached 0
	Fragile UMETA(DisplayName = "Fragile")  // After repeating same action 3 times in Weak state
};

//Implements core character functionality including health, stamina, and death handling.
//Inherits from ACharacter for Unreal Engine character behavior and ICharacterInterface
//for standardized combat interactions.
UCLASS()
class ABaseCharacter : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()
private:
	//--------------------------------
	int HpStatPoint = 0;
	int StaminaStatPoint = 0;
	int MentalityStatPoint = 0;
	int SpiritualPowerStatPoint = 0;
	int AttackPowerStatPoint = 0;

	//--------------------------------
	float Hp = 100.f;
	float MaxHp = 100.f;

	float PhysicalDef = 0.f;
	void SetPhysicalDef();
	//--------------------------------
	float Stamina = 100.f;
	float MaxStamina = 100.f;

	float PhysicalAttackCoefficient = 100.f;
	void SetPhysicalAttackCoefficient();
	//--------------------------------
	float Mentality = 100.f;
	float MaxMentality = 100.f;

	uint8 LastConsumedType = 255;
	int32 ConsecutiveSameActionCount = 0;
	EMentalityState MentalityState = EMentalityState::Normal;

public:
	void AddStatTrigger(const TScriptInterface<ITriggerObserver>& Trigger);
	void ConsumeStat(uint8 Type, float Amount);
private:
	void NotifyStatChanged(FName StatName);

	void SetMentalityState();
	float RecoveryCoefficient = 100.f;
	void SetRecoveryCoefficient();
	float DamageReceivedCoefficient = 100.f;
	void SetDamageReceivedCoefficient();
	//--------------------------------
	float SpiritualPower = 100.f;
	float MaxSpiritualPower = 100.f;

	float SpiritualDef = 0.f;
	void SetSpirituallDef();
	float SpiritualAttackCoefficient = 100.f;
	void SetSpirituallSpiritualAttackCoefficient();
	//--------------------------------
	float AttackPower = 100.f;

	//--------------------------------
	bool bIsDead = false;
	virtual void Death();

	UPROPERTY()
	TArray<TScriptInterface<ITriggerObserver>> StatTriggers;

public:
	/** Constructor that sets default values for the character's properties. */
	ABaseCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zone")
	TObjectPtr<UZoneAffectComponent> ZoneAffect = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Zone")
	EZoneType GetZoneType() const
	{
		return ZoneAffect ? ZoneAffect->GetCurrentZoneType() : EZoneType::None;
	}

protected:
	/** Called when the game starts or when spawned. */
	virtual void BeginPlay() override;

private:

	//ToDo : 30FrameTrigger
public:
	/**
	 * Calculates the effective damage of an incoming attack based on the character's
	 * current defensive state and the attack's interaction type.
	 * This does not directly modify the character's health; it only determines
	 * how much damage would be applied if the attack were resolved.
	 *
	 * @param attackParam Struct containing attack details such as raw damage and interaction type.
	 */
	virtual void CalculateDamage(const FAttackParams& attackParam) override;

	/**
	 * Handles the character's death, setting the dead flag and performing any additional logic.
	 */

};
