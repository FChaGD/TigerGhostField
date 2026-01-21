// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BaseEnemyCharacter.h"
#include "Component/UI/EnemyUIComponent.h"
#include "Components/WidgetComponent.h"
#include "Widget/UserWidgetBase.h"

ABaseEnemyCharacter::ABaseEnemyCharacter()
{
	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>(TEXT("EnemyUIComponent"));
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComponent"));
	HealthWidgetComponent->SetupAttachment(GetMesh());
}

UPawnUIComponent* ABaseEnemyCharacter::GetPawnUIComponent() const
{
	return EnemyUIComponent;
}

UEnemyUIComponent* ABaseEnemyCharacter::GetEnemyUIComponent()
{
	return EnemyUIComponent;
}

void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (UUserWidgetBase* HealthWidget = Cast<UUserWidgetBase>(HealthWidgetComponent->GetUserWidgetObject()))
	{
		// Chracter Create, init widget
		HealthWidget->InitEnemyCreatedWidget(this);
	}
}
