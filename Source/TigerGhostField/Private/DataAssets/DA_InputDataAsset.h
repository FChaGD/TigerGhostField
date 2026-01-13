// Author: Ju wonung
// Description: 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_InputDataAsset.generated.h"

class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class UDA_InputDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* RunInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* LookInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* LightAttackInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* HeavyAttackInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* RaceTransformInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* PotionInputAction;
};
