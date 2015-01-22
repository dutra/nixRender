#ifndef TYPES_H
#define TYPES_H

// Explicitly-sized versions of integer types
typedef __signed char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;

typedef uint8 bool;
#define true 1;
#define false 0;

// size is used for memory object sizes.
typedef uint32 size;

#endif /* TYPES_H */
