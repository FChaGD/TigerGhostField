// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABaseCharacter.h"
#include "BaseEnemyCharacter.generated.h"

class UWidgetComponent;
class UEnemyUIComponent;
/**
 * 
 */
UCLASS()
class ABaseEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	ABaseEnemyCharacter();
	
	//~Begin IPawnUIInterface Interface
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UEnemyUIComponent* GetEnemyUIComponent() override;
	//~End IPawnUIInterface Interface
	
protected:
	virtual void BeginPlay() override;
	
protected:
	UPlayerUIComponent* PlayerUIComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UI", meta = (AllowPrivateAccess = "true"))
	UEnemyUIComponent* EnemyUIComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UI")
	UWidgetComponent* HealthWidgetComponent;
	
};
