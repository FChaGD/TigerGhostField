// Author: kwak hyonbin
// Description: This file centralizes all combat-related data structures used throughout the game.
//              It provides unified definitions for attack parameters, combat interaction types
//              (such as Guard, Parry, Counter, and Dodge), and other combat mechanics to ensure
//              consistency, clarity, and maintainability.

#pragma once

#include "CoreMinimal.h"
#include "FCombatStructs.generated.h"

/**
 * Enum representing the interaction type between an attack and the target's defensive state.
 * Each value determines how the attack behaves depending on the target's current action.
 */
UENUM(BlueprintType)
enum class ECombatInteractionType : uint8
{
    Guard       UMETA(DisplayName = "Guard"),   // Blocked if target is guarding
    Parry       UMETA(DisplayName = "Parry"),   // Can be parried if target timing is correct
    Counter     UMETA(DisplayName = "Counter"), // Triggers a counterattack if conditions are met
    Dodge       UMETA(DisplayName = "Dodge")    // Can be dodged by a moving target
};

/**
 * Struct containing all parameters needed to calculate the damage and interaction effect of an attack.
 * This includes raw damage values and the interaction type, which determines how the attack
 * interacts with the target's defensive state.
 */
USTRUCT(BlueprintType)
struct FAttackParams
{
    GENERATED_BODY()

public:
    /** Base raw physical damage value before any modifiers, resistances, or defensive interactions are applied. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PhysicalDamage = 0.f;

    /** Defines the attack's interaction type relative to the target's defensive state (Guard, Parry, Counter, Dodge). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECombatInteractionType AttackType = ECombatInteractionType::Dodge;
};
