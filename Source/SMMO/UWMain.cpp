// Fill out your copyright notice in the Description page of Project Settings.

#include "UWMain.h"
#include "UWChat.h"

void UUWMain::NativeConstruct()
{
	Super::NativeConstruct();

	UWChatBox = Cast<UUWChat>(GetWidgetFromName(TEXT("UWChat")));
}

TSharedPtr<SWidget> UUWMain::GetChatInputTextObject()
{
	return UWChatBox->GetChatInputTextObject();
}

void UUWMain::AddChatMsg(const FString& Message)
{
	UWChatBox->AddChatMsg(Message);
}
