// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Attack/AttackTypes.h"

#include "AttackFactory.generated.h"

struct FAttackClassSet;
class UAttackBase;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UAttackFactory : public UObject
{
	GENERATED_BODY()

protected:
	// 공격 클래스 세트 (팩토리 입력)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	FAttackClassSet AttackClassSet;

public:
	UAttackBase* CreateAttack(EAttackInput Ininput, int test);
};
