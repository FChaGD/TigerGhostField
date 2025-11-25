#include "APlayerCharacter.h"

void APlayerCharacter::AttackNotificationEvent(ECombatInteractionType inputNotificationEvent)
{
    NotificationEventPop.Enqueue(inputNotificationEvent);

    AttackNotification();
}

void APlayerCharacter::CalculateDamage(const FAttackParams& attackParam)
{
}

void APlayerCharacter::AttackNotification()
{
    ProcessNextNotification();
}

void APlayerCharacter::ProcessNextNotification()
{
    ECombatInteractionType OutValue;

    if (!NotificationEventPop.Dequeue(OutValue))
    {
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Incoming Attack Type: %d"), (int32)OutValue);

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        FTimerDelegate::CreateLambda([this]()
            {
                ProcessNextNotification();
            }),
        0.7f, false );
}
