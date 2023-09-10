// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWTitle.generated.h"

/**
 * 
 */
UCLASS()
class SMMO_API UUWTitle : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UUWStartMenu* StartMenu;

protected:
	virtual void NativeOnInitialized();
};
