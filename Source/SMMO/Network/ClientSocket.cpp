// Fill out your copyright notice in the Description page of Project Settings.

#include "ClientSocket.h"

#include "../SMMOGameInstance.h"
#include "../SMMOPlayerController.h"
#include "NetPacketHandler.h"
#include "NetPacketHeader.h"

#include <sstream>
#include <string>

FClientSocket::FClientSocket()
    : StopTaskCounter(0)
    , PlayerController(nullptr)
    , RecvBuffer(MAX_BUFFER)
    , bIsConnected(false)
    , bIsInitialized(false)
{
}

FClientSocket::~FClientSocket()
{
    // delete Thread
    delete Thread;
    Thread = nullptr;

    // close socket
    ::closesocket(Socket);

    // winsock 종료
    WSACleanup();
}

bool FClientSocket::StartClientSocket()
{
    // start thread to communicate with server
    if (Thread != nullptr)
    {
        UE_LOG(LogClass, Error, TEXT("Thread Already created Error"));
        return false;
    }

    Thread = FRunnableThread::Create(this, TEXT("FClientSocket"), 0, TPri_BelowNormal);
    if (Thread == nullptr)
    {
        UE_LOG(LogClass, Error, TEXT("Create Thread Error"));
        return false;
    }

    return true;
}

void FClientSocket::StopClientSocket()
{
    UE_LOG(LogClass, Log, TEXT("Stop Client Socket"));

    if (Thread == nullptr)
    {
        return;
    }

    // task counter increase to stop thread safely
    StopTaskCounter.Increment();

    // thread stop
    Thread->WaitForCompletion();
    Thread->Kill();

    // delete thread
    delete Thread;
    Thread = nullptr;

    // set StopTaskCounter zero
    StopTaskCounter.Reset();
}

bool FClientSocket::InitSocket()
{
    // winsock initialize
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        return false;
    }

    // socket set
    Socket = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (Socket == INVALID_SOCKET)
    {
        return false;
    }

    // set init
    bIsInitialized = true;

    return true;
}

bool FClientSocket::Connect()
{
    // socket address set
    SOCKADDR_IN serverAddr;
    ::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = ::htons(SERVER_PORT);
    ::inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // connect
    if (::connect(Socket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        int32 ErrCode = ::WSAGetLastError();
        UE_LOG(LogClass, Error, TEXT("Connect Errorcode = %d"), ErrCode);
        return false;
    }

    // set connect ok
    bIsConnected = true;

    return true;
}

void FClientSocket::CloseSocket()
{
    // close socket
    ::closesocket(Socket);

    // winsock 종료
    WSACleanup();
}

bool FClientSocket::SendToServer(TSharedRef<FSendBuffer> FSendBuffer)
{
    int32 SendLen = send(Socket, FSendBuffer->GetData(), FSendBuffer->GetDataSize(), 0);
    if (SendLen <= 0)
    {
        UE_LOG(LogClass, Error, TEXT("Send Packet Error"));
        return false;
    }
    return true;
}

int32 FClientSocket::recvHandler(char* Buffer, int32 Len)
{
    int32 ProcessLen = 0;

    while (true)
    {
        int32 DataSize = Len - ProcessLen;

        // packet header parsing possible?
        if (DataSize < sizeof(FNetPacketHeader))
        {
            break;
        }

        // packet header parsing
        FNetPacketHeader Header = *(reinterpret_cast<FNetPacketHeader*>(&Buffer[ProcessLen]));

        // packet enable?
        if (DataSize < Header.Size)
        {
            break;
        }

        // packet enable
        if (!FNetPacketHandler::HandleRecvPacket(PlayerController, &Buffer[ProcessLen], Header.Size))
        {
            break;
        }

        ProcessLen += Header.Size;
    }

    return ProcessLen;

    return 0;
}

void FClientSocket::SetPlayerController(ASMMOPlayerController* PController)
{
    PlayerController = PController;
}

void FClientSocket::SetGameInstance(USMMOGameInstance* GameInst)
{
    GameInstance = GameInst;
}

bool FClientSocket::Init()
{
    return true;
}

uint32 FClientSocket::Run()
{
    // Initial wait before starting
    FPlatformProcess::Sleep(0.03f);

    // Start recv loop
    while (StopTaskCounter.GetValue() == 0 && PlayerController != nullptr)
    {
        int32 RecvLen = ::recv(Socket, RecvBuffer.GetWritePos(), RecvBuffer.GetFreeSize(), 0);
        if (RecvLen == SOCKET_ERROR)
        {
            StopClientSocket();
            break;
        }

        if (!RecvBuffer.OnWrite(RecvLen))
        {
            StopClientSocket();
            break;
        }

        int32 Datasize = RecvBuffer.GetDataSize();
        int32 ProcessLen = recvHandler(RecvBuffer.GetReadPos(), Datasize);
        if (ProcessLen < 0 || Datasize < ProcessLen || RecvBuffer.OnRead(ProcessLen) == false)
        {
            StopClientSocket();
            break;
        }

        RecvBuffer.AdjustPos();
    }

    return 0;
}

void FClientSocket::Stop()
{
    StopTaskCounter.Increment();
}

void FClientSocket::Exit()
{
}
