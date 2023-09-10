#include "NetPacketHandler.h"
#include "NetPacketHeader.h"
#include "NetworkData.h"
#include "Protocol.pb.h"
#include <chrono>

#include "../SMMOPlayerController.h"
#include "../SMMOGameInstance.h"

bool FNetPacketHandler::HandleRecvPacket(ASMMOPlayerController* PlayerController, char* Buffer, int32 Len)
{
	FNetPacketHeader Header = *(reinterpret_cast<FNetPacketHeader*>(Buffer));

	switch (Header.Id)
	{
	case EPROTOID::S2C_LOGIN_RESULT:
		return HandleLogin(PlayerController, Buffer, Len);
		break;
	case EPROTOID::S2C_LOGOUT_RESULT:
		return HandleLogout(PlayerController, Buffer, Len);
		break;
	case EPROTOID::S2C_MOVE:
		return HandleMove(PlayerController, Buffer, Len);
		break;
	case EPROTOID::S2C_ENTER:
		UE_LOG(LogClass, Log, TEXT("S2C_ENTER"));
		return HandleEnter(PlayerController, Buffer, Len);
		break;
	case EPROTOID::S2C_LEAVE:
		UE_LOG(LogClass, Log, TEXT("S2C_LEAVE"));
		return HandleLeave(PlayerController, Buffer, Len);
		break;
	case EPROTOID::S2C_MOVE_RESULT:
		return HandleMoveResult(PlayerController, Buffer, Len);
		break;
	case EPROTOID::S2C_CHAT:
		return HandleChat(PlayerController, Buffer, Len);
		break;
	default:
		break;
	}

	return false;
}

bool FNetPacketHandler::HandleLogin(ASMMOPlayerController* PlayerController, char* Buffer, int32 Len)
{
	if (!IsValid(PlayerController))
	{
		return false;
	}

	FNetPacketHeader Header = *(reinterpret_cast<FNetPacketHeader*>(Buffer));

	// check packet size
	if (Header.Size > Len)
	{
		return false;
	}

	Protocol::S2C_LOGIN_RESULT Pkt;
	Pkt.ParseFromArray(Buffer + sizeof(FNetPacketHeader), Len - sizeof(FNetPacketHeader));

	bool bResult = Pkt.result();
	if (!bResult)
	{
		UE_LOG(LogClass, Error, TEXT("HandleLogin, Login Error"));
		return false;
	}

	FCharacterInfo PlayerInfo;
	PlayerInfo.SessionId = Pkt.session_id();
	PlayerInfo.PlayerType = Pkt.player_type();

	PlayerController->RecvSessionId(Pkt.session_id());

	Protocol::PLAYER_POS_INFO posInfo = Pkt.pos_info();
	PlayerInfo.X = posInfo.x();
	PlayerInfo.Y = posInfo.y();
	PlayerInfo.Z = posInfo.z();
	PlayerInfo.Yaw = posInfo.yaw();
	PlayerInfo.Pitch = posInfo.pitch();
	PlayerInfo.Roll = posInfo.roll();
	PlayerInfo.VX = posInfo.vx();
	PlayerInfo.VY = posInfo.vy();
	PlayerInfo.VZ = posInfo.vz();

	PlayerController->RecvLoginResult(bResult, PlayerInfo);

	return true;
}

bool FNetPacketHandler::HandleLogout(ASMMOPlayerController* PlayerController, char* Buffer, int32 Len)
{
	return true;
}

