// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Network/NetworkData.h"
#include "SMMOLobbyController.generated.h"

/*
 * 
 */
UCLASS()
class SMMO_API ASMMOLobbyController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// constructor & destructor
	ASMMOLobbyController();

	// PlayerController override
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// character select
	FName SetCharacterMesh(int32 Row);
	void SaveInfoAndStartGame(FText Id, FText Pw, int32 CharacterNum);

private:
	void InitMannequin();

private:
	// ui
	TSubclassOf<class UUWTitle> TitleUIClass;
	class UUWTitle* TitleUIObject;

	// character
	class ASMMOMannequinActor* LobbyMannequin;

	// game instance
	class USMMOGameInstance* GameInstance;

	// camera
	class ACameraActor* Camera;

	// login
	bool bIsLogin = false;
};
