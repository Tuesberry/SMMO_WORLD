// Fill out your copyright notice in the Description page of Project Settings.

#include "SMMOPlayerController.h"
#include "SMMOGameInstance.h"
#include "Network/NetPacketHandler.h"
#include "UWMain.h"
#include "SMMOAnimInstance.h"
#include <chrono>

ASMMOPlayerController::ASMMOPlayerController()
	: bLoginComplete(false)
	, bRecvEnter(false)
	, bRecvMove(false)
	, bRecvLeave(false)
	, bRecvChat(false)
	, PrevVelocity()
	, ElapsedTimes(0)
{
	// find class
	static ConstructorHelpers::FClassFinder<APawn> NetPlayerBP(TEXT("/Game/Blueprint/BPOtherPlayer"));
	if (NetPlayerBP.Class != NULL)
	{
		NetPlayerToSpawn = NetPlayerBP.Class;
	}

	static ConstructorHelpers::FClassFinder<UUWMain> UWMain(TEXT("/Game/Blueprint/UWMain"));
	if (UWMain.Succeeded())
	{
		MainUIClass = UWMain.Class;
	}

	PrimaryActorTick.bCanEverTick = true;
}

ASMMOPlayerController::~ASMMOPlayerController()
{

}

void ASMMOPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	ElapsedTimes += DeltaSeconds;
	UpdateAndSendMove();

	if (!IsValid(GameInstance))
	{
		return;
	}
	if (bLoginComplete)
	{
		UpdateMyPlayer();
		bLoginComplete = false;
	}
	if (bRecvEnter || !EnterPlayers.IsEmpty())
	{
		SpawnAllPlayer();
		bRecvEnter = false;
	}
	if (bRecvMove || !MovePlayers.IsEmpty())
	{
		UpdateAllPlayer();
		bRecvMove = false;
	}
	if (bRecvLeave || !LeavePlayers.IsEmpty())
	{
		DeleteAllPlayer();
		bRecvLeave = false;
	}
	if (bRecvChat || !Chats.IsEmpty())
	{
		UpdateAllChat();
		bRecvChat = false;
	}
}

void ASMMOPlayerController::BeginPlay()
{
	// set input
	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());

	// set player controller to client socket
	GameInstance = Cast<USMMOGameInstance>(GetGameInstance());
	if (GameInstance == nullptr)
	{
		UE_LOG(LogClass, Error, TEXT("ASMMOPlayerController : Find SMMOGameInstance Error"));
		return;
	}
	GameInstance->SetPlayerControllerToSocket(this);

	// create ui object
	if (::IsValid(MainUIClass))
	{
		MainUIObject = CreateWidget<UUWMain>(this, MainUIClass);
		if (::IsValid(MainUIObject))
		{
			MainUIObject->AddToViewport();
		}
	}

	// login
	if (!FNetPacketHandler::SendLogin(GameInstance->SocketInstance, GameInstance->GetId(), GameInstance->GetPw(), GameInstance->GetCharacterNum()))
	{
		UE_LOG(LogClass, Error, TEXT("Login Error"));
		return;
	}

	// start socket thread
	if (!GameInstance->SocketInstance->StartClientSocket())
	{
		UE_LOG(LogClass, Error, TEXT("StartFClientSocket Error"));
		return;
	}

	// start player synchronize
	// GetWorldTimerManager().SetTimer(SendMoveHandle, this, &ASMMOPlayerController::SendMove, 0.1f, true);
}

void ASMMOPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

void ASMMOPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Chat"), IE_Pressed, this, &ASMMOPlayerController::FocusChat);
}

void ASMMOPlayerController::FocusChat()
{
	if (::IsValid(MainUIObject))
	{
		FInputModeUIOnly inputMode;
		inputMode.SetWidgetToFocus(MainUIObject->GetChatInputTextObject());

		SetInputMode(inputMode);
	}
}

void ASMMOPlayerController::FocusGame()
{
	SetInputMode(FInputModeGameOnly());
}

void ASMMOPlayerController::UpdateAndSendMove()
{
	if (GameInstance->IsLoginCompleted())
	{
		FVector Dist = GetPawn()->GetVelocity() - PrevVelocity;
		if (Dist.Size() > 3 || ElapsedTimes >= 1.0f)
		{
			PrevVelocity = GetPawn()->GetVelocity();
			FNetPacketHandler::SendMove(GameInstance->SocketInstance, this);
			ElapsedTimes = 0.0f;
		}
	}
}

