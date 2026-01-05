// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PotionBase.generated.h"

/**
 * 
 */
UCLASS()
class UPotionBase : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Potion")
	uint8 CurrentCount = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin="1", ClampMax="5", UIMin="1", UIMax="5"), Category="Potion")
	uint8 UpgradeLevel = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Potion")
	TArray<float> HealPercentByLevel;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Potion")
	TArray<uint8> MaxCountByLevel;
	
public:
	virtual void Execute();
};
