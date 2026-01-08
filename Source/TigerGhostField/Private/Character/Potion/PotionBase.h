// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PotionBase.generated.h"

class ABaseCharacter;

UENUM(BlueprintType)
enum class EPotionState : uint8
{
	Ready,
	Refilling
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UPotionBase : public UObject
{
	GENERATED_BODY()
	
private:
	
public:
	UPotionBase();
	
	void Initialize(ABaseCharacter* InOwnerCharacter, uint8 Level, EPotionState PotionState);
	
	// mindPercentile: 0~100 (percentage)
	float ComputeResourceRecoveryPct(float mindPercentile);

	// coreMax: (MaxHP etc)
	// resourceRecoveryPct:
	float ComputeFinalRecovery(float coreMax, float resourceRecoveryPct);
	
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Potion")
	EPotionState CurrentState;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Potion")
	uint8 CurrentCount = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin="1", ClampMax="5", UIMin="1", UIMax="5"), Category="Potion")
	uint8 UpgradeLevel = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Potion")
	TArray<float> HealPercentByLevel;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Potion")
	TArray<uint8> MaxCountByLevel;
	
	UPROPERTY()
	ABaseCharacter* OwnerCharacter;

	UPROPERTY()
	FTimerHandle PotionRefillTimerHandle;
	
	bool bBossRefill;

public:
	
	UFUNCTION(BlueprintCallable, Category="Potion")
	virtual void Execute();
	
	UFUNCTION(BlueprintCallable, Category="Potion")
	void RefillPotion();
	
	UFUNCTION(BlueprintCallable, Category="Potion")
	void StartPotionRefill();
};
