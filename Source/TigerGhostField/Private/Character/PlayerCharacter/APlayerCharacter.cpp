// APlayerCharacter.cpp
// Author: kwak hyonbin
// Description: Player character input bindings (movement/combat/race transform) and basic stubs.

#include "APlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/DA_InputDataAsset.h"
#include "Component/Input/PlayerEnhancedInputComponent.h"
#include "Component/Factory/AttackFactory.h"
#include "Character/Attack/AttackBase.h"

APlayerCharacter::APlayerCharacter()
{
}

void APlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
    const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
    const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

    if (MovementVector.Y != 0.f)
    {
        const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
        AddMovementInput(ForwardDirection, MovementVector.Y);
    }

    if (MovementVector.X != 0.f)
    {
        const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void APlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
    const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

    if (LookAxisVector.X != 0.f)
    {
        AddControllerYawInput(LookAxisVector.X);
    }

    if (LookAxisVector.Y != 0.f)
    {
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void APlayerCharacter::Input_RunTrigger(const FInputActionValue& InputActionValue)
{
    GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void APlayerCharacter::Input_RunComplete(const FInputActionValue& InputActionValue)
{
    GetCharacterMovement()->MaxWalkSpeed = 400.f;
}

void APlayerCharacter::Input_LightAttack(const FInputActionValue& InputActionValue)
{
    UE_LOG(LogTemp, Warning, TEXT("Light Attack"));

    if (!AttackFactory)
    {
        UE_LOG(LogTemp, Warning, TEXT("AttackFactory is null"));
        return;
    }

    CurrentAttack = AttackFactory->CreateAttack(EAttackInput::Light, 0);
    if (CurrentAttack)
    {
        CurrentAttack->Execute();
    }
}


void APlayerCharacter::Input_HeavyAttack(const FInputActionValue& InputActionValue)
{
    UE_LOG(LogTemp, Warning, TEXT("Heavy Attack"));

    if (!AttackFactory)
    {
        UE_LOG(LogTemp, Warning, TEXT("AttackFactory is null"));
        return;
    }

    CurrentAttack = AttackFactory->CreateAttack(EAttackInput::Heavy, 0);
    if (CurrentAttack)
    {
        CurrentAttack->Execute();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("NO Heavy Attack"));
    }
}

void APlayerCharacter::Input_RaceTransform(const FInputActionValue& InputActionValue)
{
    const bool bPressed = InputActionValue.Get<bool>();
    if (!bPressed)
    {
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Race Transform Input"));

    ERaceType TargetRaceType = CurrentRaceType;

    switch (CurrentRaceType)
    {
    case ERaceType::Human:
        TargetRaceType = ERaceType::Wraith;
        break;
    case ERaceType::Wraith:
        TargetRaceType = ERaceType::Yokai;
        break;
    case ERaceType::Yokai:
        TargetRaceType = ERaceType::Human;
        break;
    default:
        TargetRaceType = DefaultRaceType;
        break;
    }

    const bool bOk = TransformToRace(TargetRaceType);
    UE_LOG(LogTemp, Warning, TEXT("TransformToRace => %s"), bOk ? TEXT("true") : TEXT("false"));
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (!InputDataAsset || !InputDataAsset->DefaultMappingContext)
    {
        UE_LOG(LogTemp, Warning, TEXT("InputDataAsset or DefaultMappingContext is null"));
        return;
    }

    ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
    UEnhancedInputLocalPlayerSubsystem* SubSystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

    UPlayerEnhancedInputComponent* PlayerInput =
        Cast<UPlayerEnhancedInputComponent>(PlayerInputComponent);

    if (!SubSystem || !PlayerInput)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnhancedInput subsystem or PlayerInput component is null"));
        return;
    }

    SubSystem->AddMappingContext(InputDataAsset->DefaultMappingContext, 0);

    if (InputDataAsset->MoveInputAction)
    {
        PlayerInput->BindInputAction(
            InputDataAsset->MoveInputAction,
            ETriggerEvent::Triggered,
            this,
            &ThisClass::Input_Move);
    }

    if (InputDataAsset->LookInputAction)
    {
        PlayerInput->BindInputAction(
            InputDataAsset->LookInputAction,
            ETriggerEvent::Triggered,
            this,
            &ThisClass::Input_Look);
    }

    if (InputDataAsset->RunInputAction)
    {
        PlayerInput->BindInputAction(
            InputDataAsset->RunInputAction,
            ETriggerEvent::Triggered,
            this,
            &ThisClass::Input_RunTrigger);

        PlayerInput->BindInputAction(
            InputDataAsset->RunInputAction,
            ETriggerEvent::Completed,
            this,
            &ThisClass::Input_RunComplete);

        PlayerInput->BindInputAction(
            InputDataAsset->RunInputAction,
            ETriggerEvent::Canceled,
            this,
            &ThisClass::Input_RunComplete);
    }

    if (InputDataAsset->LightAttackInputAction)
    {
        PlayerInput->BindInputAction(
            InputDataAsset->LightAttackInputAction,
            ETriggerEvent::Started,
            this,
            &ThisClass::Input_LightAttack);
    }

    if (InputDataAsset->HeavyAttackInputAction)
    {
        PlayerInput->BindInputAction(
            InputDataAsset->HeavyAttackInputAction,
            ETriggerEvent::Started,
            this,
            &ThisClass::Input_HeavyAttack);
    }

    if (InputDataAsset->RaceTransformInputAction)
    {
        PlayerInput->BindInputAction(
            InputDataAsset->RaceTransformInputAction,
            ETriggerEvent::Started,
            this,
            &ThisClass::Input_RaceTransform);
    }
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    // English comment: Character begin play.
    UE_LOG(LogTemp, Warning, TEXT("[Player] BeginPlay Name=%s"), *GetName());

    // English comment: Initial zone state from ZoneAffect.
    const EZoneType Z = GetZoneType();
    UE_LOG(LogTemp, Warning, TEXT("[Player] Initial ZoneType=%d"), static_cast<int32>(Z));

    // English comment: Optional - print zone actor too.
    if (ZoneAffect)
    {
        AZoneVolumeActor* Zone = ZoneAffect->GetCurrentZone();
        UE_LOG(LogTemp, Warning, TEXT("[Player] Initial ZoneActor=%s"),
            Zone ? *Zone->GetName() : TEXT("None"));
    }
}


void APlayerCharacter::ApplyHumanAsDefaultMesh()
{
    const ERaceType HumanType = ERaceType::Human;

    UE_LOG(LogTemp, Warning, TEXT("[Race] Try ApplyHumanAsDefaultMesh. CurrentRaceType=%d"),
        static_cast<int32>(CurrentRaceType));

    const bool bOk = TransformToRace(HumanType);
    UE_LOG(LogTemp, Warning, TEXT("[Race] ApplyHumanAsDefaultMesh => %s"), bOk ? TEXT("true") : TEXT("false"));
}

FString APlayerCharacter::GetCombatInteractionName(ECombatInteractionType Type)
{
    return FString();
}

void APlayerCharacter::ProcessNextNotification()
{
}

void APlayerCharacter::AttackNotification()
{
}

void APlayerCharacter::AttackNotificationEvent(ECombatInteractionType inputNotificationEvent)
{
}

void APlayerCharacter::CalculateDamage(const FAttackParams& attackParam)
{
    Super::CalculateDamage(attackParam);
}
