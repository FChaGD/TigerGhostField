// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ProtoTypeGameMode.h"
#include "GameFramework/PlayerController.h"
#include "Character/APlayerCharacter.h"

AProtoTypeGameMode::AProtoTypeGameMode()
{
    PlayerControllerClass = APlayerController::StaticClass();
    DefaultPawnClass = APlayerCharacter::StaticClass();
}