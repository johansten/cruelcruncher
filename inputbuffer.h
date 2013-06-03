
#ifndef INPUTBUFFER_H
#define INPUTBUFFER_H

//----------------------------------------------------------------------------//

#include "types.h"
#include <stdio.h>

//----------------------------------------------------------------------------//

class InputBuffer {

public:
	uint32 getSize();
	virtual uint8* getBuffer() = 0;

protected:
	uint32	m_size;

};

//----------------------------------------------------------------------------//

class MemoryInputBuffer : public InputBuffer {

public:

	MemoryInputBuffer(uint8* pBuffer, uint32 size);
	~MemoryInputBuffer();
	virtual uint8* getBuffer();

private:

	uint8*	m_pBuffer;
};

//----------------------------------------------------------------------------//

class FileInputBuffer : public InputBuffer {

public:

	FileInputBuffer(FILE* pFile, uint32 size);
	FileInputBuffer(const char* pFilename);
	~FileInputBuffer();
	virtual uint8* getBuffer();

private:

	FILE*	m_pFile;
	uint32	m_isInternalFile;
};

//----------------------------------------------------------------------------//

#endif	//	INPUTBUFFER_H
