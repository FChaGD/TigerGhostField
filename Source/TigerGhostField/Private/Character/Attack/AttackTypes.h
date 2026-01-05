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
	
	// Human Light Attack Class
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	TSubclassOf<ULightAttack> LightAttackClass1;
	
	// Human Light Attack Class
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	TSubclassOf<ULightAttack> LightAttackClass2;
	
	// Human Light Attack Class
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	TSubclassOf<ULightAttack> LightAttackClass3;
	
	// Human Heavy Attack Class
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	TSubclassOf<UHeavyAttack> HeavyAttackClass1; 
	
	// Human Heavy Attack Class
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	TSubclassOf<UHeavyAttack> HeavyAttackClass2; 
	
	// Human Heavy Attack Class
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	TSubclassOf<UHeavyAttack> HeavyAttackClass3; 
};