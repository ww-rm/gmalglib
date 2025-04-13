#ifndef GMALGLIB_SM9_H
#define GMALGLIB_SM9_H

#include <stdint.h>
#include <gmalglib/bignum.h>
#include <gmalglib/random.h>
#include <gmalglib/sm9curve.h>
#include <gmalglib/sm3.h>

#define SM9_ERR_INVALID_MSK             -1
#define SM9_ERR_INVALID_MPK             -2
#define SM9_ERR_RANDOM_FAILED           -3
#define SM9_ERR_NEED_HID                -4
#define SM9_ERR_NEED_MSK                -5

typedef UInt256 SM9ModN;
typedef SM9ModN SM9ModNMont;

typedef struct _SM9KGC {
    int has_msk_s;
    SM9ModN msk_s;
    SM9ModNMont msk_s_mont;

    int has_mpk_s;
    SM9JacobPoint2Mont mpk_s;

    int has_hid_s;
    uint8_t hid_s;

    int has_msk_e;
    SM9ModN msk_e;
    SM9ModNMont msk_e_mont;

    int has_mpk_e;
    SM9JacobPoint1Mont mpk_e;
    
    int has_hid_e;
    uint8_t hid_e;

    int pc_mode;
    RandomAlg rand_alg;
} SM9KGC;

typedef struct _SM9 {
    int pc_mode;
    RandomAlg rand_alg;
} SM9;

#ifdef __cplusplus
extern "C" {
#endif

int SM9KGC_IsMskForSigValid(const uint8_t* msk);
int SM9KGC_IsMskForEncValid(const uint8_t* msk);

int SM9KGC_IsMpkForSigValid(const uint8_t* mpk, uint64_t mpk_len);
int SM9KGC_IsMpkForEncValid(const uint8_t* mpk, uint64_t mpk_len);

int SM9KGC_IsMasterKeyPairForSig(const uint8_t* msk, const uint8_t* mpk, uint64_t mpk_len);
int SM9KGC_IsMasterKeyPairForEnc(const uint8_t* msk, const uint8_t* mpk, uint64_t mpk_len);

int SM9KGC_GetMpkForSig(const uint8_t* msk, int pc_mode, uint8_t* mpk);
int SM9KGC_GetMpkForEnc(const uint8_t* msk, int pc_mode, uint8_t* mpk);

int SM9KGC_ConvertMpkForSig(const uint8_t* mpk, uint64_t mpk_len, int pc_mode, uint8_t* mpk_converted);
int SM9KGC_ConvertMpkForEnc(const uint8_t* mpk, uint64_t mpk_len, int pc_mode, uint8_t* mpk_converted);

int SM9KGC_Init(
    SM9KGC* self,
    const uint8_t* hid_s, const uint8_t* msk_s, const uint8_t* mpk_s, uint64_t mpk_s_len,
    const uint8_t* hid_e, const uint8_t* msk_e, const uint8_t* mpk_e, uint64_t mpk_e_len,
    int pc_mode, RandomAlg* rand_alg
);

int SM9KGC_GenerateMasterKeyPairForSig(SM9KGC* self, uint8_t* msk, uint8_t* mpk);
int SM9KGC_GenerateMasterKeyPairForEnc(SM9KGC* self, uint8_t* msk, uint8_t* mpk);

int SM9KGC_GenerateSkForSig(SM9KGC* self, uint8_t* uid, uint64_t uid_len, uint8_t* sk);
int SM9KGC_GenerateSkForEnc(SM9KGC* self, uint8_t* uid, uint64_t uid_len, uint8_t* sk);

#ifdef __cplusplus
}
#endif

#endif // !GMALGLIB_SM9_H
