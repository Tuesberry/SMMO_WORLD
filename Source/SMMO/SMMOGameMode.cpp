// Copyright Epic Games, Inc. All Rights Reserved.

#include "SMMOGameMode.h"
#include "SMMOPlayer.h"
#include "SMMOPlayerController.h"
#include "UObject/ConstructorHelpers.h"

ASMMOGameMode::ASMMOGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/BPPlayer"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set player controller
	PlayerControllerClass = ASMMOPlayerController::StaticClass();
}
