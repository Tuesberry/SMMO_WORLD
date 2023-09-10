// Fill out your copyright notice in the Description page of Project Settings.

#include "UWChat.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "SMMOPlayerController.h"
#include "UWChatDataBox.h"

UUWChat::UUWChat(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUWChatDataBox> UWChatDataBox(TEXT("/Game/Blueprint/UWChattingBox"));
	if (UWChatDataBox.Succeeded())
	{
		ChatDataBoxClass = UWChatDataBox.Class;
	}
}

void UUWChat::NativeConstruct()
{
	Super::NativeConstruct();
	
	// get chat
	ChatAreaBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ChatArea")));
	ChatInputBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ChatInput")));

	// bind func
	if (::IsValid(ChatInputBox))
	{
		ChatInputBox->OnTextCommitted.AddDynamic(this, &UUWChat::OnChatTextCommitted);
	}
}

void UUWChat::AddChatMsg(const FString& Msg)
{
	UUWChatDataBox* NewText = CreateWidget<UUWChatDataBox>(GetWorld(), ChatDataBoxClass);
	
	if (::IsValid(NewText))
	{
		NewText->SetText(FText::FromString(Msg));

		ChatAreaBox->AddChild(NewText);
		ChatAreaBox->ScrollToEnd();
	}
}

void UUWChat::SetChatInputTextMessage(const FText& Text)
{
	if (::IsValid(ChatInputBox))
	{
		ChatInputBox->SetText(Text);
	}
}

TSharedPtr<SWidget> UUWChat::GetChatInputTextObject()
{
	return ChatInputBox->GetCachedWidget();
}

void UUWChat::OnChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	ASMMOPlayerController* PController = Cast<ASMMOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PController == nullptr)
	{
		return;
	}

	switch (CommitMethod)
	{
	case ETextCommit::OnEnter:
		if (!Text.IsEmpty())
		{
			PController->SendChatMsg(Text);
			SetChatInputTextMessage(FText::GetEmpty());
		}
		PController->FocusGame();
		break;
	case ETextCommit::OnCleared:
		PController->FocusGame();
		break;
	default:
		break;
	}
}
