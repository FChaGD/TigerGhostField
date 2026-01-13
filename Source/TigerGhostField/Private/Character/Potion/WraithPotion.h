// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Potion/PotionBase.h"
#include "WraithPotion.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UWraithPotion : public UPotionBase
{
	GENERATED_BODY()
	
public:
	UWraithPotion();
	
	virtual void Execute() override;
};
