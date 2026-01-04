// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Factory/AttackFactory.h"
#include "Character/Attack/AttackTypes.h"

UAttackBase* UAttackFactory::CreateAttack(EAttackInput Ininput, int test)
{
    TSubclassOf<UAttackBase> AttackClass = nullptr;
    switch (Ininput)
    {
    case EAttackInput::Light:
        switch (test)
        {
        case 0:
            AttackClass = AttackClassSet.LightAttackClass;
            break;
        case 1:
            AttackClass = AttackClassSet.LightAttackClass;
            break;
        default:
            break;
        }
        break;
    case EAttackInput::Heavy:
        switch (test)
        {
        case 0:
            AttackClass = AttackClassSet.HeavyAttackClass;
            break;
        case 1:
            AttackClass = AttackClassSet.HeavyAttackClass;
            break;
        default:
            break;
        }
    default:
        break;
    }

    if (!AttackClass) return nullptr;

    return NewObject<UAttackBase>(this, AttackClass);
}
