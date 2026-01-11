// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Potion/PotionBase.h"
#include "Character/ABaseCharacter.h"

#include "DebugHelper.h"

UPotionBase::UPotionBase()
{
	HealPercentByLevel = {0.2, 0.25, 0.3, 0.35, 0.4};
	MaxCountByLevel = {3,4,5,5,5};
}

void UPotionBase::Initialize(ABaseCharacter* InOwnerCharacter, uint8 Level, EPotionState State)
{
	OwnerCharacter = InOwnerCharacter;
	UpgradeLevel = Level;
	CurrentState = State;
	CurrentCount = MaxCountByLevel[UpgradeLevel];
}

float UPotionBase::ComputeResourceRecoveryPct(float mindPercentile)
{
	const float R = 2.42f * (mindPercentile / 8.f);
	return FMath::Max(20.f, R);
}

float UPotionBase::ComputeFinalRecovery(float coreMax, float resourceRecoveryPct)
{
	const float base = coreMax * HealPercentByLevel[UpgradeLevel];
	return base * (resourceRecoveryPct / 100.f);
}

void UPotionBase::Execute()
{
	// switch (OwnerCharacter->Race)
	// {
	// 	
	// }
}

void UPotionBase::RefillPotion()
{
	if (CurrentCount >= MaxCountByLevel[UpgradeLevel]) return;
	
	CurrentCount++;
	
	// Max Count, End Refill
	if (CurrentCount >= MaxCountByLevel[UpgradeLevel])
	{
		OwnerCharacter->GetWorldTimerManager().ClearTimer(PotionRefillTimerHandle);	
		Debug::Print(TEXT("Refill Complete"));
	}
	
	Debug::Print(TEXT("PotionRefill + 1"));
}

void UPotionBase::StartPotionRefill()
{
	if (!OwnerCharacter) return;
	
	// already set timer, do not restart
	if (OwnerCharacter->GetWorldTimerManager().IsTimerActive(PotionRefillTimerHandle))
		return;
	
	Debug::Print(TEXT("Set PotionRefillTimer, Refill Start"));
	
	OwnerCharacter->GetWorldTimerManager().SetTimer(
		PotionRefillTimerHandle,
		this, 
		&ThisClass::RefillPotion,
		5.f,	// temp period
		true);
}
