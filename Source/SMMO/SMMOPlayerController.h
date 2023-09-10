// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "SMMOPlayer.h"
#include "SMMOOtherPlayer.h"

#include "Containers/Queue.h"
#include "Containers/Map.h"

#include "Network/ClientSocket.h"
#include "Network/NetPacketHeader.h"
#include "Network/NetworkData.h"
#include "Network/Protocol.pb.h"

#include "SMMOPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SMMO_API ASMMOPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// constructor & destructor
	ASMMOPlayerController();
	~ASMMOPlayerController();

	// PlayerController override
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupInputComponent() override;

	// game instance
	class USMMOGameInstance* GameInstance;

public:
	// ============== user interface ============== //
	void FocusChat();
	void FocusGame();

private:
	TSubclassOf<class UUWMain> MainUIClass;
	class UUWMain* MainUIObject;

public:
	// ============== client socket communicate ============== //

	// recv
	void RecvSessionId(int32 SessionId);
	void RecvLoginResult(bool Result, FCharacterInfo& Info);
	void RecvMove(FCharacterInfo& Info);
	void RecvEnter(FCharacterInfo& Info);
	void RecvLeave(int32 SessionId);
	void RecvChat(int32 SessionId, FString StrId, FString Chat);

	// send
	FTimerHandle SendMoveHandle;
	void SendMove();
	void SendChatMsg(const FText& Text);
	
private:
	// update info
	bool bLoginComplete;
	bool bRecvEnter;
	bool bRecvMove;
	bool bRecvLeave;
	bool bRecvChat;

	// recv data
	FCharacterInfo CharacterInfo;
	TQueue<FCharacterInfo> EnterPlayers;
	TQueue<FCharacterInfo> MovePlayers;
	TQueue<int32> LeavePlayers;
	TQueue<TTuple<int32, FString, FString>> Chats;

	// player data
	FVector PrevVelocity;
	float ElapsedTimes;
	void UpdateAndSendMove();

	// players update
	void UpdateMyPlayer();
	void UpdateAllPlayer();
	void SpawnAllPlayer();
	void DeleteAllPlayer();
	void UpdateAllChat();
	void UpdateChatAnimation(int32 SessionId, FString Chat);
	
	void UpdatePlayer(FCharacterInfo& PlayerInfo);
	void SpawnPlayer(FCharacterInfo& PlayerInfo);
	void DeletePlayer(int32 SessionId);

	// players data
	TMap<int32, ASMMOOtherPlayer*> Players;

	// other player's data
	TSubclassOf<ASMMOOtherPlayer> NetPlayerToSpawn;
};
