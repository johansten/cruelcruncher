
#include "structuredmodel.h"
#include "model.h"
#include "arithcoder.h"

//----------------------------------------------------------------------------//

static uint32 getMSB(uint32 t) {
#ifdef __GNUC__
	return 31 - __builtin_clz(t);
#else
	uint32 msb = 0;
	if (t & 0xffff0000) {t >>= 16; msb += 16;}
	if (t & 0x0000ff00) {t >>=  8; msb +=  8;}
	if (t & 0x000000f0) {t >>=  4; msb +=  4;}
	if (t & 0x0000000c) {t >>=  2; msb +=  2;}
	if (t & 0x00000002) {t >>=  1; msb +=  1;}
	return msb;
#endif
}

//----------------------------------------------------------------------------//

static const uint32 offset[7] = {2, 4, 8, 16, 32, 64, 128};

//----------------------------------------------------------------------------//

StructuredModel::StructuredModel(uint8* pIncrements) {

	static const uint32 numSymbols[8] = {10, 2, 4, 8, 16, 32, 64, 128};

	for (uint32 i = 0; i < 8; i++) {
		m_pModels[i] = new Model(numSymbols[i], pIncrements[i]);
	}
}
//----------------------------------------------------------------------------//


StructuredModel::~StructuredModel() {
	for (uint32 i = 0; i < 8; i++) {
		delete m_pModels[i];
	}
}

//----------------------------------------------------------------------------//

void StructuredModel::getSymbolEntropy(float* pStat, uint32 symbol) {

	if (symbol == 1) {
		pStat[0] +=	m_pModels[0]->getSymbolEntropy(2);
	} else {
		const uint32 model = getMSB(symbol) - 1;
		pStat[0] +=	m_pModels[0]->getSymbolEntropy(model + 3);
		symbol -= offset[model];
		pStat[model + 1] +=	m_pModels[model + 1]->getSymbolEntropy(symbol);
	}
}

//----------------------------------------------------------------------------//

void StructuredModel::getZeroRunEntropy(float* pStat, uint32 length) {

	length++;
	for (int i = getMSB(length) - 1; i >= 0; i--) {
		pStat[0] +=	m_pModels[0]->getSymbolEntropy((length >> i) & 1);
	}
}

//----------------------------------------------------------------------------//

void StructuredModel::encodeSymbol(ArithmeticEncoder& encoder, uint32 symbol) {

	if (symbol ==  1) {
		m_pModels[0]->encodeSymbol(encoder, 2);
	} else {
		uint32 model = getMSB(symbol) - 1;
		m_pModels[0]->encodeSymbol(encoder, model + 3);
		symbol -= offset[model];
		m_pModels[model + 1]->encodeSymbol(encoder, symbol);
	}
}

//----------------------------------------------------------------------------//

void StructuredModel::encodeZeroRun(ArithmeticEncoder& encoder, uint32 length) {

	//
	//	output bits in msb to lsb order in order to unpack more easily
	//

	length++;
	for (int i = getMSB(length) - 1; i >= 0; i--) {
 		m_pModels[0]->encodeSymbol(encoder, (length >> i) & 1);
	}
}

//----------------------------------------------------------------------------//

void StructuredModel::encode(ArithmeticEncoder& encoder, uint8* pBuffer, uint32 length) {

	uint32 zeroRun = 0;
	for (uint32 i = 0; i < length; i++) {
		uint32 symbol = pBuffer[i];
		if (symbol == 0) {
			zeroRun++;
		} else {
			if (zeroRun) {
				encodeZeroRun(encoder, zeroRun);
				zeroRun = 0;
			}
			encodeSymbol(encoder, symbol);
		}
	}

	if (zeroRun) {
		encodeZeroRun(encoder, zeroRun);
	}
}

//----------------------------------------------------------------------------//

void StructuredModel::decode(ArithmeticDecoder& decoder, uint8* pBuffer, uint32 length) {

	uint32 runLength = 1;
	uint32 index = 0;
	while (index < length) {

		uint32 symbol = m_pModels[0]->decodeSymbol(decoder);
		if (symbol < 2) {
			runLength <<= 1;
			runLength  |= symbol;
		}

		else {
			if (runLength > 1) {
				while (--runLength) {
					pBuffer[index++] = 0;
				}
				runLength = 1;
			}

			if (symbol == 2) {
				pBuffer[index++] = 1;
			}
			else {
				symbol -= 3;
				pBuffer[index++] = m_pModels[symbol + 1]->decodeSymbol(decoder) + offset[symbol];
			}
		}
	}
}

//----------------------------------------------------------------------------//
