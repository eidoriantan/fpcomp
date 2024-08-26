#ifndef _COMPARE_H
#define _COMPARE_H

#include <stdint.h>

#define MATCH_BITS 14
#define MATCH_MASK ((1 << MATCH_BITS) - 1)
#define MATCH_STRIP(x) ((uint32_t)(x) >> (32 - MATCH_BITS))

#define UNIQ_MASK ((1 << MATCH_BITS) - 1)
#define UNIQ_STRIP(x) ((uint32_t)(x) >> (32 - MATCH_BITS))

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

float match_fingerprints (signed int*, int, signed int*, int, int);

#endif
