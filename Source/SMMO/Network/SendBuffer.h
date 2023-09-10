#pragma once

#include "CoreMinimal.h"

#include <vector>

class SMMO_API FSendBuffer
{
public:
	FSendBuffer(int BufferSize);

	// get
	int GetDataSize() { return WritePos; }
	int GetFreeSize() { return BufferSize - WritePos; }

	// access data
	char* GetData() { return &Buffer[0]; }
	char* GetWritePos() { return &Buffer[WritePos]; }

	// send & write data
	bool OnWrite(int NumOfBytes);

private:
	int BufferSize;
	int WritePos;
	std::vector<char> Buffer;
};