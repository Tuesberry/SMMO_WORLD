// Fill out your copyright notice in the Description page of Project Settings.

#include "SMMOLobbyGameMode.h"
#include "SMMOLobbyController.h"

ASMMOLobbyGameMode::ASMMOLobbyGameMode()
{
	// set player controller(lobby controller)
	PlayerControllerClass = ASMMOLobbyController::StaticClass();
}
