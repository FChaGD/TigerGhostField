// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attack/HeavyAttack.h"
#include "Character/ABaseCharacter.h"

#include "DebugHelper.h"

void UHeavyAttack::Execute()
{
	
	if (!DataAsset)
	{
		Debug::Print(TEXT("No Attack Data Asset"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Heavy Attack Execute"));
		if (OwnerCharacter->GetStamina() < 3.f)
		{
			Debug::Print(TEXT("Stamina is less than 3"));
			return;
		}
	
		OwnerCharacter->BoxTrace(this);
	}
}
