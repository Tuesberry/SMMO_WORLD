// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWMain.generated.h"

/**
 * 
 */
UCLASS()
class SMMO_API UUWMain : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	TSharedPtr<class SWidget> GetChatInputTextObject();
	void AddChatMsg(const FString& Message);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UUWChat* UWChatBox;


};
