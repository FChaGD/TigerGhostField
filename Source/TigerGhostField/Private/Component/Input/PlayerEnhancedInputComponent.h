// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "PlayerEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class UPlayerEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	// ¿Œ«≤ πŸ¿Œµ˘ 
	template<class UserObject, typename CallbackFunc>
	void BindInputAction(
		const UInputAction* InputAction,
		ETriggerEvent TriggerEvent,
		UserObject* ContextObject,
		CallbackFunc Func
	);
};

template<class UserObject, typename CallbackFunc>
inline void UPlayerEnhancedInputComponent::BindInputAction(const UInputAction* InputAction, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	BindAction(InputAction, TriggerEvent, ContextObject, Func);
}
