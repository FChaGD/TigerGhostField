// CombatComponent.h
// Author: refactor assistant
// Description: Actor component that provides basic combat-related utilities
//              such as triggered tracing and attack dispatch.  The component
//              exists to keep ABaseCharacter focused on ownership rather than
//              implementation details.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class UAttackBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TIGERGHOSTFIELD_API UCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCombatComponent();

    /**
     * Performs a box trace along the owner's forward vector and applies
     * damage to any ABaseCharacter hit.  This replicates the behaviour that
     * originally lived inside ABaseCharacter::BoxTrace.
     */
    void BoxTrace(UAttackBase* AttackType);
};
