#pragma once

#include "CoreMinimal.h"

#include "SendBuffer.h"
#include "ClientSocket.h"

class ASMMOPlayerController;

class SMMO_API FNetPacketHandler
{
	// ============== recv packet handler ============== //
public:
	static bool HandleRecvPacket(ASMMOPlayerController* PlayerController, char* Buffer, int32 Len);
private:
	static bool HandleLogin(ASMMOPlayerController* PlayerController, char* Buffer, int32 Len);
	static bool HandleLogout(ASMMOPlayerController* PlayerController, char* Buffer, int32 Len);
	static bool HandleEnter(ASMMOPlayerController* PlayerController, char* Buffer, int32 Len);
	static bool HandleMove(ASMMOPlayerController* PlayerController, char* Buffer, int32 Len);
	static bool HandleLeave(ASMMOPlayerController* PlayerController, char* Buffer, int32 Len);
	static bool HandleMoveResult(ASMMOPlayerController* PlayerController, char* Buffer, int32 Len);
	static bool HandleChat(ASMMOPlayerController* PlayerController, char* Buffer, int32 Len);

	// ============== send packet handler ============== //
private:
	template<typename T>
	static TSharedRef<FSendBuffer> MakeSendBuffer(T& Pkt, unsigned short PktId);
public:
	static bool SendLogin(FClientSocket* Socket, FString Id, FString Pw, int32 PlayerType);
	static bool SendMove(FClientSocket* Socket, ASMMOPlayerController* PlayerController);
	static bool SendChat(FClientSocket* Socket, ASMMOPlayerController* PlayerController, const FText& Chat);
	static bool SendLogout(FClientSocket* Socket, int32 SessionId);
};

template<typename T>
inline TSharedRef<FSendBuffer> FNetPacketHandler::MakeSendBuffer(T& Pkt, unsigned short PktId)
{
	int32 DataSize = static_cast<int32>(Pkt.ByteSizeLong());
	int32 PacketSize = DataSize + sizeof(FNetPacketHeader);

	TSharedRef<FSendBuffer> SendBuffer = MakeShared<FSendBuffer>(PacketSize);
	FNetPacketHeader* Header = reinterpret_cast<FNetPacketHeader*>(SendBuffer->GetData());
	Header->Id = PktId;
	Header->Size = PacketSize;
	Pkt.SerializeToArray(&Header[1], DataSize);
	SendBuffer->OnWrite(PacketSize);

	return SendBuffer;
}
