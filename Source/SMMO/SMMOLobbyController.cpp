// Fill out your copyright notice in the Description page of Project Settings.

#include "SMMOLobbyController.h"

#include "UWTitle.h"
#include "Kismet/GameplayStatics.h"
#include "SMMOGameInstance.h"
#include "SMMOMannequinActor.h"
#include "UWStartMenu.h"
#include "Camera/CameraActor.h"
#include "Network/NetPacketHandler.h"
#include "Network/NetworkData.h"

ASMMOLobbyController::ASMMOLobbyController()
{
	static ConstructorHelpers::FClassFinder<UUWTitle> WB_Title(TEXT("WidgetBlueprint'/Game/Blueprint/UWTitle.UWTitle_C'"));
	if (WB_Title.Succeeded())
	{
		TitleUIClass = WB_Title.Class;
	}
}

void ASMMOLobbyController::BeginPlay()
{
	Super::BeginPlay();

	// set title widget
	TitleUIObject = CreateWidget<UUWTitle>(this, TitleUIClass);
	TitleUIObject->AddToViewport();

	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());

	// get mannequin actor
	LobbyMannequin = Cast<ASMMOMannequinActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ASMMOMannequinActor::StaticClass()));
	if (LobbyMannequin == nullptr)
	{
		UE_LOG(LogClass, Error, TEXT("Find LobbyMannequin Error"));
	}

	// get gameinstance
	GameInstance = Cast<USMMOGameInstance>(GetGameInstance());
	if (GameInstance == nullptr)
	{
		UE_LOG(LogClass, Error, TEXT("Find SMMOGameInstance Error"));
	}

	// get Camera
	Camera = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass()));
	if (Camera == nullptr)
	{
		UE_LOG(LogClass, Error, TEXT("Find Camera Error"));
	}
	else
	{
		// set view target with camera
		SetViewTarget(Camera);
	}

	// set init
	InitMannequin();

	GetPawn()->SetActorHiddenInGame(true);
}

void ASMMOLobbyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

FName ASMMOLobbyController::SetCharacterMesh(int32 Row)
{
	if (Row <= 0 || Row > GameInstance->GetCharacterDataTableRowNum())
	{
		return FName();
	}

	FCharacterData* Data = GameInstance->GetCharacterDataByNum(Row);
	if (Data == nullptr)
	{
		return FName();
	}

	LobbyMannequin->SetSkeletalMesh(Data->Mesh);
	LobbyMannequin->SetAnimInstanceClass(Data->AnimInstance);

	return Data->Name;
}

void ASMMOLobbyController::SaveInfoAndStartGame(FText Id, FText Pw, int32 CharacterNum)
{
	if (!IsValid(GameInstance))
	{
		return;
	}

	// save info in gameInstance
	GameInstance->SetCharacterInfo(Id.ToString(), Pw.ToString(), CharacterNum);

	// change level
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Stylized_Egypt/Maps/Stylized_Egypt_Demo"), TRAVEL_Absolute);
}

void ASMMOLobbyController::InitMannequin()
{
	FName Name = SetCharacterMesh(1);
	if (::IsValid(TitleUIObject))
	{
		TitleUIObject->StartMenu->SetCharacterText(FText::FromName(Name));
	}
}
