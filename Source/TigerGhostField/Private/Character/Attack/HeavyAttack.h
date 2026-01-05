// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Attack/AttackBase.h"
#include "HeavyAttack.generated.h"

/**
 * 
 */
UCLASS()
class UHeavyAttack : public UAttackBase
{
	GENERATED_BODY()

public:
	virtual void Execute() override;
};
