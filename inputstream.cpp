
#include "inputstream.h"

//----------------------------------------------------------------------------//

InputStream::InputStream() {
	m_input		= 0;
	m_bitsLeft	= 0;
}

uint32 InputStream::inputBits(uint32 numBits) {

	if (numBits > m_bitsLeft) {
		m_input <<= 32;
		m_input  |= inputDWord();
		m_bitsLeft += 32;
	}

	m_input <<= numBits;
	uint32 result = m_input >> 32;
	m_input  &= 0xffffffff;
	m_bitsLeft -= numBits;
	return result;
}

//----------------------------------------------------------------------------//

FileInputStream::FileInputStream(const char* pFilename, uint32 size) {
	m_pFile		= fopen(pFilename, "rb");
	m_size		= size / 4;
	m_pBuffer	= new uint32[m_size];
	m_pos		= 0;
}

FileInputStream::~FileInputStream() {
	fclose(m_pFile);
	delete m_pBuffer;
};

uint32 FileInputStream::inputDWord() {

	if (m_pos == 0) {
		fread(m_pBuffer, m_size, 4, m_pFile);
	}

	uint32 input = m_pBuffer[m_pos++];
	if (m_pos == m_size) {
		m_pos = 0;
	}

	return input;
}

//----------------------------------------------------------------------------//
