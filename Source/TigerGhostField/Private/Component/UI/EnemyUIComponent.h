// Author: ju wonung
#pragma once

#include "CoreMinimal.h"
#include "Component/UI/PawnUIComponent.h"
#include "EnemyUIComponent.generated.h"

class UUserWidgetBase;
/**
 * 
 */
UCLASS()
class UEnemyUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()
	
public:
	// Current Widget Register to Array
	UFUNCTION(BlueprintCallable)
	void RegisterEnemyDrawnWidget(UUserWidgetBase* InWidgetToRegister);

	// All Delete Widget from Array
	UFUNCTION(BlueprintCallable)
	void RemoveEnemyDrawnWidgetsIfAny();

private:
	TArray<UUserWidgetBase*> EnemyDrawnWidgets;
};
