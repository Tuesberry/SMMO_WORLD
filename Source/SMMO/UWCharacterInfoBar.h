// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWCharacterInfoBar.generated.h"

/**
 * 
 */
UCLASS()
class SMMO_API UUWCharacterInfoBar : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* CharacterNameTextBlock;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetCharacterNameText(FText Text);
};
