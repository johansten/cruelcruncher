
#ifndef OUTPUTSTREAM_H
#define OUTPUTSTREAM_H

//----------------------------------------------------------------------------//

#include <stdio.h>		//	FILE*
#include "types.h"

//----------------------------------------------------------------------------//

class OutputStream {

public:

	OutputStream();
	virtual ~OutputStream() {};

	void flushStream();
	void outputBits(uint32 pattern, uint32 numBits);
	virtual void outputDWord(uint32 output) = 0;

private:

	uint64	m_output;
	uint32	m_bitsUsed;
};

//----------------------------------------------------------------------------//

class FileOutputStream : public OutputStream {

public:

	FileOutputStream(FILE* pFile, uint32 size = 1024);
	FileOutputStream(const char* pFilename, uint32 size = 1024);
	virtual ~FileOutputStream();
	virtual void outputDWord(uint32 output);

private:

	FILE*	m_pFile;
	uint32*	m_pBuffer;
	uint32	m_size;
	uint32	m_pos;
};

//----------------------------------------------------------------------------//

#endif	//	OUTPUTSTREAM_H
