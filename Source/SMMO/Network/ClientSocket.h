// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Windows/AllowWindowsPlatformTypes.h"

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <Runtime/Core/Public/HAL/Runnable.h>
#include <iostream>

#include "Protocol.pb.h"

#include "RecvBuffer.h"
#include "SendBuffer.h"

#define MAX_BUFFER 4096
#define SERVER_PORT 7777
#define SERVER_IP "127.0.0.1"

class USMMOGameInstance;
class ASMMOPlayerController;
/**
 *  Client Socket
 *
 *	서버와 통신하기 위해 사용
 *	FRunnable 클래스를 상속받아 게임 로직과 별도로 동작하도록 함
 *	싱글톤 패턴 사용
 */
class SMMO_API FClientSocket : public FRunnable
{
public:
	// constructor & destructor
	FClientSocket();
	virtual ~FClientSocket();

	// singleton instance
	static FClientSocket* GetInstance()
	{
		static FClientSocket instance;
		return &instance;
	}

	// 스레드 시작 & 중단
	bool StartClientSocket();
	void StopClientSocket();

	// 소켓 통신 설정
	bool InitSocket();
	bool Connect();
	void CloseSocket();

	// getter
	bool IsInitialized() { return bIsInitialized; }
	bool IsConnected() { return bIsConnected; }

	// send
	bool SendToServer(TSharedRef<FSendBuffer> FSendBuffer);

private:
	// recv 
	int recvHandler(char* Buffer, int32 Len);

public:
	// 플레이어 컨트롤러, 게임 인스턴스 연동
	void SetPlayerController(ASMMOPlayerController* PController);
	void SetGameInstance(USMMOGameInstance* GameInst);

	// Begin FRunnable Interface
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	virtual void Exit();
	// End FRunnable Interface

private:
	// thread to run the worker FRunnable on
	FRunnableThread* Thread = nullptr;

	// use to stop this thread safely
	FThreadSafeCounter StopTaskCounter;

	// socket communicate
	SOCKET Socket;
	FRecvBuffer RecvBuffer;

	// player Contoller
	ASMMOPlayerController* PlayerController;

	// Game Instance
	USMMOGameInstance* GameInstance;

	// state
	bool bIsInitialized;
	bool bIsConnected;
};
