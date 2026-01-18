// Author: ju wonung
#include "Widget/UserWidgetBase.h"

#include "DebugHelper.h"
#include "Interface/PawnUIInterface.h"

void UUserWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (UPlayerUIComponent* PlayerUIComponent = PawnUIInterface->GetPlayerUIComponent())
		{
			BP_OnOwningPlayerUIComponentInitialized( PlayerUIComponent );
		}
	}
}

void UUserWidgetBase::InitEnemyCreatedWidget(AActor* OwningEnemyActor)
{
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(OwningEnemyActor))
	{
		UEnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent();

		checkf(EnemyUIComponent, TEXT("Failed to sxtrac an EnemyUIComponent from %s"), *OwningEnemyActor->GetActorNameOrLabel());

		BP_OnOwningEnemyUIComponentInitialized(EnemyUIComponent);
	}
}

void UUserWidgetBase::InitPlayerCreatedWidget(AActor* OwningPlayerActor)
{
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(OwningPlayerActor))
	{
		// Debug::Print(TEXT("PlayerWidget Create"));
		UPlayerUIComponent* PlayerUIComponent = PawnUIInterface->GetPlayerUIComponent();

		checkf(PlayerUIComponent, TEXT("Failed to sxtrac an PlayerUIComponent from %s"), *OwningPlayerActor->GetActorNameOrLabel());

		BP_OnOwningPlayerUIComponentInitialized(PlayerUIComponent);
	}
}