// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Potion/WraithPotion.h"
#include "Character/ABaseCharacter.h"

#include "DebugHelper.h"

UWraithPotion::UWraithPotion()
{
}

void UWraithPotion::Execute()
{
	if (CurrentCount <= 0) return;
	
	CurrentCount -= 1;
	if (CurrentCount == 0)
	{
		CurrentState = EPotionState::Refilling;
		StartPotionRefill();
	}
	Debug::Print(TEXT("WraithPotion Execute"));
	
	// Core Stat restore
	OwnerCharacter->SetMentality(ComputeFinalRecovery(OwnerCharacter->GetMaxMentality(), ComputeResourceRecoveryPct(OwnerCharacter->GetMentalityPercent())));
	
	Debug::Print(TEXT("Warith Potion Complete"));
}
