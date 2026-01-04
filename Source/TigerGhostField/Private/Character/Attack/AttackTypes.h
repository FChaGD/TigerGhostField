#pragma once 

#include "CoreMinimal.h" 
#include "Character/Attack/AttackBase.h"
#include "Character/Attack/HeavyAttack.h"
#include "Character/Attack/LightAttack.h"
#include "AttackTypes.generated.h" 

class ULightAttack;
class UHeavyAttack;

UENUM(BlueprintType) 
enum class EAttackInput : uint8 
{ 
	Light, 
	Heavy 
}; 

USTRUCT(BlueprintType) 
struct FAttackClassSet 
{ 
	GENERATED_BODY() 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	TSubclassOf<ULightAttack> LightAttackClass; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	TSubclassOf<UHeavyAttack> HeavyAttackClass; 
};