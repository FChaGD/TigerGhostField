// Author: Ju wonung
// Description: 

#pragma once

#include "CoreMinimal.h"

#include "AttackBase.generated.h"

class UDA_AttackData;
class ABaseCharacter;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UAttackBase : public UObject
{
	GENERATED_BODY()

public:
	UAttackBase();
	~UAttackBase();
	
	UFUNCTION(BlueprintCallable, Category="Attack")
	void Initialize(ABaseCharacter* InOwnerCharacter);

protected:
	float BaseDamge;
	bool bIsCross;
	bool bIsRepeat;
	int combo;
	ABaseCharacter* OwnerCharacter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDA_AttackData* DataAsset;
	
public:
	
	UFUNCTION(BlueprintCallable, Category="Attack")
	virtual void Execute();
	
	UFUNCTION(BlueprintCallable, Category="Attack")
	ABaseCharacter* GetOwnerCharacter() const;
};
