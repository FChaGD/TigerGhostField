#include "Character/ABaseCharacter.h"
#include "Math/UnrealMathUtility.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	//ToDo : StateReset
}

void ABaseCharacter::CalculateDamage(const FAttackParams& attackParam)
{
    //NOTE : Temporarily written content
    //TODO : Modification after plan confirmation
    Hp -= attackParam.PhysicalDamage * (1 - PhysicalDef / 100);
    SpiritualPower -= attackParam.SpiritualDamage * (1 - SpiritualDef / 100);
    SetPhysicalDef();
    SetSpirituallDef();
    SetSpirituallSpiritualAttackCoefficient();
    UE_LOG(LogTemp, Warning, TEXT("Current HP: %.2f / %.2f"), Hp, MaxHp);
    if ((SpiritualPower <= 0.f || Hp <= 0.f) && !bIsDead)
    {
        Death();
    }
}

void ABaseCharacter::SetPhysicalDef()
{
    float HpPercent = (MaxHp > 0.f) ? (Hp / MaxHp * 100.f) : 100.f;

    PhysicalDef = FMath::Clamp(55.f - 0.55f * HpPercent, 0.f, 50.f);
}

void ABaseCharacter::SetPhysicalAttackCoefficient()
{
    float StaminaPercent = (MaxStamina > 0.f) ? (Stamina / MaxStamina * 100.f) : 100.f;

    PhysicalAttackCoefficient = FMath::Clamp(0.75f * StaminaPercent + 65.f, 80.f, 140.f);
}

void ABaseCharacter::SetMentalityState()
{
    if (Mentality <= 0.f)
    {
        if (MentalityState != EMentalityState::Weak && MentalityState != EMentalityState::Fragile)
        {
            MentalityState = EMentalityState::Weak;
            ConsecutiveSameActionCount = 0;
            UE_LOG(LogTemp, Warning, TEXT("MentalityState: Weak"));
        }
    }
    else
    {
        MentalityState = EMentalityState::Normal;
    }

    if (MentalityState == EMentalityState::Weak && ConsecutiveSameActionCount >= 3)
    {
        MentalityState = EMentalityState::Fragile;
        UE_LOG(LogTemp, Warning, TEXT("MentalityState: Fragile"));
    }
}


void ABaseCharacter::SetRecoveryCoefficient()
{
    float MentalityPercent = (MaxMentality > 0.f) ? (Mentality / MaxMentality * 100.f) : 100.f;

    RecoveryCoefficient = FMath::Clamp(2.42f * MentalityPercent + 8.f, 20.f, 250.f);
}

void ABaseCharacter::SetDamageReceivedCoefficient()
{
    float MentalityPercent = (MaxMentality > 0.f) ? (Mentality / MaxMentality * 100.f) : 100.f;

    DamageReceivedCoefficient = FMath::Clamp(122.f - 0.42f * MentalityPercent, 80.f, 120.f);
}

void ABaseCharacter::SetSpirituallDef()
{
    int SpiritualPowerValue = FMath::RoundToInt(SpiritualPower);
    SpiritualDef = 100 + 5 * SpiritualPowerValue;
}

void ABaseCharacter::SetSpirituallSpiritualAttackCoefficient()
{
    int SpiritualPowerValue = FMath::RoundToInt(SpiritualPower);
    SpiritualAttackCoefficient = 100 + 5 * SpiritualPowerValue;
}

void ABaseCharacter::Death()
{
    //ToDo : HPCheck
	bIsDead = true;
}
