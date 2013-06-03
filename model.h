#ifndef	MODEL_H
#define	MODEL_H

//----------------------------------------------------------------------------//

#include "types.h"

//----------------------------------------------------------------------------//

class ArithmeticEncoder;
class ArithmeticDecoder;

//----------------------------------------------------------------------------//

class Model {

public:

	Model(uint32 numSymbols, uint32 increment);
	void update(uint32 symbol);
	float getSymbolEntropy(uint32 symbol);
	void encodeSymbol(ArithmeticEncoder& encoder, uint32 symbol);
	uint32 decodeSymbol(ArithmeticDecoder& decoder);

private:

	uint32*	m_pCumProbs;
	uint32	m_numSymbols;
	uint32	m_total;
	uint32	m_increment;
};

//----------------------------------------------------------------------------//

#endif	//	MODEL_H