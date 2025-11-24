// Author: kwak hyonbin
// Description: Base character class providing core properties such as health and stamina,
//              and implementing the ICharacterInterface for handling combat interactions.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/CharacterInterface.h"
#include "BaseCharacter.generated.h"

//Implements core character functionality including health, stamina, and death handling.
//Inherits from ACharacter for Unreal Engine character behavior and ICharacterInterface
//for standardized combat interactions.
UCLASS()
class ABaseCharacter : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

	float hp = 100.f;
	float maxHp = 100.f;
	float stamina = 100.f;
	float maxStamina = 100.f;

	bool bIsDead = false;

public:
	/** Constructor that sets default values for the character's properties. */
	ABaseCharacter();

protected:
	/** Called when the game starts or when spawned. */
	virtual void BeginPlay() override;

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
	virtual void death();
};
