// Fill out your copyright notice in the Description page of Project Settings.

#include "UWTitle.h"
#include "UWStartMenu.h"

void UUWTitle::NativeOnInitialized()
{
	StartMenu = Cast<UUWStartMenu>(GetWidgetFromName(TEXT("UWStartMenu")));
}
