
#ifndef COMPRESS_H
#define COMPRESS_H

//----------------------------------------------------------------------------//
//		FORWARD DECLARATIONS
//----------------------------------------------------------------------------//

class OutputStream;
class InputBuffer;

//----------------------------------------------------------------------------//

void compress(const char* pInputFilename, const char* pOutputFilename);
void compress(InputBuffer& input, OutputStream& outputStream);

//----------------------------------------------------------------------------//

#endif	//	COMPRESS_H
