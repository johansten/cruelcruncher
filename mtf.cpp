
#include "mtf.h"

//----------------------------------------------------------------------------//

extern "C" {

//----------------------------------------------------------------------------//

void MTF(uint8* pBuffer, const uint32 length) {

	uint8 order[256];
	for (uint32 i = 0; i < 256; i++) {
		order[i] = i;
	}

	for (uint32 i = 0; i < length; i++) {
		uint32 symbol = pBuffer[i];

		uint32 pos;
		for (pos = 0; pos < 256; pos++) {
			if (order[pos] == symbol) {
				break;
			}
		}
		pBuffer[i] = pos;

		for (uint32 j = pos; j > 0; j--) {
			order[j] = order[j-1];
		}
		order[0] = symbol;
	}
}

//----------------------------------------------------------------------------//

void iMTF(uint8* pBuffer, const uint32 length) {

	uint8 order[256];
	for (uint32 i = 0; i < 256; i++) {
		order[i] = i;
	}

	for (uint32 i = 0; i < length; i++) {
		uint32 pos = pBuffer[i];
		uint32 symbol = order[pos];
		pBuffer[i] = symbol;

		for (uint32 j = pos; j > 0; j--) {
			order[j] = order[j-1];
		}
		order[0] = symbol;
	}
}

//----------------------------------------------------------------------------//

}	//	EXTERN "C"

//----------------------------------------------------------------------------//
