
#ifndef ARITHCODER_H
#define ARITHCODER_H

//----------------------------------------------------------------------------//

#include "types.h"

//----------------------------------------------------------------------------//
//		FORWARD DECLARATIONS
//----------------------------------------------------------------------------//

class OutputStream;
class InputStream;

//----------------------------------------------------------------------------//

class ArithmeticEncoder {

public:

	ArithmeticEncoder(OutputStream& os);
	~ArithmeticEncoder();
	void flush();
	void update(uint32 lo, uint32 hi, uint32 total);

private:

	OutputStream&	m_os;
	uint64			m_lo;
	uint64			m_hi;
	uint32			m_underflow;
};

//----------------------------------------------------------------------------//

class ArithmeticDecoder {

public:

	ArithmeticDecoder(InputStream& is);
	unsigned int getCurrentProbability(uint32 total);
	void update(uint32 lo, uint32 hi, uint32 total);

private:

	InputStream&	m_is;
	uint64			m_lo;
	uint64			m_hi;
	uint64			m_code;
};

//----------------------------------------------------------------------------//

#endif //ARITHCODER_H
