// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attack/LightAttack.h"
#include "Character/ABaseCharacter.h"

#include "DebugHelper.h"

void ULightAttack::Execute()
{
	if (!DataAsset)
	{
		Debug::Print(TEXT("No Attack Data Asset"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Lighty Attack Execute"));
		if (OwnerCharacter->GetStamina() < 2)
		{
			Debug::Print(TEXT("Stamina is less than 2"));
			return;
		}
	
		OwnerCharacter->BoxTrace(this);
	}
}
