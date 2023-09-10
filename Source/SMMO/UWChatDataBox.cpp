// Fill out your copyright notice in the Description page of Project Settings.

#include "UWChatDataBox.h"
#include "Components/TextBlock.h"

void UUWChatDataBox::NativeConstruct()
{
	Super::NativeConstruct();

	ChatDataBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("ChatText")));
}

void UUWChatDataBox::SetText(FText Text)
{
	if(::IsValid(ChatDataBlock))		
	{
		ChatDataBlock->SetText(Text);
	}
	else
	{
		ChatDataBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("ChatText")));
		if (::IsValid(ChatDataBlock))
		{
			ChatDataBlock->SetText(Text);
		}
	}
}
