
//----------------------------------------------------------------------------//

#include "model.h"
#include "arithcoder.h"
#include <math.h>

//----------------------------------------------------------------------------//

static const uint32 climit = 8192;
static const float cOneOverLog2 = 1.0 / log(2.0f);

//----------------------------------------------------------------------------//

Model::Model(uint32 numSymbols, uint32 increment) {
	m_numSymbols = numSymbols;
	m_total = numSymbols;
	m_increment = increment;

	m_pCumProbs = new uint32[numSymbols + 1];
	for (uint32 i = 0; i <= numSymbols; i++) {
		m_pCumProbs[i] = i;
	}
}

void Model::update(uint32 symbol) {

	//	increase "hi" for all symbols above symbol

	for (uint32 i = symbol; i < m_numSymbols; i++) {
		m_pCumProbs[i+1] += m_increment;
	}
	m_total += m_increment;

	//	if total cum prob is above limit, cut everything in half
	//	(but be sure that each symbol has a non-zero prob)

	if (m_total > climit) {

		uint32 lo = 0;
		for (uint32 i = 0; i < m_numSymbols; i++) {
			uint32 hi = m_pCumProbs[i + 1];
			hi >>= 1;
			if (lo >= hi) {
				hi = lo + 1;
			}
			m_pCumProbs[i + 1] = hi;
			lo = hi;
		}
		m_total = lo;
	}
}

float Model::getSymbolEntropy(uint32 symbol) {
	const float range  = m_pCumProbs[symbol + 1] - m_pCumProbs[symbol];
	const float result = -log(range / m_total) * cOneOverLog2;
	update(symbol);
	return result;
}

void Model::encodeSymbol(ArithmeticEncoder& encoder, uint32 symbol) {

	// find lo, hi, total
	const uint32 lo = m_pCumProbs[symbol];
	const uint32 hi = m_pCumProbs[symbol + 1];
	encoder.update(lo, hi, m_total);

	//	update model
	update(symbol);
}

uint32 Model::decodeSymbol(ArithmeticDecoder& decoder) {

	//	get the encoded probability within the current range
	const uint32 prob = decoder.getCurrentProbability(m_total);

	//	find the corresponding symbol, w/ lo <= prob < hi

	uint32 symbol;
	for (uint32 i = m_numSymbols - 1; i >= 0; i--) {

		if ((m_pCumProbs[i] <= prob) && (prob < m_pCumProbs[i+1])) {
			symbol = i;
			break;
		}
	}

	//	update decoder state

	const uint32 lo = m_pCumProbs[symbol];
	const uint32 hi = m_pCumProbs[symbol + 1];
	decoder.update(lo, hi, m_total);

	//	update model

	update(symbol);
	return symbol;
}

//----------------------------------------------------------------------------//
