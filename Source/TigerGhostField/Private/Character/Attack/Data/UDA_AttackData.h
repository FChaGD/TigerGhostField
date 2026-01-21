// Author: ju wonung
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UDA_AttackData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UDA_AttackData : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UAnimMontage* AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	float DamagePercent;
};
