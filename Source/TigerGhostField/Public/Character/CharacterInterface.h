// Author: kwak hyonbin
// Description: Defines common interaction functions that external systems can use to communicate with character actors.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Character/CombatStructs.h"
#include "CharacterInterface.generated.h"

// This class is designed for polymorphism and provides a unified interface for character-related interactions.
UINTERFACE(MinimalAPI)
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class TIGERGHOSTFIELD_API ICharacterInterface
{
	GENERATED_BODY()

public:
    /**
     * Calculates the effective damage of an incoming attack based on the character's
     * current defensive state and the attack's interaction type.
     * This does not directly modify the character's health; it only determines
     * how much damage would be applied if the attack were resolved.
     *
     * @param attackParam Struct containing attack details such as raw damage and interaction type.
     */
	virtual void CalculateDamage(const FAttackParams& attackParam) = 0;
};
