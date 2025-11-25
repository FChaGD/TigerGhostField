#include "Character/ABaseCharacter.h"

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
    //NOTE : Temporarily written content
    //TODO : Modification after plan confirmation
    Hp -= attackParam.PhysicalDamage;
    if (Hp <= 0.f && !bIsDead)
    {
        Death();
    }
}

void ABaseCharacter::Death()
{
	bIsDead = true;
}
