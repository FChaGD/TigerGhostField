// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PotionFactory.generated.h"

class UPotionBase;
class UHumanPotion;
class UWraithPotion;
class UYokaiPotion;

USTRUCT(BlueprintType) 
struct FPotionClassSet
{
	GENERATED_BODY()
	
	// 인간 포션 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	TSubclassOf<UHumanPotion> HumanPotionClass;
	
	// 망령 포션 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	TSubclassOf<UWraithPotion> WraithPotionClass;
	
	// 요괴 포션 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	TSubclassOf<UYokaiPotion> YokaiPotionClass;
	
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UPotionFactory : public UObject
{
	GENERATED_BODY()
	
protected:
	// Potion 목록들
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion")
	FPotionClassSet PotionClassSet;

public:
	UPotionBase* CreatePotion(int test);
};
