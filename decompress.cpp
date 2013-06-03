
#include "decompress.h"
#include "header.h"
#include "inputstream.h"
#include "outputbuffer.h"
#include "structuredmodel.h"
#include "arithcoder.h"
#include "mtf.h"
#include "bwt.h"
#include "rle.h"

//----------------------------------------------------------------------------//

void decompress(const char* pInputFilename, const char* pOutputFilename) {
	FileInputStream  input(pInputFilename);
	FileOutputBuffer output(pOutputFilename);
	decompress(input, output);
}

//----------------------------------------------------------------------------//

void decompress(InputStream& inputStream, OutputBuffer& output, header* pHeader) {

	header h;
	if (pHeader) {
		h = *pHeader;
	} else {
		readHeader(inputStream, h);
	}

	uint8* pInputBuffer	 = new uint8[h.fileSize];
	uint8* pOutputBuffer = new uint8[h.fileSize];

	ArithmeticDecoder	decoder(inputStream);
	StructuredModel		model(h.increments);
	model.decode(decoder, pInputBuffer, h.tempSize);

	iMTF(pInputBuffer, h.tempSize);
	iBWT(pInputBuffer, pOutputBuffer, h.tempSize, h.first);
	iRLE(pOutputBuffer, pInputBuffer, h.tempSize);

	output.setBuffer(pInputBuffer, h.fileSize);
	delete[] pOutputBuffer;
}

//----------------------------------------------------------------------------//

void readHeader(InputStream& is, header& h) {

	h.first = is.inputBits(32);

	for (uint32 i = 0; i < 8; i++) {
		h.increments[i] = is.inputBits(8);
	}

	h.fileSize = is.inputBits(32);
	h.tempSize = is.inputBits(32);
}

//----------------------------------------------------------------------------//
