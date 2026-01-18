// Author: ju wonung
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetBase.generated.h"

class UPlayerUIComponent;
class UEnemyUIComponent;

/**
 * 
 */
UCLASS()
class UUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	// When Init Widget, Owner is Player, Widget Bind with PlayerUIComponent
	virtual void NativeOnInitialized() override;

	// UI Component Input, Widget Bind UI Component Function
	UFUNCTION(BlueprintImplementableEvent,meta = (DisplayName = "On Owning Hero UI Component Initialized"))
	void BP_OnOwningPlayerUIComponentInitialized(UPlayerUIComponent* OwningHeroUIComponent);
	
	UFUNCTION(BlueprintImplementableEvent,meta = (DisplayName = "On Owning Enemy UI Component Initialized"))
	void BP_OnOwningEnemyUIComponentInitialized(UEnemyUIComponent* OwningEnemyUIComponent);

public:
	UFUNCTION(BlueprintCallable)
	void InitEnemyCreatedWidget(AActor* OwningEnemyActor);
	
	UFUNCTION(BlueprintCallable)
	void InitPlayerCreatedWidget(AActor* OwningPlayerActor);
};
