#include "APlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/DA_InputDataAsset.h"
#include "Component/Input/PlayerEnhancedInputComponent.h"
#include "PlayerData/UDA_RaceForm.h"

void APlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
    const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
    const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

    if (MovementVector.Y != 0.f)
    {
        const FVector ForwardDirection =
            MovementRotation.RotateVector(FVector::ForwardVector);
        AddMovementInput(ForwardDirection, MovementVector.Y);
    }

    if (MovementVector.X != 0.f)
    {
        const FVector RightDirection =
            MovementRotation.RotateVector(FVector::RightVector);
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
}

void APlayerCharacter::Input_HeavyAttack(const FInputActionValue& InputActionValue)
{
    UE_LOG(LogTemp, Warning, TEXT("Heavy Attack"));
}

bool APlayerCharacter::TransformToRace(ERaceType TargetRaceType)
{
    return false;
}

void APlayerCharacter::ApplyRaceForm(const UDA_RaceForm* Form)
{
    // TODO: Validate Form pointer before applying any changes.
    // TODO: Cache previous race-related parameters if rollback support is required.

    // TODO: Apply skeletal mesh defined by the race form.
    // TODO: Apply animation instance class associated with the race form.

    // TODO: Update capsule size (radius / half-height) based on race form data.
    // NOTE: Capsule size changes may affect collision and should be applied before movement updates.

    // TODO: Update mesh relative transform to match the new capsule configuration.

    // TODO: Apply movement parameters (e.g., MaxWalkSpeed, JumpZVelocity).
    // NOTE: Movement parameters must be applied after capsule changes to avoid physics inconsistencies.

    // TODO: Reset or synchronize animation state if required by the new race form.
    // NOTE: Ensure no invalid animation state is carried over from the previous race.

    // TODO: Update CurrentRaceType after successful application of the form.

    // TODO: Trigger race change related events (UI refresh, effects, sound).
    // NOTE: Do not trigger gameplay logic here; this function should remain a pure form-application step.

    // FIXME: Verify edge cases when transforming during airborne or hit-reaction states.
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    ULocalPlayer* LocalPlayer =
        GetController<APlayerController>()->GetLocalPlayer();

    UEnhancedInputLocalPlayerSubsystem* SubSystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

    UPlayerEnhancedInputComponent* PlayerInput =
        Cast<UPlayerEnhancedInputComponent>(PlayerInputComponent);

    SubSystem->AddMappingContext(
        InputDataAsset->DefaultMappingContext, 0);

    PlayerInput->BindInputAction(
        InputDataAsset->MoveInputAction,
        ETriggerEvent::Triggered,
        this,
        &ThisClass::Input_Move);

    PlayerInput->BindInputAction(
        InputDataAsset->LookInputAction,
        ETriggerEvent::Triggered,
        this,
        &ThisClass::Input_Look);

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

    PlayerInput->BindInputAction(
        InputDataAsset->LightAttackInputAction,
        ETriggerEvent::Started,
        this,
        &ThisClass::Input_LightAttack);

    PlayerInput->BindInputAction(
        InputDataAsset->HeavyAttackInputAction,
        ETriggerEvent::Started,
        this,
        &ThisClass::Input_HeavyAttack);
}

void APlayerCharacter::CalculateDamage(const FAttackParams& attackParam)
{
    Super::CalculateDamage(attackParam);
}

void APlayerCharacter::AttackNotificationEvent(
    ECombatInteractionType inputNotificationEvent)
{
    NotificationEventPop.Enqueue(inputNotificationEvent);
    AttackNotification();
}

void APlayerCharacter::AttackNotification()
{
    ProcessNextNotification();
}

void APlayerCharacter::Attack()
{
}

APlayerCharacter::APlayerCharacter()
{
    HeadWidgets.SetNum(HeadWidgetCount);

    for (int32 i = 0; i < HeadWidgetCount; ++i)
    {
        const ECombatInteractionType Type =
            static_cast<ECombatInteractionType>(i);

        const FString TypeName = GetCombatInteractionName(Type);
        const FName CompName =
            *FString::Printf(TEXT("NotifyWidget_%s"), *TypeName);

        UWidgetComponent* WidgetComp =
            CreateDefaultSubobject<UWidgetComponent>(CompName);

        HeadWidgets[i] = WidgetComp;

        WidgetComp->SetupAttachment(GetRootComponent());
        WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
        WidgetComp->SetDrawSize(FVector2D(320.f, 320.f));
        WidgetComp->SetPivot(FVector2D(0.5f, 0.5f));
        WidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 110.f));
        WidgetComp->SetVisibility(false);
    }

    // Disable controller rotation inheritance
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;

    // Camera boom setup
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 200.f;
    CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera =
        CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(
        CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // Character movement setup
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate =
        FRotator(0.f, 500.f, 0.f);
    GetCharacterMovement()->MaxWalkSpeed = 400.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

FString APlayerCharacter::GetCombatInteractionName(
    ECombatInteractionType Type)
{
    const UEnum* EnumPtr = StaticEnum<ECombatInteractionType>();
    if (!EnumPtr)
    {
        return TEXT("Invalid");
    }

    return EnumPtr->GetNameStringByValue(
        static_cast<int64>(Type));
}

void APlayerCharacter::ProcessNextNotification()
{
    ECombatInteractionType OutValue;

    if (!NotificationEventPop.Dequeue(OutValue))
    {
        return;
    }

    UE_LOG(LogTemp, Warning,
        TEXT("Incoming Attack Type: %d"),
        static_cast<int32>(OutValue));

    const int32 WidgetIndex =
        static_cast<int32>(OutValue);

    if (!HeadWidgets.IsValidIndex(WidgetIndex))
    {
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            TimerHandle,
            FTimerDelegate::CreateLambda(
                [this]() { ProcessNextNotification(); }),
            0.5f, false);
        return;
    }

    UWidgetComponent* WidgetComp =
        HeadWidgets[WidgetIndex];

    if (!WidgetComp)
    {
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            TimerHandle,
            FTimerDelegate::CreateLambda(
                [this]() { ProcessNextNotification(); }),
            0.5f, false);
        return;
    }

    for (UWidgetComponent* Comp : HeadWidgets)
    {
        if (Comp)
        {
            Comp->SetVisibility(false);
        }
    }

    WidgetComp->SetVisibility(true);

    FTimerHandle HideTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        HideTimerHandle,
        FTimerDelegate::CreateLambda(
            [WidgetComp]()
            {
                if (WidgetComp)
                {
                    WidgetComp->SetVisibility(false);
                }
            }),
        1.0f, false);

    FTimerHandle NextTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        NextTimerHandle,
        FTimerDelegate::CreateLambda(
            [this]() { ProcessNextNotification(); }),
        1.0f, false);
}
