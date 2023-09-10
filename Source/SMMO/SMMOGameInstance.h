// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/GameInstance.h"
#include "CharacterData.h"
#include "Network/NetworkData.h"

#include "SMMOGameInstance.generated.h"

class ASMMOPlayerController;

/**
 * 
 */
UCLASS()
class SMMO_API USMMOGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	USMMOGameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;

public:
	// ============== character info ============== //
	void SetCharacterInfo(FString NewId, FString NewPw, int32 NewCharacterNum);
	
	FString GetId() { return Id; }
	FString GetPw() { return Pw; }
	int32 GetCharacterNum() { return CharacterNum; }

private:
	FString Id;
	FString Pw;
	int32 CharacterNum;

public:
	// ============== character data table ============== //

	FCharacterData* GetCharacterDataByNum(int32 Num);
	FCharacterData* GetCharacterDataByName(FName Name);
	int32 GetCharacterDataTableRowNum();

private:
	// character data table
	class UDataTable* CharacterDataTable;

	// character data table row num
	int32 CharacterDataTableRowNum;

public:
	// ============== client socket communicate ============== //

	// socket instance
	class FClientSocket* SocketInstance;

private:
	// state
	bool bCompleteLogin;
	bool bIsConnected;

	// server connection
	void StartConnection();
	void StopConnection();

public:
	// getter
	bool IsLoginCompleted() { return bCompleteLogin; }
	bool IsConnectedToServer() { return bIsConnected; }

	// setter
	void SetLoginComplete() { bCompleteLogin = true; }
	void SetPlayerControllerToSocket(ASMMOPlayerController* PlayerController);

	// sessionId
	int32 SessionId;
};
