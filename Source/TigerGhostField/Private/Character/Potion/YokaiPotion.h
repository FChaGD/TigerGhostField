// Author: ju wonung
#pragma once

#include "CoreMinimal.h"
#include "Character/Potion/PotionBase.h"
#include "YokaiPotion.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UYokaiPotion : public UPotionBase
{
	GENERATED_BODY()
	
public:
	UYokaiPotion();
	
	virtual void Execute() override; 
};
