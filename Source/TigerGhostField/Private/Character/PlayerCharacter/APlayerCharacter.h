// Author: kwak hyonbin
// Description:

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Character/FCombatStructs.h"
#include "Delegates/DelegateCombinations.h"
#include "Character/ABaseCharacter.h"
#include "APlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UDA_InputDataAsset;
struct FInputActionValue;

UCLASS()
class TIGERGHOSTFIELD_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Constructor that sets default values for the player character's properties.
	APlayerCharacter();

	// Combat notification entry point
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AttackNotificationEvent(ECombatInteractionType inputNotificationEvent);

	// Damage calculation override
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void CalculateDamage(const FAttackParams& attackParam) override;

protected:
	// Input binding
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Indicates the character's baseline defensive state when no specific input is provided.
	ECombatInteractionType DefaultDeffenceType;

	// Represents the defensive type currently active for hit detection.
	ECombatInteractionType CurrentDeffenceType;

	// Stores the defensive type requested by player input.
	ECombatInteractionType InputDeffenceType;

	static constexpr int32 HeadWidgetCount = static_cast<int32>(ECombatInteractionType::MAX);

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TArray<TObjectPtr<UWidgetComponent>> HeadWidgets;

	static FString GetCombatInteractionName(ECombatInteractionType Type);

	// Combat notification queue
	TQueue<ECombatInteractionType> NotificationEventPop;
	void ProcessNextNotification();
	void AttackNotification();

	void Attack();

	// Camera Components Start
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	// Camera Components End

#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDA_InputDataAsset* InputDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

	void Input_RunTrigger(const FInputActionValue& InputActionValue);
	void Input_RunComplete(const FInputActionValue& InputActionValue);

	void Input_LightAttack(const FInputActionValue& InputActionValue);
	void Input_HeavyAttack(const FInputActionValue& InputActionValue);
#pragma endregion
};
