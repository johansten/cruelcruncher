
#ifndef TYPES_H
#define TYPES_H

typedef unsigned long long int uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

#define STATIC_ASSERT( condition, name )\
    typedef char assert_failed_ ## name [ (condition) ? 1 : -1 ];

STATIC_ASSERT(sizeof(uint64) == 8, sizeof_uint64);
STATIC_ASSERT(sizeof(uint32) == 4, sizeof_uint32);
STATIC_ASSERT(sizeof(uint16) == 2, sizeof_uint16);
STATIC_ASSERT(sizeof(uint8)  == 1, sizeof_uint8);

#endif //TYPES_H
