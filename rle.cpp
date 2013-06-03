
#include "rle.h"

//----------------------------------------------------------------------------//

#define MINIMUM_RUN 6

//----------------------------------------------------------------------------//

extern "C" {

//----------------------------------------------------------------------------//

void RLE(const uint8* pInputBuffer, uint8* pOutputBuffer, const uint32 inputSize, uint32* outputSize) {

	uint32 index = 0;
	uint32 i = 0;
	uint32 prevSymbol = 256;
	uint32 runLength = 0;
	for (;;) {

		uint32 symbolPending = 0;
		uint32 symbol;

		//
		//	input[n] != input[n-1]
		//

		while (i < inputSize) {
			symbol = pInputBuffer[i++];
			if (symbol != prevSymbol) {
				pOutputBuffer[index++] = symbol;
				prevSymbol = symbol;
			} else {
				runLength = 2;
				break;
			}
		}

		while (i < inputSize) {
			symbol = pInputBuffer[i++];
			if (symbol == prevSymbol) {
				runLength++;
			} else {
				symbolPending = 1;
				break;
			}
		}

		if (runLength > 1) {
			if (runLength < MINIMUM_RUN) {
				while (--runLength) {
					pOutputBuffer[index++] = prevSymbol;
				}
			} else {
				for (uint32 j = 1; j < MINIMUM_RUN; j++) {
					pOutputBuffer[index++] = prevSymbol;
				}

				runLength -= MINIMUM_RUN;
				for (;;) {
					if (runLength >= 128) {
						pOutputBuffer[index++] = (runLength & 127) | 128;
					} else {
						pOutputBuffer[index++] = runLength;
					}

					runLength >>= 7;
					if (runLength == 0) {
						break;
					}
				}

			}
			prevSymbol = symbol;
		}

		if (symbolPending) {
			pOutputBuffer[index++] = symbol;
		}

		if (i == inputSize) {
			break;
		}
	}

	*outputSize = index;
}

void iRLE(const uint8* pInputBuffer, uint8* pOutputBuffer, const uint32 inputSize) {

	uint32 index = 0;
	uint32 prevSymbol = 256;
	uint32 runLength = 0;

	uint32 i = 0;
	while (i < inputSize) {
		uint32 symbol = pInputBuffer[i++];
		pOutputBuffer[index++] = symbol;

		if (symbol != prevSymbol) {
			prevSymbol = symbol;
			runLength = 1;
		}

		else {
			runLength++;
			if (runLength == MINIMUM_RUN) {
				runLength = 0;
				uint32 shift = 0;
				for (;;) {
					symbol = pInputBuffer[i++];
					runLength |= (symbol & 127) << shift;
					shift += 7;

					if (!(symbol & 0x80)) {
						break;
					}
				}
				while (runLength--) {
					pOutputBuffer[index++] = prevSymbol;
				}
			}
		}
	}
}

//----------------------------------------------------------------------------//

}	//	EXTERN "C"

//----------------------------------------------------------------------------//
