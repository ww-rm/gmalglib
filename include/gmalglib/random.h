#ifndef GMALGLIB_RANDOM_H
#define GMALGLIB_RANDOM_H

#include <stdint.h>
#include <gmalglib/bignum.h>

// return boolean value
typedef int (*RandomProc)(void* rand_obj, uint64_t bytes_len, uint8_t* bytes);

typedef struct _RandomAlg {
    RandomProc rand_proc;
    void* rand_obj;
} RandomAlg;


#ifdef __cplusplus
extern "C" {
#endif

extern const RandomAlg* const DEFAULT_RANDOM_ALGORITHM;

// return specified length bytes
int RandomBytes(RandomAlg* self, uint64_t bytes_len, uint8_t* bytes);

// return a big num between 1 and max, including both ending
int RandomUInt256(RandomAlg* self, const UInt256* max, UInt256* num);

#ifdef __cplusplus
}
#endif

#endif // !GMALGLIB_RANDOM_H
