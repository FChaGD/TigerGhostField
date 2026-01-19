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
#include "Component/Factory/PotionFactory.h"
#include "Component/UI/PlayerUIComponent.h"
#include "Character/Attack/AttackBase.h"
#include "Character/Potion/PotionBase.h"
// English comment: Needed for zone actor logging in BeginPlay.
#include "TypeZone/ZoneVolumeActor.h"
#include "Widget/UserWidgetBase.h"

#include "DebugHelper.h"

APlayerCharacter::APlayerCharacter()
{
    // Current Controller dont rotate Character
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;

    // Camera SpringArm Create
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());	// 루트컴포넌트에 자식으로 붙이기
    CameraBoom->TargetArmLength = 200.f;
    CameraBoom->SocketOffset = FVector(0.f,55.f,65.f);	// 자식의 offset 지정(카메라 오프셋)
    CameraBoom->bUsePawnControlRotation = true;		// Camera Controller Rotation

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;	// 이미 카메라봉이 회전하고 있으므로 회전 X
    
    GetCharacterMovement()->bOrientRotationToMovement = true;	// Character Move = Rotation 
    GetCharacterMovement()->RotationRate = FRotator(0.f,500.f,0.f);	// Character Rotation Speed
    GetCharacterMovement()->MaxWalkSpeed = 600.f;	// Max Walk Speed
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;	// Brake speed
    
    PlayerUIComponent = CreateDefaultSubobject<UPlayerUIComponent>(TEXT("PlayerUIComponent"));
    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComponent"));
    HealthWidgetComponent->SetupAttachment(GetMesh());
}

UPawnUIComponent* APlayerCharacter::GetPawnUIComponent() const
{
    return PlayerUIComponent;
}

UPlayerUIComponent* APlayerCharacter::GetPlayerUIComponent()
{
    return PlayerUIComponent;
}

void APlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
    if (IsDead()) return;
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
    GetCharacterMovement()->MaxWalkSpeed = 800.f;
}

void APlayerCharacter::Input_RunComplete(const FInputActionValue& InputActionValue)
{
    GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void APlayerCharacter::Input_LightAttack(const FInputActionValue& InputActionValue)
{
    if (IsDead()) return;
    UE_LOG(LogTemp, Warning, TEXT("Light Attack"));

    if (!CurrentLightAttack)
    {
        UE_LOG(LogTemp, Warning, TEXT("CurrentLightAttack is null"));
        return;
    }

    CurrentLightAttack->Execute();
}

void APlayerCharacter::Input_HeavyAttack(const FInputActionValue& InputActionValue)
{
    if (IsDead()) return;
    UE_LOG(LogTemp, Warning, TEXT("Heavy Attack"));

    if (!CurrentHeavyAttack)
    {
        UE_LOG(LogTemp, Warning, TEXT("CurrentHeavyAttack is null"));
        return;
    }

    CurrentHeavyAttack->Execute();
}

void APlayerCharacter::Input_RaceTransform(const FInputActionValue& InputActionValue)
{
    if (IsDead()) return;
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

void APlayerCharacter::Input_Potion(const FInputActionValue& InputActionValue)
{
    if (IsDead()) return;
    UE_LOG(LogTemp, Warning, TEXT("Input Potion"));

    if (CurrentPotion)
    {
        CurrentPotion->Execute();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("NO Potion"));
    }
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

    if (InputDataAsset->PotionInputAction)
    {
        PlayerInput->BindInputAction(
            InputDataAsset->PotionInputAction,
            ETriggerEvent::Started,
            this,
            &ThisClass::Input_Potion);
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

    // English comment: Attack factory setup (BaseAttack style).
    if (!AttackFactoryClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("AttackFactoryClass is null"));
    }
    AttackFactory = AttackFactoryClass ? NewObject<UAttackFactory>(this, AttackFactoryClass) : nullptr;
    if (!AttackFactory)
    {
        UE_LOG(LogTemp, Warning, TEXT("AttackFactory create failed"));
    }
    else
    {
        CurrentLightAttack = AttackFactory->CreateLightAttack(0);
        if (CurrentLightAttack)
        {
            CurrentLightAttack->Initialize(this);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("CreateLightAttack returned null"));
        }

        CurrentHeavyAttack = AttackFactory->CreateHeavyAttack(0);
        if (CurrentHeavyAttack)
        {
            CurrentHeavyAttack->Initialize(this);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("CreateHeavyAttack returned null"));
        }
    }

    // English comment: Potion factory setup (BaseAttack style).
    if (!PotionFactoryClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("PotionFactoryClass is null"));
    }
    PotionFactory = PotionFactoryClass ? NewObject<UPotionFactory>(this, PotionFactoryClass) : nullptr;
    if (!PotionFactory)
    {
        UE_LOG(LogTemp, Warning, TEXT("PotionFactory create failed"));
    }
    else
    {
        CurrentPotion = PotionFactory->CreatePotion(0);
        if (CurrentPotion)
        {
            CurrentPotion->Initialize(this, 0, EPotionState::Ready);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("CreatePotion returned null"));
        }
    }
    // just Widget Component, need Cast
    if (UUserWidgetBase* HealthWidget = Cast<UUserWidgetBase>(HealthWidgetComponent->GetUserWidgetObject()))
    {
        // Chracter Create, init widget
        HealthWidget->InitPlayerCreatedWidget(this);
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
