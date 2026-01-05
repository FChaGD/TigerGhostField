// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Potion/PotionBase.h"
#include "HumanPotion.generated.h"

/**
 * 
 */
UCLASS()
class UHumanPotion : public UPotionBase
{
	GENERATED_BODY()
	
public:
	virtual void Execute() override;
};
