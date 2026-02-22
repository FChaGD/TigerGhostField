#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Character/Attack/AttackTypes.h"
#include "IWeaponInterface.generated.h"

// Forward declarations
class ULightAttack;
class UHeavyAttack;
class USkeletalMeshComponent;
class UAnimMontage;

/**
 * UInterface class for Blueprint exposure
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UWeaponInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * Interface for weapon objects providing access to attacks, motions, and mesh.
 */
class TIGERGHOSTFIELD_API IWeaponInterface
{
    GENERATED_BODY()

public:
    /** Returns the LightAttack object associated with this weapon */
    virtual ULightAttack* GetLightAttack() const = 0;

    /** Returns the HeavyAttack object associated with this weapon */
    virtual UHeavyAttack* GetHeavyAttack() const = 0;

    /** Returns the attack montage or motion associated with this weapon */
    virtual UAnimMontage* GetAttackMotion() const = 0;

    /** Returns the weapon mesh for rendering/equip purposes */
    virtual USkeletalMeshComponent* GetWeaponMesh() const = 0;
};
