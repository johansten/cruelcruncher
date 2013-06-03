
#ifndef BWT_H
#define BWT_H

//----------------------------------------------------------------------------//

#include "types.h"

//----------------------------------------------------------------------------//

#ifdef	__cplusplus
extern "C" {
#endif	//__cplusplus

//----------------------------------------------------------------------------//

void johanBWT(const uint8* pInput, uint8* pOutput, uint32* pFirst, const uint32 length);
void BWT(const uint8* pInput, uint8* pOutput, const uint32 length, uint32* pFirst);
void iBWT(const uint8* pInput, uint8* pOutput, const uint32 length, const uint32 first);

//----------------------------------------------------------------------------//

#ifdef	__cplusplus
}
#endif	//__cplusplus

//----------------------------------------------------------------------------//

#endif //BWT_H
