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
#include "PlayerData/RaceTypes.h"
#include "APlayerCharacter.generated.h"

// Forward declarations
class USpringArmComponent;
class UCameraComponent;
class UDA_InputDataAsset;
struct FInputActionValue;

UCLASS()
class TIGERGHOSTFIELD_API APlayerCharacter : public ABaseCharacter
{
    GENERATED_BODY()

public:
    APlayerCharacter();


protected:
    virtual void BeginPlay() override;

private:
    void ApplyHumanAsDefaultMesh();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void AttackNotificationEvent(ECombatInteractionType inputNotificationEvent);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    virtual void CalculateDamage(const FAttackParams& attackParam) override;

protected:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
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
#pragma endregion
};
