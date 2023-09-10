#pragma once

#include "CoreMinimal.h"

struct FCharacterInfo
{
	int64 TimeStamp;
	int32 SessionId;
	int32 PlayerType;
	FString PlayerId;
	float X;
	float Y;
	float Z;
	float Yaw;
	float Pitch;
	float Roll;
	float VX;
	float VY;
	float VZ;
};

