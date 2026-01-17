// Author: ju wonung

#include "Character/Potion/YokaiPotion.h"
#include "Character/ABaseCharacter.h"

#include "DebugHelper.h"

UYokaiPotion::UYokaiPotion()
{
}

void UYokaiPotion::Execute()
{
	if (CurrentCount <= 0) return;
	
	CurrentCount -= 1;
	if (CurrentCount == 0)
	{
		CurrentState = EPotionState::Refilling;
		StartPotionRefill();
	}
	
	Debug::Print(TEXT("YokaiPotion Execute"));
	
	// Core Stat restore
	OwnerCharacter->SetStamina(ComputeFinalRecovery(OwnerCharacter->GetMaxStamina(), ComputeResourceRecoveryPct(OwnerCharacter->GetMentalityPercent())));
	
	Debug::Print(TEXT("Yokai Potion Complete"));
}
