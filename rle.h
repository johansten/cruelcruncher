
#ifndef RLE_H
#define RLE_H

//----------------------------------------------------------------------------//

#include "types.h"

//----------------------------------------------------------------------------//

#ifdef	__cplusplus
extern "C" {
#endif	//__cplusplus

//----------------------------------------------------------------------------//

void iRLE(const uint8* pInputBuffer, uint8* pOutputBuffer, const uint32 inputSize);
void RLE(const uint8* pInputBuffer, uint8* pOutputBuffer, const uint32 inputSize, uint32* outputSize);
void johanRLE(const uint8* pInputBuffer, uint8* pOutputBuffer, const uint32 inputSize, uint32* outputSize);

//----------------------------------------------------------------------------//

#ifdef	__cplusplus
}
#endif	//__cplusplus

//----------------------------------------------------------------------------//

#endif //RLE_H
