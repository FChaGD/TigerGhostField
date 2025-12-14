#include "APlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/DA_InputDataAsset.h"
#include "Component/Input/PlayerEnhancedInputComponent.h"

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

void APlayerCharacter::AttackNotificationEvent(ECombatInteractionType inputNotificationEvent)
{
    NotificationEventPop.Enqueue(inputNotificationEvent);

    AttackNotification();
}

void APlayerCharacter::CalculateDamage(const FAttackParams& attackParam)
{
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

    UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
    UPlayerEnhancedInputComponent* PlayerInput = Cast<UPlayerEnhancedInputComponent>(PlayerInputComponent);

    SubSystem->AddMappingContext(InputDataAsset->DefaultMappingContext, 0);

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

    //현재 컨트롤러로 캐릭터 회전 불가능
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;

    // 카메라 봉 생성자
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());	// 루트컴포넌트에 자식으로 붙이기
    CameraBoom->TargetArmLength = 200.f;
    CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);	// 자식의 offset 지정(카메라 오프셋)
    CameraBoom->bUsePawnControlRotation = true;		// 컨트롤러에 의해 회전이 되는지 설정

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;	// 이미 카메라봉이 회전하고 있으므로 회전 X

    GetCharacterMovement()->bOrientRotationToMovement = true;	// 캐릭터 wasd 이동시 회전여부
    GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);	// 캐릭터 회전속도
    GetCharacterMovement()->MaxWalkSpeed = 400.f;	// 최대 걷기 속도
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;	// 이동 정지시 감속

}

FString APlayerCharacter::GetCombatInteractionName(ECombatInteractionType Type)
{
    const UEnum* EnumPtr = StaticEnum<ECombatInteractionType>();
    if (!EnumPtr)
        return TEXT("Invalid");

    return EnumPtr->GetNameStringByValue(static_cast<int64>(Type));
}

void APlayerCharacter::ProcessNextNotification()
{
    ECombatInteractionType OutValue;

    if (!NotificationEventPop.Dequeue(OutValue))
    {
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Incoming Attack Type: %d"), (int32)OutValue);

    const int32 WidgetIndex = static_cast<int32>(OutValue);
    if (!HeadWidgets.IsValidIndex(WidgetIndex))
    {
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            TimerHandle,
            FTimerDelegate::CreateLambda([this]() { ProcessNextNotification(); }),
            0.5f, false
        );
        return;
    }

    UWidgetComponent* WidgetComp = HeadWidgets[WidgetIndex];
    if (!WidgetComp)
    {
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            TimerHandle,
            FTimerDelegate::CreateLambda([this]() { ProcessNextNotification(); }),
            0.5f, false
        );
        return;
    }

    for (UWidgetComponent* Comp : HeadWidgets)
    {
        if (Comp)
            Comp->SetVisibility(false);
    }

    WidgetComp->SetVisibility(true);

    FTimerHandle HideTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        HideTimerHandle,
        FTimerDelegate::CreateLambda([WidgetComp]()
            {
                if (WidgetComp)
                    WidgetComp->SetVisibility(false);
            }),
        1.0f, false
    );

    FTimerHandle NextTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        NextTimerHandle,
        FTimerDelegate::CreateLambda([this]() { ProcessNextNotification(); }),
        1.0f, false
    );
}