void ASMMOPlayerController::UpdateMyPlayer()
{
	// set location
	FVector Location;
	Location.X = CharacterInfo.X;
	Location.Y = CharacterInfo.Y;
	Location.Z = CharacterInfo.Z;

	GetPawn()->SetActorLocation(Location);

	// set mesh
	ASMMOPlayer* SMMOPlayer = Cast<ASMMOPlayer>(GetPawn());
	if (::IsValid(SMMOPlayer))
	{
		SMMOPlayer->UpdateCharacterMesh(CharacterInfo.PlayerType);
	}

	// set SessionId
	if (::IsValid(GameInstance))
	{
		GameInstance->SessionId = CharacterInfo.SessionId;
		GameInstance->SetLoginComplete();
	}
}

void ASMMOPlayerController::UpdateAllPlayer()
{
	FCharacterInfo CharcterInfo;
	while (MovePlayers.Dequeue(CharacterInfo))
	{
		if (CharacterInfo.SessionId == GameInstance->SessionId)
		{
			continue;
		}
		UpdatePlayer(CharacterInfo);
	}
}

void ASMMOPlayerController::SpawnAllPlayer()
{
	FCharacterInfo CharcterInfo;
	while (EnterPlayers.Dequeue(CharcterInfo))
	{
		if (CharcterInfo.SessionId == GameInstance->SessionId)
		{
			continue;
		}
		SpawnPlayer(CharcterInfo);
	}
}

void ASMMOPlayerController::DeleteAllPlayer()
{
	int32 PlayerId;
	while (LeavePlayers.Dequeue(PlayerId))
	{
		if (PlayerId == GameInstance->SessionId)
		{
			continue;
		}
		DeletePlayer(PlayerId);
	}
}

void ASMMOPlayerController::UpdateAllChat()
{
	TTuple<int32, FString, FString> ChatData;
	while (Chats.Dequeue(ChatData))
	{
		if (::IsValid(MainUIObject))
		{
			UpdateChatAnimation(ChatData.Get<0>(), ChatData.Get<2>());
			MainUIObject->AddChatMsg(ChatData.Get<1>() + " : " + ChatData.Get<2>());
		}
	}
}

void ASMMOPlayerController::UpdateChatAnimation(int32 SessionId, FString Chat)
{
	if (Chat == "Hello" || Chat == "Hi")
	{
		if (SessionId == GameInstance->SessionId)
		{
			ASMMOPlayer* SMMOPlayer = Cast<ASMMOPlayer>(GetPawn());
			if (IsValid(SMMOPlayer) && SMMOPlayer->WavingMontage != nullptr)
			{
				float Ret = SMMOPlayer->PlayAnimMontage(SMMOPlayer->WavingMontage);
				if (Ret == 0.0f)
				{
					UE_LOG(LogClass, Error, TEXT("play anim montage error"));
				}
			}
		}
		else
		{
			if (Players.Contains(SessionId))
			{
				ASMMOOtherPlayer* SMMOPlayer = (*Players.Find(SessionId));
				if (SMMOPlayer != nullptr && SMMOPlayer->WavingMontage != nullptr)
				{
					SMMOPlayer->PlayAnimMontage(SMMOPlayer->WavingMontage);
				}
			}
		}
	}
}

void ASMMOPlayerController::UpdatePlayer(FCharacterInfo& PlayerInfo)
{
	ASMMOOtherPlayer** SMMOPlayer = nullptr;
	if ((SMMOPlayer = Players.Find(PlayerInfo.SessionId)) == nullptr)
	{
		UE_LOG(LogClass, Error, TEXT("UpdatePlayer, player doesn't exist"));
		return;
	}

	FVector Location;
	FRotator Rotation;
	FVector Velocity;

	Location.X = PlayerInfo.X;
	Location.Y = PlayerInfo.Y;
	Location.Z = PlayerInfo.Z;

	Rotation.Yaw = PlayerInfo.Yaw;
	Rotation.Pitch = PlayerInfo.Pitch;
	Rotation.Roll = PlayerInfo.Roll;

	Velocity.X = PlayerInfo.VX;
	Velocity.Y = PlayerInfo.VY;
	Velocity.Z = PlayerInfo.VZ;
	
	// calc current location
	int64 CurrTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	float DTime = ((float)(CurrTime - PlayerInfo.TimeStamp)) / 1000000.0f;

	FVector NewLocation;
	NewLocation.X = Location.X + DTime * Velocity.X;
	NewLocation.Y = Location.Y + DTime * Velocity.Y;
	NewLocation.Z = Location.Z + DTime * Velocity.Z;

	// set velocity, location
	(*SMMOPlayer)->PlayerVelocity = Velocity;
	(*SMMOPlayer)->SetActorLocation(NewLocation);
}

