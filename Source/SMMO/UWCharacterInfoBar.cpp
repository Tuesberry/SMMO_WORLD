// Fill out your copyright notice in the Description page of Project Settings.

#include "UWCharacterInfoBar.h"
#include "Components/TextBlock.h"

void UUWCharacterInfoBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// get textblock
	CharacterNameTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("CharacterName")));
}

void UUWCharacterInfoBar::SetCharacterNameText(FText Text)
{
	CharacterNameTextBlock->SetText(Text);
}
