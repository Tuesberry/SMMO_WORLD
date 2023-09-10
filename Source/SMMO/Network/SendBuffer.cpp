#include "SendBuffer.h"

FSendBuffer::FSendBuffer(int BufferSize)
	: BufferSize(BufferSize)
	, WritePos(0)
	, Buffer()
{
	Buffer.resize(BufferSize);
}

bool FSendBuffer::OnWrite(int NumOfBytes)
{
	if (NumOfBytes > GetFreeSize())
	{
		return false;
	}

	WritePos += NumOfBytes;
	return true;
}

