
#include "compress.h"
#include "header.h"
#include "bwt.h"
#include "mtf.h"
#include "arithcoder.h"
#include "outputstream.h"
#include "inputbuffer.h"
#include "structuredmodel.h"
#include "rle.h"

//----------------------------------------------------------------------------//

static void getModelStatistics(const uint8* pBuffer, const uint32 length, uint8* pIncrements, float* pStats);
static void optimizeIncrements(const uint8* pBuffer, const uint32 length, uint8* pIncrements);
static void writeHeader(OutputStream& os, header& h);

//----------------------------------------------------------------------------//

void compress(const char* pInputFilename, const char* pOutputFilename) {
	FileInputBuffer  input(pInputFilename);
	FileOutputStream output(pOutputFilename);
	compress(input, output);
}

//----------------------------------------------------------------------------//

void compress(InputBuffer& input, OutputStream& output) {

	header h;

	h.fileSize = input.getSize();
	uint8* pInputBuffer = input.getBuffer();
	uint8* pOutputBuffer = new uint8[h.fileSize];

	RLE(pInputBuffer, pOutputBuffer, h.fileSize, &h.tempSize);
	BWT(pOutputBuffer, pInputBuffer, h.tempSize, &h.first);
	MTF(pInputBuffer, h.tempSize);

	optimizeIncrements(pInputBuffer, h.tempSize, h.increments);

#if 0
	float model_stats[8];
	getModelStatistics(pInputBuffer, h.tempSize, h.increments,  model_stats);

	float sum = 0;
	for (uint32 i = 0; i < 8; i++) {
		printf("model #%d: %12.2f bits (increment: %3d)\n", i, model_stats[i], h.increments[i]);
		sum += model_stats[i];
	}
	printf("%f bpb\n", sum / h.fileSize);
#endif

	writeHeader(output, h);
	ArithmeticEncoder	encoder(output);
	StructuredModel		model(h.increments);
	model.encode(encoder, pInputBuffer, h.tempSize);

	delete[] pInputBuffer;
	delete[] pOutputBuffer;
}

//----------------------------------------------------------------------------//

static void writeHeader(OutputStream& os, header& h) {

	os.outputBits(h.first, 32);

	for (uint32 i = 0; i < 8; i++) {
		os.outputBits(h.increments[i], 8);
	}

	os.outputBits(h.fileSize, 32);
	os.outputBits(h.tempSize, 32);
}

//----------------------------------------------------------------------------//

static void getModelStatistics(const uint8* pBuffer, const uint32 length, uint8* pIncrements, float* pStats) {

	StructuredModel model(pIncrements);

	for (uint32 i = 0; i < 8; i++) {
		pStats[i] = 0.0f;
	}

	uint32 zeroRun = 0;
	for (uint32 i = 0; i < length; i++) {
		uint32 symbol = pBuffer[i];
		if (symbol == 0) {
			zeroRun++;
		} else {
			if (zeroRun) {
				model.getZeroRunEntropy(pStats, zeroRun);
				zeroRun = 0;
			}

			model.getSymbolEntropy(pStats, symbol);
		}
	}

	if (zeroRun) {
		model.getZeroRunEntropy(pStats, zeroRun);
	}
}

//----------------------------------------------------------------------------//

static void optimizeIncrements(const uint8* pBuffer, const uint32 length, uint8* pIncrements) {

	uint8 left[8];
	uint8 right[8];

	for (uint32 i = 0; i < 8; i++) {
		left[i]  = 0;
		right[i] = 255;
	}

	uint32 iter = 0;

	uint32 not_done = 0xff;
	while (not_done) {
		uint8 left3[8];
		uint8 right3[8];

		for (uint32 i = 0; i < 8; i++) {
			left3[i]  = (2*left[i] +   right[i]) / 3;
			right3[i] = (  left[i] + 2*right[i]) / 3;
		}

		float f_left[8];
		float f_right[8];
		getModelStatistics(pBuffer, length, left3,  f_left);
		getModelStatistics(pBuffer, length, right3, f_right);

		for (uint32 i = 0; i < 8; i++) {

			if ((right3[i] - left3[i]) == 1) {
				if (f_left[i] < f_right[i]) {
					pIncrements[i] = left3[i];
				} else {
					pIncrements[i] = right3[i];
				}
				not_done &= ~(1 << i);
			}

			else {
				if (f_left[i] < f_right[i]) {
					right[i] = right3[i];
				} else {
					left[i]  = left3[i];
				}
			}
		}

		iter++;
	}

//	printf("num iterations: %d\n", iter);
}

//----------------------------------------------------------------------------//
