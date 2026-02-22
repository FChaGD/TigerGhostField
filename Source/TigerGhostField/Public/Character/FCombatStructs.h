#pragma once

#include "CoreMinimal.h"
#include "FCombatStructs.generated.h"

/**
 * Enum representing the defensive interaction type for an attack (Guard, Parry, Counter, Dodge).
 */
UENUM(BlueprintType)
enum class ECombatInteractionType : uint8
{
    Guard       UMETA(DisplayName = "Guard"),   // Blocked if target is guarding
    Parry       UMETA(DisplayName = "Parry"),   // Can be parried if target timing is correct
    Counter     UMETA(DisplayName = "Counter"), // Triggers a counterattack if conditions are met
    Dodge       UMETA(DisplayName = "Dodge"),   // Can be dodged by a moving target
    MAX         UMETA(Hidden)
};

/**
 * Enum representing the combat type for an attack (ABC style naming).
 */
UENUM(BlueprintType)
enum class ECombatType : uint8
{
    A    UMETA(DisplayName = "Guard"),
    B    UMETA(DisplayName = "Parry"),
    C    UMETA(DisplayName = "Counter"),
    MAX  UMETA(Hidden)
};

/**
 * Struct for attack parameters including multiple damage types and defensive interaction.
 */
USTRUCT(BlueprintType)
struct FAttackParams
{
    GENERATED_BODY()

public:
    /** Base HP damage */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Damage")
    float HPDamage = 0.f;

    /** Base Stamina damage */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Damage")
    float StaminaDamage = 0.f;

    /** Base Mantality damage */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Damage")
    float MantalityDamage = 0.f;

    /** Base SpiritualPower damage */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Damage")
    float SpiritualPowerDamage = 0.f;

    /** Defines the attack's defensive interaction type (Guard, Parry, Counter, Dodge) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Interaction")
    ECombatInteractionType GuardType = ECombatInteractionType::Dodge;

    /** Defines the attack's combat type (ABC style) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Interaction")
    ECombatType CombatType = ECombatType::A;
};
