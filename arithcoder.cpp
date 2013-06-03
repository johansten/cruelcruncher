
#include "arithcoder.h"
#include "inputstream.h"
#include "outputstream.h"
#include "types.h"

#include <stdio.h>
#define ASSERT(x)

//----------------------------------------------------------------------------//

#define NUM_BITS 32
#define ONE (1ULL << NUM_BITS)
#define HALF (1ULL << (NUM_BITS - 1))
#define QUARTER (1ULL << (NUM_BITS - 2))

//----------------------------------------------------------------------------//

ArithmeticEncoder::ArithmeticEncoder(OutputStream& os) : m_os(os) {
	m_lo = 0;
	m_hi = ONE - 1; 
	m_underflow = 0;
}

ArithmeticEncoder::~ArithmeticEncoder() {
	flush();
}

void ArithmeticEncoder::flush() {
	m_os.outputBits((m_lo >> (NUM_BITS - 2)) & 1, 1);
	m_underflow++;

	while (m_underflow) {
		m_os.outputBits(((m_lo >> (NUM_BITS - 2)) ^ 1) & 1, 1);
		m_underflow--;
	}
}

void ArithmeticEncoder::update(uint32 lo, uint32 hi, uint32 total) {
	const uint64 range = (m_hi - m_lo) + 1;
	m_hi = m_lo + (range * hi / total) - 1;
	m_lo = m_lo + (range * lo / total);

	for (;;) {

		if ((m_hi & HALF) == (m_lo & HALF)) {

#if 1
			ASSERT(m_underflow < 32);

			//	write the whole pattern at once:

			//	for e.g. m_underflow == 7, we have
			//		bit: 1	->	10000000
			//		bit: 0	->	01111111

			const uint32 bit = m_hi >> (NUM_BITS - 1);
			const uint32 pattern = (1 << m_underflow) - (bit ^ 1);
			m_os.outputBits(pattern, m_underflow + 1);
			m_underflow = 0;
#else
			const uint32 bit = m_hi >> (NUM_BITS - 1);
			m_os.outputBits(bit, 1);
			while (m_underflow) {
				m_os.outputBits(bit ^ 1, 1);
				m_underflow--;
			}
#endif
		}

		else {
			if ((m_lo & QUARTER) && !(m_hi & QUARTER)) {
				m_underflow++;

				m_lo &= QUARTER - 1;
				m_hi |= QUARTER;
			}

			else {
				return;
			}
		}

		m_lo <<= 1;
		m_lo &= ONE - 1;

		m_hi <<= 1;
		m_hi += 1;
		m_hi &= ONE - 1;
	}
}

//----------------------------------------------------------------------------//

ArithmeticDecoder::ArithmeticDecoder(InputStream& is) : m_is(is){
	m_lo = 0;
	m_hi = ONE - 1; 
	m_code = m_is.inputBits(NUM_BITS);
}

unsigned int ArithmeticDecoder::getCurrentProbability(uint32 total) {
	const uint64 range = (m_hi - m_lo) + 1;
	return ((((m_code - m_lo) + 1)  * total) - 1) / range;
}

void ArithmeticDecoder::update(uint32 lo, uint32 hi, uint32 total) {
	const uint64 range = (m_hi - m_lo) + 1;
	m_hi = m_lo + (range * hi / total) - 1;
	m_lo = m_lo + (range * lo / total);

	for (;;) {

		if ((m_hi & HALF) == (m_lo & HALF)) {
		}

		else {
			if ((m_lo & QUARTER) && !(m_hi & QUARTER)) {
				m_code	^= QUARTER;
				m_lo 	&= QUARTER - 1;
				m_hi	|= QUARTER;
			}

			else {
				return;
			}
		}

		m_lo <<= 1;
		m_lo  &= ONE - 1;

		m_hi <<= 1;
		m_hi  |= 1;
		m_hi  &= ONE - 1;

		m_code <<= 1;
		m_code  |= m_is.inputBits(1);
		m_code  &= ONE - 1;
	}
}

//----------------------------------------------------------------------------//

#undef NUM_BITS
#undef ONE
#undef HALF
#undef QUARTER

//----------------------------------------------------------------------------//
