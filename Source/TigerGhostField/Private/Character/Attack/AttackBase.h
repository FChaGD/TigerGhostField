// Author: Ju wonung
// Description: 

#pragma once

#include "CoreMinimal.h"

#include "AttackBase.generated.h"

/**
 * 
 */
UCLASS()
class UAttackBase : public UObject
{
	GENERATED_BODY()

public:
	UAttackBase();
	~UAttackBase();

protected:

	float BaseDamge;
	
	virtual void Execute();
};
