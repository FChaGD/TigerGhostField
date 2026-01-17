// Author: ju wonung
#include "Component/Factory/AttackFactory.h"
#include "Character/Attack/AttackTypes.h"

// Light Attack Create Function
ULightAttack* UAttackFactory::CreateLightAttack(int test)
{
        UE_LOG(LogTemp, Warning, TEXT("Create Light Attack"));
    TSubclassOf<ULightAttack> AttackClass = nullptr;
    switch (test)
    {
    case 0:
        AttackClass = AttackClassSet.LightAttackClass1;
        break;
    case 1:
        AttackClass = AttackClassSet.LightAttackClass2;
        break;
    case 2:
        AttackClass = AttackClassSet.LightAttackClass3;
        break;
    default:
        break;
    }

    if (!AttackClass) return nullptr;

    return NewObject<ULightAttack>(this, AttackClass);
}

// Heavy Attack Create Function
UHeavyAttack* UAttackFactory::CreateHeavyAttack(int test)
{
        UE_LOG(LogTemp, Warning, TEXT("Create Light Attack"));
    TSubclassOf<UHeavyAttack> AttackClass = nullptr;
    switch (test)
    {
    case 0:
        AttackClass = AttackClassSet.HeavyAttackClass1;
        break;
    case 1:
        AttackClass = AttackClassSet.HeavyAttackClass2;
        break;
    case 2:
        AttackClass = AttackClassSet.HeavyAttackClass3;
        break;
    default:
        break;
    }

    if (!AttackClass) return nullptr;

    return NewObject<UHeavyAttack>(this, AttackClass);
}
