
#include "outputbuffer.h"
#include <stdio.h>

//----------------------------------------------------------------------------//

FileOutputBuffer::FileOutputBuffer(const char* pFilename) {
	m_pFile		= fopen(pFilename, "wb");
	m_pBuffer	= 0;
	m_size 		= 0;
}

FileOutputBuffer::~FileOutputBuffer() {
	fclose(m_pFile);
}

void FileOutputBuffer::setBuffer(uint8* pBuffer, uint32 size) {
	m_pBuffer	= pBuffer;
	m_size		= size;
	m_isMemoryInternal = 0;

	fwrite(m_pBuffer, 1, m_size, m_pFile);
	delete[] m_pBuffer;
}

//----------------------------------------------------------------------------//

MemoryOutputBuffer::MemoryOutputBuffer() {
	m_pBuffer	= 0;
	m_size 		= 0;
}

MemoryOutputBuffer::~MemoryOutputBuffer() {
}

void MemoryOutputBuffer::setBuffer(uint8* pBuffer, uint32 size) {
	m_pBuffer	= pBuffer;
	m_size		= size;
	m_isMemoryInternal = 0;
}

//----------------------------------------------------------------------------//
