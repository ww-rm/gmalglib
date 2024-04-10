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
    UInt256 u256[2];
} UInt512;


#ifdef __cplusplus
extern "C" {
#endif

void UInt256_ToBytes(const UInt256* x, uint8_t* bytes);

void UInt256_FromBytes(const uint8_t* bytes, UInt256* x);

int UInt256_Cmp(const UInt256* x, const UInt256* y);

int UInt256_IsZero(const UInt256* x);

uint8_t UInt256_Add(const UInt256* x, const UInt256* y, UInt256* z);

uint8_t UInt256_Sub(const UInt256* x, const UInt256* y, UInt256* z);

void UInt256_Mul(const UInt256* x, const UInt256* y, UInt512* z);

uint8_t UInt512_Add(const UInt512* x, const UInt512* y, UInt512* z);

#ifdef _DEBUG

void UInt256_Print(const UInt256* x, int sep);

#endif // _DEBUG


#ifdef __cplusplus
}
#endif

#endif // !GMALGLIB_BIGNUM_H
