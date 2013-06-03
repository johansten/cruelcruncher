
#ifndef INPUTSTREAM_H
#define INPUTSTREAM_H

//----------------------------------------------------------------------------//

#include <stdio.h>		//	FILE*
#include "types.h"

//----------------------------------------------------------------------------//

class InputStream {

public:

	InputStream();
	virtual ~InputStream() {};
	uint32 inputBits(uint32 numBits);
	virtual uint32 inputDWord() = 0;

private:
	uint64	m_input;
	uint32	m_bitsLeft;
};

//----------------------------------------------------------------------------//

class FileInputStream : public InputStream {

public:

	FileInputStream(const char* pFilename, uint32 size = 1024);
	virtual ~FileInputStream();
	virtual uint32 inputDWord();

private:

	FILE*	m_pFile;
	uint32*	m_pBuffer;
	uint32	m_size;
	uint32	m_pos;
};

//----------------------------------------------------------------------------//

#endif //INPUTSTREAM_H
