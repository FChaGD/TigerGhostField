// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Potion/PotionBase.h"
#include "YokaiPotion.generated.h"

/**
 * 
 */
UCLASS()
class UYokaiPotion : public UPotionBase
{
	GENERATED_BODY()
	
public:
	virtual void Execute() override; 
};
