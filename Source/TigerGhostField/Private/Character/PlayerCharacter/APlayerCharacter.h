// APlayerCharacter.h
// Author: kwak hyonbin
// Description: Player-controlled character class handling combat interaction notifications, camera control, input binding,
//              and race transformation (form swap).

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Character/FCombatStructs.h"
#include "Delegates/DelegateCombinations.h"
#include "Character/ABaseCharacter.h"
#include "Containers/Queue.h"
#include "PlayerData/Race/RaceTypes.h"
#include "PrivateInterface/IWeaponInterface.h"
#include "APlayerCharacter.generated.h"

// Forward declarations
class USpringArmComponent;
class UCameraComponent;
class UDA_InputDataAsset;
struct FInputActionValue;
class UPlayerUIComponent;

UCLASS()
class TIGERGHOSTFIELD_API APlayerCharacter : public ABaseCharacter
{
    GENERATED_BODY()

public:
    /**
     * Default constructor - sets up camera components and UI attachments.
     */
    APlayerCharacter();
	
	//~Begin IPawnUIInterface Interface
	// The pawn UI interface is implemented by the base class but the player
	// character exposes a typed version for convenience.
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UPlayerUIComponent* GetPlayerUIComponent() override;
	//~End IPawnUIInterface Interface


protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;

private:
#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDA_InputDataAsset* InputDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

	void Input_RunTrigger(const FInputActionValue& InputActionValue);
	void Input_RunComplete(const FInputActionValue& InputActionValue);

	void Input_LightAttack(const FInputActionValue& InputActionValue);
	void Input_HeavyAttack(const FInputActionValue& InputActionValue);
	
	void Input_RaceTransform(const FInputActionValue& InputActionValue);

	void Input_Potion(const FInputActionValue& InputActionValue);
#pragma endregion
    void ApplyHumanAsDefaultMesh();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void AttackNotificationEvent(ECombatInteractionType inputNotificationEvent);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    virtual void CalculateDamage(const FAttackParams& attackParam) override;

    ECombatInteractionType DefaultDeffenceType;
    ECombatInteractionType CurrentDeffenceType;
    ECombatInteractionType InputDeffenceType;

    static constexpr int32 HeadWidgetCount = static_cast<int32>(ECombatInteractionType::MAX);

    UPROPERTY(VisibleAnywhere, Category = "UI")
    TArray<TObjectPtr<UWidgetComponent>> HeadWidgets;

    static FString GetCombatInteractionName(ECombatInteractionType Type);

    TQueue<ECombatInteractionType> NotificationEventPop;

    void ProcessNextNotification();
    void AttackNotification();
    void Attack();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

	protected:
#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UI", meta = (AllowPrivateAccess = "true"))
	UPlayerUIComponent* PlayerUIComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UI")
	UWidgetComponent* HealthWidgetComponent;
#pragma endregion

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TScriptInterface<IWeaponInterface> WeaponInterface;
};
