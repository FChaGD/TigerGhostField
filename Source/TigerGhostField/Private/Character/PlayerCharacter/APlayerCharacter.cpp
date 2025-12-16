#include "APlayerCharacter.h"

void APlayerCharacter::CalculateDamage(const FAttackParams& attackParam)
{
    Super::CalculateDamage(attackParam);
}

void APlayerCharacter::AttackNotificationEvent(ECombatInteractionType inputNotificationEvent)
{
    NotificationEventPop.Enqueue(inputNotificationEvent);

    AttackNotification();
}

void APlayerCharacter::AttackNotification()
{
    ProcessNextNotification();
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

