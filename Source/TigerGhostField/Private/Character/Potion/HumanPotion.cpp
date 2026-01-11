// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Potion/HumanPotion.h"
#include "Character/ABaseCharacter.h"

#include "DebugHelper.h"

UHumanPotion::UHumanPotion()
{
	
}

void UHumanPotion::Execute()
{
	if (CurrentCount <= 0) return;
	
	CurrentCount -= 1;
	if (CurrentCount == 0)
	{
		CurrentState = EPotionState::Refilling;
		StartPotionRefill();
	}
	
	Debug::Print(TEXT("HumanPotion Execute"));
	
	// Core Stat restore
	OwnerCharacter->SetHp(ComputeFinalRecovery(OwnerCharacter->GetMaxHp(), ComputeResourceRecoveryPct(OwnerCharacter->GetMentalityPercent())));
	
	Debug::Print(TEXT("Human Potion Complete"));
}
