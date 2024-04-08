#ifndef GMALGLIB_PRIMEFIELD_H
#define GMALGLIB_PRIMEFIELD_H

#include <stdint.h>

typedef uint8_t uint256_t[32];
typedef uint8_t uint512_t[64];

typedef uint256_t mont256_t;

#ifdef __cplusplus
extern "C" {
#endif

uint8_t U256_Add(const uint256_t* x, const uint256_t* y, uint256_t* z);

uint8_t U256_Sub(const uint256_t* x, const uint256_t* y, uint256_t* z);

void U256_Mul(const uint256_t* x, const uint256_t* y, uint512_t* z);

uint8_t U512_Add(const uint512_t* x, const uint512_t* y, uint512_t* z);




#ifdef __cplusplus
}
#endif

#endif // !GMALGLIB_PRIMEFIELD_H
