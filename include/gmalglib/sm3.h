#ifndef GMALGLIB_SM3_H
#define GMALGLIB_SM3_H

#include <stdint.h>

#define SM3_MAX_MSG_BITLEN      0xffffffffffffffffULL  // (1 << 64 - 1)
#define SM3_DIGEST_LENGTH       32

#define SM3_ERR_OVERFLOW        -1

typedef struct _SM3 {
    uint32_t value[8];
    uint8_t msg_buffer[64];
    uint64_t msg_buffer_length;
    uint64_t msg_bitlen;
} SM3;

#ifdef __cplusplus
extern "C" {
#endif

void SM3_Init(SM3* self);

int SM3_Update(SM3* self, const uint8_t* data, uint64_t data_len);

void SM3_Digest(SM3* self, uint8_t* digest);

#ifdef __cplusplus
}
#endif

#endif // !GMALGLIB_SM3_H
