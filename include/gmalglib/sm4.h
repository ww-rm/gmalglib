#ifndef GMALGLIB_SM4_H
#define GMALGLIB_SM4_H

#include <stdint.h>

#define SM4_KEY_LENGTH      16
#define SM4_BLOCK_LENGTH    16

typedef struct _SM4 {
    uint32_t round_key[32];
} SM4;

#ifdef __cplusplus
extern "C" {
#endif

void SM4_Init(SM4* self, const uint8_t* key);

void SM4_Encrypt(SM4* self, const uint8_t* block_in, uint8_t* block_out);

void SM4_Decrypt(SM4* self, const uint8_t* block_in, uint8_t* block_out);

#ifdef __cplusplus
}
#endif

#endif // !GMALGLIB_SM4_H
