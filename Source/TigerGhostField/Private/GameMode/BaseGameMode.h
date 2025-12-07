#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"

UCLASS(Abstract)
class ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABaseGameMode();

protected:
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rules")
	bool bAllowRespawn = true;

protected:

	UFUNCTION(BlueprintNativeEvent, Category = "Game Flow")
	void OnGameStart();
	virtual void OnGameStart_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Game Flow")
	void OnGameEnd();
	virtual void OnGameEnd_Implementation();
};