void ASMMOPlayerController::SpawnPlayer(FCharacterInfo& PlayerInfo)
{
	if (Players.Contains(PlayerInfo.SessionId))
	{
		//UE_LOG(LogClass, Log, TEXT("SpawnPlayer, Player already exist"));
		return;
	}

	FVector Location;
	FRotator Rotation;
	FVector Velocity;

	Location.X = PlayerInfo.X;
	Location.Y = PlayerInfo.Y;
	Location.Z = PlayerInfo.Z;

	Rotation.Yaw = PlayerInfo.Yaw;
	Rotation.Pitch = PlayerInfo.Pitch;
	Rotation.Roll = PlayerInfo.Roll;

	Velocity.X = PlayerInfo.VX;
	Velocity.Y = PlayerInfo.VY;
	Velocity.Z = PlayerInfo.VZ;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	//spawnParams.Name = FName(*FString::FromInt(playerInfo.SessionId));

	ASMMOOtherPlayer* SpawnCharacter = GetWorld()->SpawnActor<ASMMOOtherPlayer>(Location, Rotation, SpawnParams);
	if (SpawnCharacter == nullptr)
	{
		return;
	}

	// set player default controller
	SpawnCharacter->SpawnDefaultController();

	// set player mesh
	SpawnCharacter->UpdateCharacterMesh(PlayerInfo.PlayerType);

	// set velocity
	SpawnCharacter->PlayerVelocity = Velocity;

	// set player name
	SpawnCharacter->SetCharacterName(FText::FromString(PlayerInfo.PlayerId));

	UE_LOG(LogClass, Log, TEXT("Spawn Player, %d, %d"), PlayerInfo.SessionId, PlayerInfo.PlayerType);

	Players.Add(PlayerInfo.SessionId, SpawnCharacter);
}

void ASMMOPlayerController::DeletePlayer(int32 SessionId)
{
	if (!Players.Contains(SessionId))
	{
		return;
	}

	ASMMOOtherPlayer* player = *Players.Find(SessionId);

	if (!player->Destroy())
	{
		UE_LOG(LogClass, Error, TEXT("DeletePlayer, Player Destroy Fail, SessionId = %d"), SessionId);
		return;
	}

	Players.Remove(SessionId);
}

void ASMMOPlayerController::RecvSessionId(int32 SessionId)
{
	if (IsValid(GameInstance))
	{
		GameInstance->SessionId = SessionId;
	}
}

void ASMMOPlayerController::RecvLoginResult(bool Result, FCharacterInfo& Info)
{
	bLoginComplete = Result;
	if (bLoginComplete)
	{
		CharacterInfo = Info;
	}
}

void ASMMOPlayerController::RecvMove(FCharacterInfo& Info)
{
	MovePlayers.Enqueue(Info);
	bRecvMove = true;
}

void ASMMOPlayerController::RecvEnter(FCharacterInfo& Info)
{
	EnterPlayers.Enqueue(Info);
	bRecvEnter = true;
}

void ASMMOPlayerController::RecvLeave(int32 SessionId)
{
	LeavePlayers.Enqueue(SessionId);
	bRecvLeave = true;
}

void ASMMOPlayerController::RecvChat(int32 SessionId, FString StrId, FString Chat)
{
	TTuple<int32, FString, FString> chatData;
	chatData.Get<0>() = SessionId;
	chatData.Get<1>() = StrId;
	chatData.Get<2>() = Chat;

	Chats.Enqueue(chatData);

	bRecvChat = true;
}

void ASMMOPlayerController::SendMove()
{
	if (GameInstance->IsLoginCompleted())
	{
		FNetPacketHandler::SendMove(GameInstance->SocketInstance, this);
	}
}

void ASMMOPlayerController::SendChatMsg(const FText& Text)
{
	if (GameInstance->IsLoginCompleted())
	{
		FNetPacketHandler::SendChat(GameInstance->SocketInstance, this, Text);
	}
}

