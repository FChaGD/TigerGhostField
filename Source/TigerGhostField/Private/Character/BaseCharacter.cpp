#include "Character/BaseCharacter.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::CalculateDamage(const FAttackParams& attackParam)
{
    hp -= attackParam.PhysicalDamage;
    if (hp <= 0.f && !bIsDead)
    {
        death();
    }
}

void ABaseCharacter::death()
{
	bIsDead = true;
}
