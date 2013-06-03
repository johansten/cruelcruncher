
#ifndef OUTPUTBUFFER_H
#define OUTPUTBUFFER_H

//----------------------------------------------------------------------------//

#include "types.h"

#include <stdio.h>

//----------------------------------------------------------------------------//

class OutputBuffer {

public:

	virtual void setBuffer(uint8* pBuffer, uint32 size) = 0;

protected:

	uint8*	m_pBuffer;
	uint32	m_size;
	uint32	m_isMemoryInternal;
};

//----------------------------------------------------------------------------//

class FileOutputBuffer : public OutputBuffer {

public:

	FileOutputBuffer(const char* pFilename);
	~FileOutputBuffer();
	virtual void setBuffer(uint8* pBuffer, uint32 size);

public:

	FILE*	m_pFile;
};

//----------------------------------------------------------------------------//

class MemoryOutputBuffer : public OutputBuffer {

public:

	MemoryOutputBuffer();
	~MemoryOutputBuffer();
	virtual void setBuffer(uint8* pBuffer, uint32 size);

};

//----------------------------------------------------------------------------//

#endif	//	OUTPUTBUFFER_H
