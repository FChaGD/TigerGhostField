// Author: ju wonung
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
	// Attack Class 목록들
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	FAttackClassSet AttackClassSet;

public:
	ULightAttack* CreateLightAttack(int test);
	
	UHeavyAttack* CreateHeavyAttack(int test);
};
