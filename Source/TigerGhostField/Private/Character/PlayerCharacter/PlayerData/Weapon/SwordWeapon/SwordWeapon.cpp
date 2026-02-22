#include "SwordWeapon.h"
#include "Character/ABaseCharacter.h"
#include "DebugHelper.h"

USwordWeapon::USwordWeapon()
{
    OwnerCharacter = nullptr;
    LightAttack = nullptr;
    HeavyAttack = nullptr;
    WeaponMesh = nullptr;
    AttackMontage = nullptr;

    BasePhysicalDamage = 80.f;
    ConsecutiveAttackCount = 0;
}

void USwordWeapon::ExecuteLightAttack()
{
    if (!LightAttack || !OwnerCharacter)
    {
        Debug::Print(TEXT("LightAttack or OwnerCharacter is null"));
        return;
    }

    if (OwnerCharacter->GetStamina() < 2.f)
    {
        Debug::Print(TEXT("Not enough stamina for light attack"));
        return;
    }

    // BoxTrace에서 내부적으로 타겟에게 피해 적용
    OwnerCharacter->BoxTrace(LightAttack);

    ConsecutiveAttackCount++; // 콤보 카운트만 SwordWeapon에서 증가
    UE_LOG(LogTemp, Warning, TEXT("Executed Light Attack!"));
}

void USwordWeapon::ExecuteHeavyAttack()
{
    if (!HeavyAttack || !OwnerCharacter)
    {
        Debug::Print(TEXT("HeavyAttack or OwnerCharacter is null"));
        return;
    }

    if (OwnerCharacter->GetStamina() < 5.f)
    {
        Debug::Print(TEXT("Not enough stamina for heavy attack"));
        return;
    }

    OwnerCharacter->BoxTrace(HeavyAttack);

    ConsecutiveAttackCount++;
    UE_LOG(LogTemp, Warning, TEXT("Executed Heavy Attack!"));
}


void USwordWeapon::ResetSequence()
{
    ConsecutiveAttackCount = 0;
}
