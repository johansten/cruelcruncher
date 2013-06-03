#ifndef HEADER_H
#define HEADER_H

#include "types.h"

struct header {
	uint32	first;
	uint8	increments[8];
	uint32	fileSize;
	uint32	tempSize;
};

#endif	//	HEADER_H
