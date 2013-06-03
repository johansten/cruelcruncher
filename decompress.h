
#ifndef DECOMPRESS_H
#define DECOMPRESS_H

//----------------------------------------------------------------------------//

#include "types.h"

//----------------------------------------------------------------------------//
//		FORWARD DECLARATIONS
//----------------------------------------------------------------------------//

class InputStream;
class OutputBuffer;
struct header;

//----------------------------------------------------------------------------//

void readHeader(InputStream& is, header& h);
void decompress(const char* pInputFilename, const char* pOutputFilename);
void decompress(InputStream& inputStream, OutputBuffer& output, header* pHeader = 0);

//----------------------------------------------------------------------------//

#endif	//	DECOMPRESS_H
