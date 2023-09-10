#include "RecvBuffer.h"
#include "HAL/UnrealMemory.h"

FRecvBuffer::FRecvBuffer(int32 BufferSize)
	: Capacity(BufferSize * 3)
	, BufferSize(BufferSize)
	, ReadPos(0)
	, WritePos(0)
	, Buffer()
{
	Buffer.resize(Capacity);
}

bool FRecvBuffer::OnRead(int32 NumOfBytes)
{
	if (NumOfBytes > GetDataSize())
	{
		return false;
	}

	ReadPos += NumOfBytes;
	return true;
}

bool FRecvBuffer::OnWrite(int32 NumOfBytes)
{
	if (NumOfBytes > GetFreeSize())
	{
		return false;
	}

	WritePos += NumOfBytes;
	return true;
}

void FRecvBuffer::AdjustPos()
{
	int32 dataSize = GetDataSize();
	if (ReadPos == WritePos)
	{
		ReadPos = 0;
		WritePos = 0;
	}
	else
	{
		if (GetFreeSize() < (BufferSize))
		{
			::memcpy(&Buffer[0], &Buffer[ReadPos], dataSize);
			ReadPos = 0;
			WritePos = dataSize;
		}
	}
}