bool FNetPacketHandler::HandleEnter(ASMMOPlayerController* PlayerController, char* Buffer, int32 Len)
{
	if (!IsValid(PlayerController))
	{
		return false;
	}

	FNetPacketHeader Header = *(reinterpret_cast<FNetPacketHeader*>(Buffer));

	// check packet size
	if (Header.Size > Len)
	{
		UE_LOG(LogClass, Error, TEXT("HandleEnter, Packet Size Error"));
		return false;
	}

	Protocol::S2C_ENTER Pkt;
	Pkt.ParseFromArray(Buffer + sizeof(FNetPacketHeader), Len - sizeof(FNetPacketHeader));

	// check id validation
	if (Pkt.session_id() != PlayerController->GameInstance->SessionId)
	{
		UE_LOG(LogClass, Error, TEXT("HandleEnter, SessionId Error"));
		return false;
	}
	if (Pkt.target_id() == PlayerController->GameInstance->SessionId)
	{
		UE_LOG(LogClass, Error, TEXT("HandleEnter, TargetId Error"));
		return false;
	}

	// get Player position info
	FCharacterInfo PlayerInfo;
	PlayerInfo.SessionId = Pkt.target_id();
	PlayerInfo.PlayerType = Pkt.player_type();
	PlayerInfo.PlayerId = FString(Pkt.target_str_id().c_str());
	PlayerInfo.TimeStamp = Pkt.time_stamp();

	Protocol::PLAYER_POS_INFO posInfo = Pkt.pos_info();

	PlayerInfo.X = posInfo.x();
	PlayerInfo.Y = posInfo.y();
	PlayerInfo.Z = posInfo.z();
	PlayerInfo.Yaw = posInfo.yaw();
	PlayerInfo.Pitch = posInfo.pitch();
	PlayerInfo.Roll = posInfo.roll();
	PlayerInfo.VX = posInfo.vx();
	PlayerInfo.VY = posInfo.vy();
	PlayerInfo.VZ = posInfo.vz();

	PlayerController->RecvEnter(PlayerInfo);

	return true;
}

bool FNetPacketHandler::HandleMove(ASMMOPlayerController* PlayerController, char* Buffer, int32 Len)
{
	if (!IsValid(PlayerController))
	{
		return false;
	}

	FNetPacketHeader Header = *(reinterpret_cast<FNetPacketHeader*>(Buffer));

	// check packet size
	if (Header.Size > Len)
	{
		UE_LOG(LogClass, Error, TEXT("HandleMove, Packet Size Error"));
		return false;
	}

	Protocol::S2C_MOVE Pkt;
	Pkt.ParseFromArray(Buffer + sizeof(FNetPacketHeader), Len - sizeof(FNetPacketHeader));

	// check id validation
	if (Pkt.session_id() != PlayerController->GameInstance->SessionId)
	{
		UE_LOG(LogClass, Error, TEXT("HandleMove, SessionId Error"));
		return false;
	}
	if (Pkt.target_id() == PlayerController->GameInstance->SessionId)
	{
		UE_LOG(LogClass, Error, TEXT("HandleMove, TargetId Error"));
		return false;
	}

	FCharacterInfo PlayerInfo;
	PlayerInfo.SessionId = Pkt.target_id();
	PlayerInfo.TimeStamp = Pkt.time_stamp();

	Protocol::PLAYER_POS_INFO PosInfo = Pkt.pos_info();

	PlayerInfo.X = PosInfo.x();
	PlayerInfo.Y = PosInfo.y();
	PlayerInfo.Z = PosInfo.z();
	PlayerInfo.Yaw = PosInfo.yaw();
	PlayerInfo.Pitch = PosInfo.pitch();
	PlayerInfo.Roll = PosInfo.roll();
	PlayerInfo.VX = PosInfo.vx();
	PlayerInfo.VY = PosInfo.vy();
	PlayerInfo.VZ = PosInfo.vz();

	PlayerController->RecvMove(PlayerInfo);

	return true;
}

bool FNetPacketHandler::HandleLeave(ASMMOPlayerController* PlayerController, char* Buffer, int32 Len)
{
	if (!IsValid(PlayerController))
	{
		return false;
	}

	FNetPacketHeader Header = *(reinterpret_cast<FNetPacketHeader*>(Buffer));

	// check packet size
	if (Header.Size > Len)
	{
		UE_LOG(LogClass, Error, TEXT("HandleLeave, Packet Size Error"));
		return false;
	}

	Protocol::S2C_LEAVE Pkt;
	Pkt.ParseFromArray(Buffer + sizeof(FNetPacketHeader), Len - sizeof(FNetPacketHeader));

	// check id validation
	if (Pkt.session_id() != PlayerController->GameInstance->SessionId)
	{
		UE_LOG(LogClass, Error, TEXT("HandleLeave, SessionId Error"));
		return false;
	}
	if (Pkt.target_id() == PlayerController->GameInstance->SessionId)
	{
		UE_LOG(LogClass, Error, TEXT("HandleLeave, TargetId Error"));
		return false;
	}

	PlayerController->RecvLeave(Pkt.target_id());

	return true;
}

bool FNetPacketHandler::HandleMoveResult(ASMMOPlayerController* PlayerController, char* Buffer, int32 Len)
{
	if (!IsValid(PlayerController))
	{
		return false;
	}

	FNetPacketHeader Header = *(reinterpret_cast<FNetPacketHeader*>(Buffer));

	// check packet size
	if (Header.Size > Len)
	{
		return false;
	}

	return true;
}

