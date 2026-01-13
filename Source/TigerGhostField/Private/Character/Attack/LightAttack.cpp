// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attack/LightAttack.h"
#include "Character/ABaseCharacter.h"

void ULightAttack::Execute()
{
    UE_LOG(LogTemp, Warning, TEXT("Lighty Attack Execute"));
    if (OwnerCharacter->GetStamina() < 3) 
        return;
	
	OwnerCharacter->BoxTrace(this);
}
