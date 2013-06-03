#ifndef	STRUCTUREDMODEL_H
#define	STRUCTUREDMODEL_H

//----------------------------------------------------------------------------//

#include "types.h"

//----------------------------------------------------------------------------//
//		FORWARD DECLARATIONS
//----------------------------------------------------------------------------//

class Model;
class ArithmeticEncoder;
class ArithmeticDecoder;

//----------------------------------------------------------------------------//

class StructuredModel {

public:

	StructuredModel(uint8* pIncrements);
	~StructuredModel();
	void getSymbolEntropy(float* pStat, uint32 symbol);
	void getZeroRunEntropy(float* pStat, uint32 length);
	void encode(ArithmeticEncoder& encoder, uint8* pBuffer, uint32 length);
	void decode(ArithmeticDecoder& decoder, uint8* pBuffer, uint32 length);

//private:

	void encodeSymbol(ArithmeticEncoder& encoder, uint32 symbol);
	void encodeZeroRun(ArithmeticEncoder& encoder, uint32 length);

private:

	Model* m_pModels[8];

	//	z0, z1, 1, m1, m2, m3, m4, m5, m6, m7
	//	2-3
	//	4-7
	//	8-15
	//	16-31
	//	32-63
	//	64-127
	//	128-255
};

//----------------------------------------------------------------------------//

#endif	//	STRUCTUREDMODEL_H
