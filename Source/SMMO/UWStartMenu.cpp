// Fill out your copyright notice in the Description page of Project Settings.

#include "UWStartMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "SMMOGameInstance.h"
#include "SMMOLobbyController.h"
#include "UWMain.h"

void UUWStartMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// get button
	CharacterSelectLeftButton = Cast<UButton>(GetWidgetFromName(TEXT("LeftButton")));
	CharacterSelectRightButton = Cast<UButton>(GetWidgetFromName(TEXT("RightButton")));
	JoinServerButton = Cast<UButton>(GetWidgetFromName(TEXT("JoinButton")));

	// get editable text box
	IdTextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ID_TextBox")));
	PwTextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("PW_TextBox")));

	// get textblock
	CharacterNameTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("CharacterName")));
}

void UUWStartMenu::SetCharacterText(FText text)
{
	CharacterNameTextBlock->SetText(text);
}

void UUWStartMenu::CharacterSelectLeftButtonCallback()
{
	UpdateCharacterNumToLeft();

	ASMMOLobbyController* LobbyController = Cast<ASMMOLobbyController>(GetOwningPlayer());
	if (::IsValid(LobbyController))
	{
		FName Name = LobbyController->SetCharacterMesh(CharacterNum + 1);
		CharacterNameTextBlock->SetText(FText::FromName(Name));
	}
}

void UUWStartMenu::CharacterSelectRightButtonCallback()
{
	UpdateCharacterNumToRight();

	ASMMOLobbyController* LobbyController = Cast<ASMMOLobbyController>(GetOwningPlayer());
	if (::IsValid(LobbyController))
	{
		FName Name = LobbyController->SetCharacterMesh(CharacterNum + 1);
		CharacterNameTextBlock->SetText(FText::FromName(Name));
	}
}

void UUWStartMenu::JoinServerCallback()
{
	ASMMOLobbyController* LobbyController = Cast<ASMMOLobbyController>(GetOwningPlayer());
	if (::IsValid(LobbyController))
	{
		LobbyController->SaveInfoAndStartGame(IdTextBox->GetText(), PwTextBox->GetText(), CharacterNum + 1);
	}
}

void UUWStartMenu::UpdateCharacterNumToRight()
{
	USMMOGameInstance* GameInstance = Cast<USMMOGameInstance>(GetGameInstance());
	if (::IsValid(GameInstance))
	{
		int32 WholeNum = GameInstance->GetCharacterDataTableRowNum();
		CharacterNum = (CharacterNum + 1) % WholeNum;
	}
}

void UUWStartMenu::UpdateCharacterNumToLeft()
{
	USMMOGameInstance* GameInstance = Cast<USMMOGameInstance>(GetGameInstance());
	if (::IsValid(GameInstance))
	{
		int32 WholeNum = GameInstance->GetCharacterDataTableRowNum();
		CharacterNum = (CharacterNum + WholeNum - 1) % WholeNum;
	}
}



