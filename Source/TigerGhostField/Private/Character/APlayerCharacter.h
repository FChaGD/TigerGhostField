// Author: kwak hyonbin
// Description: 

#pragma once

#include "CoreMinimal.h"
#include "Character/FCombatStructs.h"
#include "Character/ABaseCharacter.h"
#include "APlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TIGERGHOSTFIELD_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
private:
	
	// Indicates the character's baseline defensive state when no specific input is provided.
	// This value represents the passive or idle guard that is always available by default.
	ECombatInteractionType DefaultDeffenceType;

	// Represents the defensive type that is currently active and being evaluated for hit detection.
	// This value should not change while a defensive action is in progress, ensuring stable and
	// predictable defense behavior during ongoing animations or frames.
	ECombatInteractionType CurrentDeffenceType;

	// Stores the defensive type requested by player input. If a defensive action is already active,
	// this value is held in a queue-like manner and applied once the current defensive action
	// becomes replaceable. Used to prevent input loss and enable smooth defensive transitions.
	ECombatInteractionType InputDeffenceType;
	

	TQueue<ECombatInteractionType> NotificationEventPop;
	void ProcessNextNotification();
	void AttackNotification();

	void Attack();
public:	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AttackNotificationEvent(ECombatInteractionType inputNotificationEvent);

	virtual void CalculateDamage(const FAttackParams& attackParam) override;
};