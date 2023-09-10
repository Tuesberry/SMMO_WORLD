// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWStartMenu.generated.h"

/**
 * 
 */
UCLASS()
class SMMO_API UUWStartMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* CharacterSelectLeftButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* CharacterSelectRightButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* JoinServerButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UEditableTextBox* IdTextBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UEditableTextBox* PwTextBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* CharacterNameTextBlock;

protected:
	virtual void NativeOnInitialized() override; // 생성자 역할

public:
	void SetCharacterText(FText text);

private:
	UFUNCTION(BlueprintCallable)
		void CharacterSelectLeftButtonCallback();

	UFUNCTION(BlueprintCallable)
		void CharacterSelectRightButtonCallback();

	UFUNCTION(BlueprintCallable)
		void JoinServerCallback();

private:
	void UpdateCharacterNumToRight();
	void UpdateCharacterNumToLeft();

	int32 CharacterNum = 0;
};
