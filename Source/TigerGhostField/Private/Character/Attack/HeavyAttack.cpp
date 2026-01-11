// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attack/HeavyAttack.h"
#include "Character/ABaseCharacter.h"

void UHeavyAttack::Execute()
{
    UE_LOG(LogTemp, Warning, TEXT("Heavy Attack Execute"));
    if (OwnerCharacter->GetStamina() < 2.f)
        return;
	
	OwnerCharacter->BoxTrace(this);
}
