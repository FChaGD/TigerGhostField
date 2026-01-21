// Author: ju wonung
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
	
	UFUNCTION(BlueprintCallable, Category="Potion")
	void Initialize(ABaseCharacter* InOwnerCharacter, uint8 Level, EPotionState PotionState);
	
	// mindPercentile: 0~100 (percentage)
	UFUNCTION(BlueprintCallable, Category="Potion")
	float ComputeResourceRecoveryPct(float mindPercentile);

	// coreMax: (MaxHP, Mentality, Stamina)
	// resourceRecoveryPct:
	UFUNCTION(BlueprintCallable, Category="Potion")
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
	
	UPROPERTY()
	float BossRefillPercent = 0.0f;

public:
	
	UFUNCTION(BlueprintCallable, Category="Potion")
	virtual void Execute();
	
	UFUNCTION(BlueprintCallable, Category="Potion")
	void RefillPotion();
	
	UFUNCTION(BlueprintCallable, Category="Potion")
	void StartPotionRefill();
};
