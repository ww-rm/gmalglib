#ifndef GMALGLIB_SM2_H
#define GMALGLIB_SM2_H

#include <stdint.h>
#include <gmalglib/random.h>
#include <gmalglib/sm2curve.h>
#include <gmalglib/sm3.h>

#define SM2_UID_MAX_LENGTH          8192  // (1 << 16 >> 3)
#define SM2_MSG_MAX_LENGTH          ((SM3_MAX_MSG_BITLEN >> 3) - SM3_DIGEST_LENGTH)

#define SM2_DEFAULT_UID_LENGTH      16

#define SM2_ERR_UID_OVERFLOW        -1
#define SM2_ERR_MSG_OVERFLOW        -2
#define SM2_ERR_INVALID_PCMODE      -3

typedef struct _SM2 {
    SM2ModNMont sk;
    int has_sk;

    SM2JacobPointMont pk;
    int has_pk;

    uint8_t entity_info[SM3_DIGEST_LENGTH];
    int has_entity_info;

    int pc_mode;
    RandomBytesFunction rand_func;
    void* rand_obj;

} SM2;

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t* const SM2_DEFAULT_UID;

int SM2_EntityInfo(const uint8_t* uid, uint16_t* uid_len, const uint8_t* pk, uint8_t* entity_info);

int SM2_GetPk(SM2* self, uint8_t* pk);

int SM2_GenerateKeyPair(SM2* self, uint8_t* sk, uint8_t* pk);

int SM2_Init(
    SM2* self, 
    const uint8_t* sk,
    const uint8_t* pk,
    const uint8_t* uid,
    uint16_t uid_len, 
    int pc_mode, 
    RandomBytesFunction rand_func, 
    void* rand_obj
);


int SM2_Sign();




#ifdef __cplusplus
}
#endif

#endif // !GMALGLIB_SM2_H
