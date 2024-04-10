#ifndef GMALGLIB_SM2_H
#define GMALGLIB_SM2_H

#include <stdint.h>
#include <gmalglib/bignum.h>
#include <gmalglib/random.h>
#include <gmalglib/sm2curve.h>
#include <gmalglib/sm3.h>

#define SM2_UID_MAX_LENGTH              8192  // (1 << 16 >> 3)
#define SM2_ENTITYINFO_LENGTH           SM3_DIGEST_LENGTH
#define SM2_MSG_MAX_LENGTH              ((SM3_MAX_MSG_BITLEN >> 3) - SM2_ENTITYINFO_LENGTH)
#define SM2_SK_LENGTH                   SM2_PARAMS_LENGTH
#define SM2_PK_MAX_LENGTH               SM2_POINTBYTES_MAX_LENGTH
#define SM2_SIGN_R_LENGTH               SM2_PARAMS_LENGTH
#define SM2_SIGN_S_LENGTH               SM2_PARAMS_LENGTH

#define SM2_DEFAULT_UID_LENGTH          16

#define SM2_ERR_UID_OVERFLOW            -1
#define SM2_ERR_INVALID_SK              -2
#define SM2_ERR_INVALID_PK              -3
#define SM2_ERR_MSG_OVERFLOW            -4
#define SM2_ERR_NEED_SK                 -5
#define SM2_ERR_RANDOM_FAILED           -6


typedef UInt256 SM2ModN;
typedef SM2ModN SM2ModNMont;

typedef struct _SM2 {
    UInt256 sk;
    SM2ModNMont sk_modn_mont;
    SM2ModNMont inv_1_plus_sk_modn_mont; // used for sign
    int has_sk;

    SM2JacobPointMont pk;
    int has_pk;

    uint8_t entity_info[SM2_ENTITYINFO_LENGTH];
    int has_entity_info;

    int pc_mode;
    RandomAlg rand_alg;
} SM2;

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t* const SM2_DEFAULT_UID;

int SM2_Init(SM2* self, const uint8_t* sk, const uint8_t* pk, const uint8_t* uid, uint16_t uid_len, int pc_mode, RandomAlg* rand_alg);

int SM2_SignDigest(SM2* self, const uint8_t* digest, uint8_t* r, uint8_t* s);




#ifdef __cplusplus
}
#endif

#endif // !GMALGLIB_SM2_H
