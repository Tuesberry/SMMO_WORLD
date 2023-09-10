// Fill out your copyright notice in the Description page of Project Settings.

#include "SMMOGameInstance.h"

#include <Engine/DataTable.h>

#include "SMMOPlayerController.h"
#include "Network/ClientSocket.h"
#include "Network/NetPacketHandler.h"

USMMOGameInstance::USMMOGameInstance()
	: SocketInstance(nullptr)
	, bCompleteLogin(false)
	, bIsConnected(false)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DTChar(TEXT("/Game/Blueprint/CharacterDataTable"));
	if (DTChar.Succeeded())
	{
		CharacterDataTable = DTChar.Object;
	}
}

void USMMOGameInstance::Init()
{
	Super::Init();

	// get character data table num
	CharacterDataTableRowNum = CharacterDataTable->GetRowMap().Num();

	// start connection
	StartConnection();
}

void USMMOGameInstance::Shutdown()
{
	// stop connection
	StopConnection();

	Super::Shutdown();
}

void USMMOGameInstance::SetCharacterInfo(FString NewId, FString NewPw, int32 NewCharacterNum)
{
	Id = NewId;
	Pw = NewPw;
	CharacterNum = NewCharacterNum;
}

FCharacterData* USMMOGameInstance::GetCharacterDataByNum(int32 Num)
{
	return CharacterDataTable->FindRow<FCharacterData>(*FString::FromInt(Num), TEXT(""));
}

FCharacterData* USMMOGameInstance::GetCharacterDataByName(FName Name)
{
	return CharacterDataTable->FindRow<FCharacterData>(Name, TEXT(""));
}

int32 USMMOGameInstance::GetCharacterDataTableRowNum()
{
	return CharacterDataTableRowNum;
}

void USMMOGameInstance::StartConnection()
{
	if (SocketInstance == nullptr)
	{
		SocketInstance = FClientSocket::GetInstance();
	}

	if (!SocketInstance->InitSocket())
	{
		UE_LOG(LogClass, Error, TEXT("Init Socket Error"));
	}

	bIsConnected = SocketInstance->Connect();
	if (!bIsConnected)
	{
		UE_LOG(LogClass, Error, TEXT("Connect to Server Error"));
	}
	else
	{
		UE_LOG(LogClass, Log, TEXT("Connect to Server Complete"));
		SocketInstance->SetGameInstance(this);
	}
}

void USMMOGameInstance::StopConnection()
{
	if (SocketInstance == nullptr)
	{
		return;
	}

	// logout
	if (bCompleteLogin)
	{
		//NetPacketHandler::SendLogout(SocketInstance, SessionId);
	}

	// stop client socket thread
	SocketInstance->StopClientSocket();
	SocketInstance = nullptr;
}

void USMMOGameInstance::SetPlayerControllerToSocket(ASMMOPlayerController* PlayerController)
{
	SocketInstance->SetPlayerController(PlayerController);
}