bool FNetPacketHandler::HandleChat(ASMMOPlayerController* PlayerController, char* Buffer, int32 Len)
{
	if (!IsValid(PlayerController))
	{
		return false;
	}

	FNetPacketHeader Header = *(reinterpret_cast<FNetPacketHeader*>(Buffer));

	// check packet size
	if (Header.Size > Len)
	{
		UE_LOG(LogClass, Error, TEXT("HandleChat, Packet Size Error"));
		return false;
	}

	Protocol::S2C_CHAT Pkt;
	Pkt.ParseFromArray(Buffer + sizeof(FNetPacketHeader), Len - sizeof(FNetPacketHeader));

	// check id validation
	if (Pkt.session_id() != PlayerController->GameInstance->SessionId)
	{
		UE_LOG(LogClass, Error, TEXT("HandleChat, SessionId Error"));
		return false;
	}

	PlayerController->RecvChat(Pkt.target_id(), FString(Pkt.target_str_id().c_str()), FString(Pkt.chat().c_str()));

	return true;
}

bool FNetPacketHandler::SendLogin(FClientSocket* Socket, FString Id, FString Pw, int32 PlayerType)
{
	Protocol::C2S_LOGIN Pkt;
	
	Pkt.set_player_id(std::string(TCHAR_TO_UTF8(*Id)));
	Pkt.set_player_pw(std::string(TCHAR_TO_UTF8(*Pw)));
	Pkt.set_player_type(PlayerType);

	Pkt.set_login_time(0);

	UE_LOG(LogClass, Log, TEXT("Login Send"));

	TSharedRef<FSendBuffer> FSendBuffer = FNetPacketHandler::MakeSendBuffer(Pkt, EPROTOID::C2S_LOGIN);
	return Socket->SendToServer(FSendBuffer);
}

bool FNetPacketHandler::SendMove(FClientSocket* Socket, ASMMOPlayerController* PlayerController)
{
	if (PlayerController == nullptr || Socket == nullptr)
	{
		return false;
	}

	FVector Location = PlayerController->GetPawn()->GetActorLocation();
	FRotator Rotation = PlayerController->GetPawn()->GetActorRotation();
	FVector Velocity = PlayerController->GetPawn()->GetVelocity();

	Protocol::C2S_MOVE Pkt;
	Protocol::PLAYER_POS_INFO* posInfo = Pkt.mutable_pos_info();

	Pkt.set_session_id(PlayerController->GameInstance->SessionId);
	Pkt.set_time_stamp(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
	Pkt.set_request_result(false);

	posInfo->set_x(Location.X);
	posInfo->set_y(Location.Y);
	posInfo->set_z(Location.Z);
	
	posInfo->set_yaw(Rotation.Yaw);
	posInfo->set_pitch(Rotation.Pitch);
	posInfo->set_roll(Rotation.Roll);
	
	posInfo->set_vx(Velocity.X);
	posInfo->set_vy(Velocity.Y);
	posInfo->set_vz(Velocity.Z);

	TSharedRef<FSendBuffer> FSendBuffer = FNetPacketHandler::MakeSendBuffer(Pkt, EPROTOID::C2S_MOVE);
	return Socket->SendToServer(FSendBuffer);
}

bool FNetPacketHandler::SendChat(FClientSocket* Socket, ASMMOPlayerController* PlayerController, const FText& Chat)
{
	if (PlayerController == nullptr || Socket == nullptr)
	{
		return false;
	}

	Protocol::C2S_CHAT Pkt;
	Pkt.set_session_id(PlayerController->GameInstance->SessionId);
	Pkt.set_chat(std::string(TCHAR_TO_UTF8(*Chat.ToString())));

	TSharedRef<FSendBuffer> FSendBuffer = FNetPacketHandler::MakeSendBuffer(Pkt, EPROTOID::C2S_CHAT);
	return Socket->SendToServer(FSendBuffer);
}

bool FNetPacketHandler::SendLogout(FClientSocket* Socket, int32 SessionId)
{
	if (Socket == nullptr)
	{
		return false;
	}

	Protocol::C2S_LOGOUT Pkt;
	Pkt.set_session_id(SessionId);

	TSharedRef<FSendBuffer> FSendBuffer = FNetPacketHandler::MakeSendBuffer(Pkt, EPROTOID::C2S_LOGOUT);
	return Socket->SendToServer(FSendBuffer);
}
