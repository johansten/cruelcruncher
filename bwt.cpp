
#include "bwt.h"
#include "types.h"

#include <stdio.h>
#include <string.h>
#include <stack>

//----------------------------------------------------------------------------//

static void sort3(const uint8* pInput, uint32* pIndex, int left, int right, uint32 offset, uint32 length);

//----------------------------------------------------------------------------//

extern "C" {

//----------------------------------------------------------------------------//

void BWT(const unsigned char* pInput, unsigned char* pOutput, const uint32 length, uint32* pFirst) {

	uint32* pSuffixArray = new uint32[length];

	//
	//	calculate the symbol counts
	//

	uint32 counts[256];
	uint32 offsets[256];
	for (uint32 i = 0; i < 256; i++) {
		counts[i] = 0;
	}

	for (uint32 i = 0; i < length; i++) {
		counts[pInput[i]]++;
	}

	//
	//	transform "counts" into symbol offsets
	//

	uint32 sum = 0;
	for (uint32 i = 0; i < 256; i++) {
		const uint32 curr = counts[i];
		offsets[i] = sum;
		counts[i] = 0;
		sum += curr;
	}

	//
	//	use the symbol offsets to sort indices into buckets
	//	based on their starting symbol
	//

	for (uint32 i = 0; i < length; i++) {
		uint32 symbol = pInput[i];
		pSuffixArray[offsets[symbol] + counts[symbol]] = i;
		counts[symbol]++;
	}

	//
	//	now that the indices are all sorted in radix-256 order
	//	we quicksort each bucket independently
	//

	for (uint32 i = 0; i < 256; i++) {
		if (counts[i] >= 2) {
			sort3(pInput, pSuffixArray, offsets[i], offsets[i] + counts[i] - 1, 1, length);
		}
	}

	//
	//	for each string, output the last symbol
	//

	uint32 j = 0;
	for (uint32 i = 0; i < length; i++) {

		uint32 index = pSuffixArray[i];
		if (index == 0) {
			index = length;
			*pFirst = j;
		}
		pOutput[j++] = pInput[index - 1];
	}

	delete[] pSuffixArray;
}

//----------------------------------------------------------------------------//

void iBWT(const unsigned char* pInput, unsigned char* pOutput, const uint32 length, const uint32 first) {

	uint32* pLinks = new uint32[length];

	//
	//	calculate the symbol counts
	//

	uint32 counts[256];
	for (uint32 i = 0; i < 256; i++) {
		counts[i] = 0;
	}

	for (uint32 i = 0; i < length; i++) {
		counts[pInput[i]]++;
	}

	//
	//	transform "counts" into symbol offsets
	//

	uint32 sum = 0;
	for (uint32 i = 0; i < 256; i++) {
		const uint32 curr = counts[i];
		counts[i] = sum;
		sum += curr;
	}

	for (uint32 i = 0; i < length; i++) {
		uint32 symbol = pInput[i];
		pLinks[counts[symbol]] = i;
		counts[symbol]++;
	}

	uint32 index = first;
	for (uint32 i = 0; i < length; i++) {
		index = pLinks[index];
		pOutput[i] = pInput[index];
	}

	delete[] pLinks;
}

//----------------------------------------------------------------------------//

}	//	EXTERN "C"

//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//		Terniary Quicksort
//
//----------------------------------------------------------------------------//

static inline void swap(uint32* pIndex, uint32 a, uint32 b) {
	const uint32 temp = pIndex[a];
	pIndex[a] = pIndex[b];
	pIndex[b] = temp;
}

static void sort3(const uint8* pInput, uint32* pIndex, int left, int right, uint32 offset, const uint32 length) {

#define PUSH(a, b, c) { \
	stack.push(c); \
	stack.push(b); \
	stack.push(a); \
}

#define POP(a, b, c) { \
	a = stack.top(); stack.pop(); \
	b = stack.top(); stack.pop(); \
	c = stack.top(); stack.pop(); \
}

	std::stack<int> stack;
	for (;;) {

		int i, j, k;
		i = left;
		k = right;

		//
		//		use median as pivot element, if more than three items
		//

		uint32 pivot;
		if ((k - i + 1) > 3) {
			uint32 p0 = pInput[(pIndex[left+0] + offset) % length];
			uint32 p1 = pInput[(pIndex[left+1] + offset) % length];
			uint32 p2 = pInput[(pIndex[left+2] + offset) % length];

			if (p1 > p2) {uint32 t = p1; p1 = p2; p2 = t;}
			if (p0 > p2) {uint32 t = p0; p0 = p2; p2 = t;}
			if (p0 > p1) {uint32 t = p0; p0 = p1; p1 = t;}
			pivot = p1;
		} else {
			pivot = pInput[(pIndex[left] + offset) % length];
		}

		//
		//		partition into three sets (<, ==, >)
		//

		j = i;
		while (j <= k) {
			uint32 item = pInput[(pIndex[j] + offset) % length];

			if (item < pivot) {
				swap(pIndex, i++, j++);
			}

			else if (item > pivot) {
				swap(pIndex, j, k--);
			}

			else {
				j++;
			}
		}

		//
		//		push sets on stack
		//

		if (left < (i - 1)) {
			PUSH(left, i - 1, offset);		//	<
		}

		if (i < k) {
			PUSH(i, k, offset + 1);			//	==
		}

		if ((k + 1) < right) {
			PUSH(k + 1, right, offset);		//	>
		}

		if (!stack.empty()) {
			POP(left, right, offset);
		} else {
			break;
		}
	}

#undef PUSH
#undef POP

}

//----------------------------------------------------------------------------//
