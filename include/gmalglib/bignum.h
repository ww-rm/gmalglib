#ifndef GMALGLIB_BIGNUM_H
#define GMALGLIB_BIGNUM_H

#include <stdint.h>

typedef union _UInt256
{
    uint8_t u8[32];
    uint32_t u32[8];
    uint64_t u64[4];
} UInt256;

typedef union _UInt512
{
    uint8_t u8[64];
    uint32_t u32[16];
    uint64_t u64[8];
} UInt512;


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
