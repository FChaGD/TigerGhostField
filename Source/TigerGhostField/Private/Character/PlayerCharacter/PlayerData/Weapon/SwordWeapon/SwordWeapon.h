#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Character/Attack/LightAttack.h"
#include "Character/Attack/HeavyAttack.h"
#include "Character/PlayerCharacter/PrivateInterface/IWeaponInterface.h"
#include "SwordWeapon.generated.h"

class ABaseCharacter;

UCLASS(Blueprintable)
class TIGERGHOSTFIELD_API USwordWeapon : public UObject, public IWeaponInterface
{
    GENERATED_BODY()

public:
    USwordWeapon();

    /** Owner character reference */
    UPROPERTY()
    ABaseCharacter* OwnerCharacter;

    /** Attack objects */
    UPROPERTY()
    ULightAttack* LightAttack;

    UPROPERTY()
    UHeavyAttack* HeavyAttack;

    /** Weapon mesh and montage */
    UPROPERTY()
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY()
    UAnimMontage* AttackMontage;

    /** Tracks consecutive attack count */
    UPROPERTY()
    int32 ConsecutiveAttackCount;

    /** Base physical damage */
    UPROPERTY()
    float BasePhysicalDamage;

    /** Executes a light attack */
    void ExecuteLightAttack();

    /** Executes a heavy attack */
    void ExecuteHeavyAttack();

    /** Resets combo sequence */
    void ResetSequence();

    /** IWeaponInterface implementation */
    virtual ULightAttack* GetLightAttack() const override { return LightAttack; }
    virtual UHeavyAttack* GetHeavyAttack() const override { return HeavyAttack; }
    virtual UAnimMontage* GetAttackMotion() const override { return AttackMontage; }
    virtual USkeletalMeshComponent* GetWeaponMesh() const override { return WeaponMesh; }
};
