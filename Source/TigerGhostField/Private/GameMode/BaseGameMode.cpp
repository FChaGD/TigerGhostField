#include "GameMode/BaseGameMode.h"

ABaseGameMode::ABaseGameMode()
{
	PlayerControllerClass = nullptr;
	DefaultPawnClass = nullptr;
	GameStateClass = nullptr;
	PlayerStateClass = nullptr;
}

void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	OnGameStart();
}

void ABaseGameMode::OnGameStart_Implementation()
{
}

void ABaseGameMode::OnGameEnd_Implementation()
{

}
