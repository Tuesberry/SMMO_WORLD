// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWChat.generated.h"

/**
 * 
 */
UCLASS()
class SMMO_API UUWChat : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUWChat(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	// add chat msg in area
	void AddChatMsg(const FString& Msg);

	// set chat input text
	void SetChatInputTextMessage(const FText& Text);

	// get widget
	// next time, press enter, then focus on chat
	TSharedPtr<SWidget> GetChatInputTextObject();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UScrollBox* ChatAreaBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UEditableTextBox* ChatInputBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UUWChatDataBox> ChatDataBoxClass;

private:
	UFUNCTION()
		void OnChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
