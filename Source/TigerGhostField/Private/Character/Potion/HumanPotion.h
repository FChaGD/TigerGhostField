// Author: ju wonung

#pragma once

#include "CoreMinimal.h"
#include "Character/Potion/PotionBase.h"
#include "HumanPotion.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UHumanPotion : public UPotionBase
{
	GENERATED_BODY()
	
public:
	UHumanPotion();
	
	virtual void Execute() override;
};
