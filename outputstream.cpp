
#include "outputstream.h"

//----------------------------------------------------------------------------//

OutputStream::OutputStream() {
	m_output	= 0;
	m_bitsUsed	= 0;
}

void OutputStream::flushStream() {
	if (m_bitsUsed) {
		outputDWord(m_output << (32 - m_bitsUsed));
	}
}

void OutputStream::outputBits(uint32 pattern, uint32 numBits) {
	m_output = (m_output << numBits) | pattern;
	m_bitsUsed += numBits;
	if (m_bitsUsed >= 32) {
		m_bitsUsed -= 32;
		outputDWord(m_output >> m_bitsUsed);
		m_output &= ((1 << m_bitsUsed) - 1);
	}
}

//----------------------------------------------------------------------------//

FileOutputStream::FileOutputStream(const char* pFilename, uint32 size) {
	m_pFile		= fopen(pFilename, "wb");
	m_size		= size / 4;
	m_pBuffer	= new uint32[m_size];
	m_pos		= 0;
}

FileOutputStream::FileOutputStream(FILE* pFile, uint32 size) {
	m_pFile		= pFile;
	m_size		= size / 4;
	m_pBuffer	= new uint32[m_size];
	m_pos		= 0;
}

FileOutputStream::~FileOutputStream() {
	flushStream();
	fwrite(m_pBuffer, m_pos, 4, m_pFile);
	fclose(m_pFile);
	delete m_pBuffer;
};

void FileOutputStream::outputDWord(uint32 output) {

	m_pBuffer[m_pos++] = output;

	if (m_pos == m_size) {
		fwrite(m_pBuffer, m_size, 4, m_pFile);
		m_pos = 0;
	}
}

//----------------------------------------------------------------------------//

