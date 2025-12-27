#pragma once 

#include "CoreMinimal.h" 
#include "AttackTypes.generated.h" 

class UAttackBase; 

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
	TSubclassOf<UAttackBase> LightAttackClass; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	TSubclassOf<UAttackBase> HeavyAttackClass; 
};