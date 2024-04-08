#ifndef GMALGLIB_BIGNUM_H
#define GMALGLIB_BIGNUM_H

#include <stdint.h>

typedef uint8_t UInt256[32];
typedef uint8_t UInt512[64];

#ifdef __cplusplus
extern "C" {
#endif

int UInt256_Cmp(const UInt256* x, const UInt256* y);

uint8_t UInt256_Add(const UInt256* x, const UInt256* y, UInt256* z);

uint8_t UInt256_Sub(const UInt256* x, const UInt256* y, UInt256* z);

void UInt256_Mul(const UInt256* x, const UInt256* y, UInt512* z);

uint8_t UInt512_Add(const UInt512* x, const UInt512* y, UInt512* z);

#ifdef __cplusplus
}
#endif

#endif // !GMALGLIB_BIGNUM_H
