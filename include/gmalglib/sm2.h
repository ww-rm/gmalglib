#ifndef GMALGLIB_SM2_H
#define GMALGLIB_SM2_H

#include <stdint.h>
#include <gmalglib/bignum.h>
#include <gmalglib/random.h>
#include <gmalglib/sm2curve.h>
#include <gmalglib/sm3.h>

#define SM2_DEFAULT_UID_LENGTH                      16
#define SM2_UID_MAX_LENGTH                          8191  // (0xFFFF >> 3)
#define SM2_ENTITYINFO_LENGTH                       SM3_DIGEST_LENGTH

#define SM2_SK_LENGTH                               SM2_PARAMS_LENGTH
#define SM2_PK_HALF_LENGTH                          SM2_POINTBYTES_HALF_LENGTH
#define SM2_PK_FULL_LENGTH                          SM2_POINTBYTES_FULL_LENGTH
#define SM2_GET_PK_LENGTH(pc_mode)                  SM2_GET_POINTBYTES_LENGTH(pc_mode)

#define SM2_MSG_MAX_LENGTH                          ((SM3_MAX_MSG_BITLEN >> 3) - SM2_ENTITYINFO_LENGTH)
#define SM2_SIGN_R_LENGTH                           SM2_PARAMS_LENGTH
#define SM2_SIGN_S_LENGTH                           SM2_PARAMS_LENGTH
#define SM2_SIGNATURE_LENGTH                        (SM2_SIGN_R_LENGTH + SM2_SIGN_S_LENGTH)

#define SM2_ENCRYPT_C1_HALF_LENGTH                  SM2_POINTBYTES_HALF_LENGTH
#define SM2_ENCRYPT_C1_FULL_LENGTH                  SM2_POINTBYTES_FULL_LENGTH
#define SM2_GET_ENCRYPT_C1_LENGTH(pc_mode)          SM2_GET_POINTBYTES_LENGTH(pc_mode)
#define SM2_ENCRYPT_C3_LENGTH                       SM3_DIGEST_LENGTH
#define SM2_GET_ENCRYPT_HEADER_LENGTH(pc_mode)      (SM2_GET_ENCRYPT_C1_LENGTH(pc_mode) + SM2_ENCRYPT_C3_LENGTH)

#define SM2_ERR_UID_OVERFLOW            -1
#define SM2_ERR_INVALID_SK              -2
#define SM2_ERR_INVALID_PK              -3
#define SM2_ERR_MSG_OVERFLOW            -4
#define SM2_ERR_NEED_SK                 -5
#define SM2_ERR_RANDOM_FAILED           -6
#define SM2_ERR_INVALID_SIGN            -7
#define SM2_ERR_NEED_PK                 -8
#define SM2_ERR_NO_MEMORY               -9
#define SM2_ERR_DATA_OVERFLOW           -10
#define SM2_ERR_INVALID_C1              -11
#define SM2_ERR_INVALID_C3              -12
#define SM2_ERR_INVALID_CIPHER          -13

typedef UInt256 SM2ModN;
typedef SM2ModN SM2ModNMont;

typedef struct _SM2 {
    int has_sk;
    SM2ModN sk;

    int has_pk;
    SM2JacobPointMont pk;

    int pc_mode;
    RandomAlg rand_alg;

    SM2ModNMont sk_modn_mont;
    SM2ModNMont inv_1_plus_sk_modn_mont;        // used for sign
    uint8_t entity_info[SM2_ENTITYINFO_LENGTH];
} SM2;

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t* const SM2_DEFAULT_UID;

int SM2_IsSkValid(const uint8_t* sk);
int SM2_IsPkValid(const uint8_t* pk, uint64_t pk_len);
int SM2_GetPk(const uint8_t* sk, uint8_t* pk, int pc_mode);

int SM2_Init(SM2* self, const uint8_t* sk, const uint8_t* pk, uint64_t pk_len, const uint8_t* uid, uint64_t uid_len, int pc_mode, RandomAlg* rand_alg);
int SM2_GenerateKeyPair(SM2* self, uint8_t* sk, uint8_t* pk);
int SM2_GetEntityInfo(SM2* self, uint8_t* entity_info);
int SM2_SignDigest(SM2* self, const uint8_t* digest, uint8_t* signature);
int SM2_VerifyDigest(SM2* self, const uint8_t* digest, const uint8_t* signature);
int SM2_Sign(SM2* self, const uint8_t* msg, uint64_t msg_len, uint8_t* signature);
int SM2_Verify(SM2* self, const uint8_t* msg, uint64_t msg_len, const uint8_t* signature);
int SM2_Encrypt(SM2* self, const uint8_t* plain, uint64_t plain_len, uint8_t* cipher);
int SM2_Decrypt(SM2* self, const uint8_t* cipher, uint64_t cipher_len, uint8_t* plain, uint64_t* plain_len);

#ifdef __cplusplus
}
#endif

#endif // !GMALGLIB_SM2_H
