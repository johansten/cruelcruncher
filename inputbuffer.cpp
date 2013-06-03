
#include "inputbuffer.h"
#include "types.h"

#include <stdio.h>			//	FILE*
#include <sys/stat.h>		//	stat

//----------------------------------------------------------------------------//

static uint32 fsize(const char* pFilename);

//----------------------------------------------------------------------------//

uint32 InputBuffer::getSize() {
	return m_size;
}

//----------------------------------------------------------------------------//

MemoryInputBuffer::MemoryInputBuffer(uint8* pBuffer, uint32 size) {
	m_pBuffer	= pBuffer;
	m_size		= size;	
}

uint8* MemoryInputBuffer::getBuffer() {
	return m_pBuffer;
}

//----------------------------------------------------------------------------//

FileInputBuffer::FileInputBuffer(FILE* pFile, uint32 size) {
	m_pFile = pFile;
	m_size	= size;
	m_isInternalFile = 0;
}

FileInputBuffer::FileInputBuffer(const char* pFilename) {
	m_pFile = fopen(pFilename, "rb");
	m_size	= fsize(pFilename);
	m_isInternalFile = 1;
}

FileInputBuffer::~FileInputBuffer() {
}

uint8* FileInputBuffer::getBuffer() {
	uint8* pBuffer = new uint8[m_size];
	fread(pBuffer, 1, m_size, m_pFile);

	if (m_isInternalFile) {
		fclose(m_pFile);
	}

	return pBuffer;
}

//----------------------------------------------------------------------------//

static uint32 fsize(const char* pFilename) {
	struct stat _stat;
    if (stat(pFilename, &_stat) == 0) {
        return _stat.st_size;
    }

    return 0;
}

//----------------------------------------------------------------------------//
