#pragma once

#include "CoreMinimal.h"

#include <vector>

class SMMO_API FRecvBuffer
{
public:
	FRecvBuffer(int32 BufferSize);

	// get
	int32 GetDataSize() { return WritePos - ReadPos; }
	int32 GetFreeSize() { return Capacity - WritePos; }

	// access data
	char* GetReadPos() { return &Buffer[ReadPos]; }
	char* GetWritePos() { return &Buffer[WritePos]; }

	// read & write data
	bool OnRead(int32 NumOfBytes);
	bool OnWrite(int32 NumOfBytes);

	// read & write pos adjust
	void AdjustPos();

private:
	int32 Capacity;
	int32 BufferSize;
	int32 ReadPos;
	int32 WritePos;

	std::vector<char> Buffer;
};