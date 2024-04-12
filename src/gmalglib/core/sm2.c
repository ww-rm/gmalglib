#include <stdint.h>
#include <gmalglib/random.h>
#include <gmalglib/sm2curve.h>
#include <gmalglib/sm3.h>
#include <gmalglib/sm2.h>

static const uint8_t _SM2_DEFAULT_UID[SM2_DEFAULT_UID_LENGTH] = {
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
};
const uint8_t* const SM2_DEFAULT_UID = _SM2_DEFAULT_UID;

static const UInt256 _CONSTS_N = { .u32 = {
    0x39D54123, 0x53BBF409, 0x21C6052B, 0x7203DF6B,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE
} };
static const UInt256* const CONSTS_N = &_CONSTS_N;

// -n
static const UInt256 _CONSTS_NEG_N = { .u32 = {
    0xC62ABEDD, 0xAC440BF6, 0xDE39FAD4, 0x8DFC2094, 
    0x00000000, 0x00000000, 0x00000000, 0x00000001
} };
static const UInt256* const CONSTS_NEG_N = &_CONSTS_NEG_N;
static const SM2ModNMont* const CONSTS_MODN_MONT_ONE = &_CONSTS_NEG_N;

// 1
static const UInt256 _CONSTS_ONE = { .u64 = {  1, 0, 0, 0 } };
static const UInt256* const CONSTS_ONE = &_CONSTS_ONE;
static const SM2ModN* const CONSTS_MODN_ONE = &_CONSTS_ONE;

// n - 1
static const UInt256 _CONSTS_N_MINUS_ONE = { .u32 = {
    0x39D54122, 0x53BBF409, 0x21C6052B, 0x7203DF6B,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE
} };
static const UInt256* const CONSTS_N_MINUS_ONE = &_CONSTS_N_MINUS_ONE;

// n - 2
static const UInt256 _CONSTS_N_MINUS_TWO = { .u32 = {
    0x39D54121, 0x53BBF409, 0x21C6052B, 0x7203DF6B,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE
} };
static const UInt256* const CONSTS_N_MINUS_TWO = &_CONSTS_N_MINUS_TWO;

// -1 / n
static const UInt256 _CONSTS_N_PRIME = { .u32 = {
    0x72350975, 0x327F9E88, 0xFC8319A5, 0xDF1E8D34, 
    0xB08941D4, 0x2B0068D3, 0x82E4C7BC, 0x6F39132F
} };
static const UInt256* const CONSTS_N_PRIME = &_CONSTS_N_PRIME;

// (2^256)^2
static const SM2ModP _CONSTS_MODN_R2 = { .u32 = {
    0x7C114F20, 0x901192AF, 0xDE6FA2FA, 0x3464504A, 
    0x3AFFE0D4, 0x620FC84C, 0xA22B3D3B, 0x1EB5E412
} };
static const SM2ModP* const CONSTS_MODN_R2 = &_CONSTS_MODN_R2;

static
void SM2ModN_Add(const SM2ModN* x, const SM2ModN* y, SM2ModN* z)
{
    if (UInt256_Add(x, y, z))
    {
        UInt256_Add(z, CONSTS_NEG_N, z);
    }
    else if (UInt256_Cmp(z, CONSTS_N) >= 0)
    {
        UInt256_Sub(z, CONSTS_N, z);
    }
}

static
void SM2ModN_Sub(const SM2ModN* x, const SM2ModN* y, SM2ModN* z)
{
    if (UInt256_Sub(x, y, z))
    {
        UInt256_Sub(z, CONSTS_NEG_N, z);
    }
}

static
void SM2ModN_MontMul(const SM2ModNMont* x, const SM2ModNMont* y, SM2ModNMont* z)
{
    UInt512 _xy = { 0 };
    UInt512* xy = &_xy;
    UInt512 _z_tmp = { 0 };
    UInt512* z_tmp = &_z_tmp;
    uint8_t carry = 0;

    UInt256_Mul(x, y, xy);
    UInt256_Mul(xy->u256, CONSTS_N_PRIME, z_tmp);
    UInt256_Mul(z_tmp->u256, CONSTS_N, z_tmp);

    carry = UInt512_Add(xy, z_tmp, z_tmp);
    (*z) = z_tmp->u256[1];

    if (carry)
    {
        UInt256_Add(z, CONSTS_NEG_N, z);
    }
    else if (UInt256_Cmp(z, CONSTS_N) >= 0)
    {
        UInt256_Sub(z, CONSTS_N, z);
    }
}

static
void SM2ModN_ToMont(const SM2ModN* x, SM2ModNMont* y)
{
    SM2ModN_MontMul(x, CONSTS_MODN_R2, y);
}

static
void SM2ModN_FromMont(const SM2ModNMont* x, SM2ModN* y)
{
    SM2ModN_MontMul(x, CONSTS_MODN_ONE, y);
}

static
void SM2ModN_MontAdd(const SM2ModNMont* x, const SM2ModNMont* y, SM2ModNMont* z)
{
    SM2ModN_Add(x, y, z);
}

static
void SM2ModN_MontSub(const SM2ModNMont* x, const SM2ModNMont* y, SM2ModNMont* z)
{
    SM2ModN_Sub(x, y, z);
}

static
void SM2ModN_MontPow(const SM2ModNMont* x, const UInt256* e, SM2ModNMont* y)
{
    int32_t i;
    uint32_t j;
    uint64_t tmp = 0;
    SM2ModNMont _y_tmp = *CONSTS_MODN_MONT_ONE;
    SM2ModNMont* y_tmp = &_y_tmp;

    for (i = 3; i >= 0; i--)
    {
        tmp = e->u64[i];
        for (j = 0; j < 64; j++)
        {
            SM2ModN_MontMul(y_tmp, y_tmp, y_tmp);
            if (tmp & 0x8000000000000000)
            {
                SM2ModN_MontMul(y_tmp, x, y_tmp);
            }
            tmp <<= 1;
        }
    }

    *y = *y_tmp;
}

static
void SM2ModN_MontInv(const SM2ModNMont* x, SM2ModNMont* y)
{
    SM2ModN_MontPow(x, CONSTS_N_MINUS_TWO, y);
}

static
int _SM2_IsSkValid(const SM2ModN* sk)
{
    return !UInt256_IsZero(sk) && UInt256_Cmp(sk, CONSTS_N_MINUS_TWO) <= 0;
}

int SM2_IsSkValid(const uint8_t* sk)
{
    SM2ModN sk_num = { 0 };
    UInt256_FromBytes(sk, &sk_num);
    return _SM2_IsSkValid(&sk_num);
}

int SM2_IsPkValid(const uint8_t* pk, uint64_t pk_len)
{
    SM2JacobPointMont P = { 0 };
    return pk_len > 1 && SM2JacobPointMont_FromBytes(pk, pk_len, &P) == 0;
}

int SM2_GetPk(const uint8_t* sk, uint8_t* pk, int pc_mode)
{
    SM2ModN sk_num = { 0 };
    SM2JacobPointMont P = { 0 };
    UInt256_FromBytes(sk, &sk_num);

    if (!_SM2_IsSkValid(&sk_num))
        return SM2_ERR_INVALID_SK;

    SM2JacobPointMont_MulG(&sk_num, &P);

    if (pc_mode != SM2_PCMODE_COMPRESS && pc_mode != SM2_PCMODE_MIX)
        pc_mode = SM2_PCMODE_RAW;

    SM2JacobPointMont_ToBytes(&P, pk, pc_mode);
    return 0;
}

static
void _SM2_GetEntityInfo(const SM2JacobPointMont* pk, const uint8_t* uid, uint64_t uid_len, uint8_t* entity_info)
{
    SM2Point P = { 0 };
    uint8_t buffer[SM2_PARAMS_LENGTH] = { 0 };
    SM3 sm3 = { 0 };

    SM3_Init(&sm3);

    buffer[0] = (uint8_t)(uid_len << 3 >> 8);
    buffer[1] = (uint8_t)(uid_len << 3);
    SM3_Update(&sm3, buffer, 2);
    SM3_Update(&sm3, uid, uid_len);

    UInt256_ToBytes(SM2_PARAMS_A, buffer);
    SM3_Update(&sm3, buffer, SM2_PARAMS_LENGTH);
    UInt256_ToBytes(SM2_PARAMS_B, buffer);
    SM3_Update(&sm3, buffer, SM2_PARAMS_LENGTH);

    UInt256_ToBytes(&SM2_PARAMS_G->x, buffer);
    SM3_Update(&sm3, buffer, SM2_PARAMS_LENGTH);
    UInt256_ToBytes(&SM2_PARAMS_G->y, buffer);
    SM3_Update(&sm3, buffer, SM2_PARAMS_LENGTH);

    SM2JacobPointMont_ToPoint(pk, &P);
    UInt256_ToBytes(&P.x, buffer);
    SM3_Update(&sm3, buffer, SM2_PARAMS_LENGTH);
    UInt256_ToBytes(&P.y, buffer);
    SM3_Update(&sm3, buffer, SM2_PARAMS_LENGTH);

    SM3_Digest(&sm3, entity_info);
}

int SM2_Init(SM2* self, const uint8_t* sk, const uint8_t* pk, uint64_t pk_len, const uint8_t* uid, uint64_t uid_len, int pc_mode, RandomAlg* rand_alg)
{
    UInt256 one = { 1 };

    // check and parse sk
    self->has_sk = 0;
    if (sk)
    {
        UInt256_FromBytes(sk, &self->sk);
        if (!_SM2_IsSkValid(&self->sk))
            return SM2_ERR_INVALID_SK;

        SM2ModN_ToMont(&self->sk, &self->sk_modn_mont);
        SM2ModN_MontAdd(CONSTS_MODN_MONT_ONE, &self->sk_modn_mont, &self->inv_1_plus_sk_modn_mont);
        SM2ModN_MontInv(&self->inv_1_plus_sk_modn_mont, &self->inv_1_plus_sk_modn_mont);
        self->has_sk = 1;
    }
    
    // check and parse pk
    self->has_pk = 0;
    if (pk)
    {
        if (pk_len <= 1 || SM2JacobPointMont_FromBytes(pk, pk_len, &self->pk) != 0)
            return SM2_ERR_INVALID_PK;

        self->has_pk = 1;
    }
    else if (self->has_sk)
    {
        SM2JacobPointMont_MulG(&self->sk, &self->pk);
        self->has_pk = 1;
    }

    // check and get entity_info
    if (self->has_pk)
    {
        if (!uid)
        {
            uid = SM2_DEFAULT_UID;
            uid_len = SM2_DEFAULT_UID_LENGTH;
        }
        if (uid_len > SM2_UID_MAX_LENGTH)
            return SM2_ERR_UID_OVERFLOW;

        _SM2_GetEntityInfo( &self->pk, uid, uid_len,self->entity_info);
    }

    // check and parse pc_mode, default to RAW
    if (pc_mode != SM2_PCMODE_COMPRESS && pc_mode != SM2_PCMODE_MIX)
        pc_mode = SM2_PCMODE_RAW;
    self->pc_mode = pc_mode;

    // store rand function and rand obj
    // if init failed, rand_alg should be untouched
    if (!rand_alg)
        self->rand_alg = *DEFAULT_RANDOM_ALGORITHM;
    else
        self->rand_alg = *rand_alg;

    return 0;
}

int SM2_GenerateKeyPair(SM2* self, uint8_t* sk, uint8_t* pk)
{
    SM2ModN sk_num = { 0 };
    SM2JacobPointMont P = { 0 };

    if (!RandomUInt256(&self->rand_alg, CONSTS_N_MINUS_TWO, &sk_num))
        return SM2_ERR_RANDOM_FAILED;

    SM2JacobPointMont_MulG(&sk_num, &P);

    UInt256_ToBytes(&sk_num, sk);
    SM2JacobPointMont_ToBytes(&P, pk, self->pc_mode);
    return 0;
}

int SM2_GetEntityInfo(SM2* self, uint8_t* entity_info)
{
    uint32_t i = 0;

    if (!self->has_pk)
        return SM2_ERR_NEED_PK;

    for (i = 0; i < SM2_ENTITYINFO_LENGTH; i++)
    {
        entity_info[i] = self->entity_info[i];
    }
    return 0;
}

static
int _SM2_SignDigest(SM2* self, const uint8_t* digest, UInt256* r, UInt256* s)
{
    SM2ModN _e = { 0 };
    const SM2ModN* e = &_e;
    UInt256 k = { 0 };
    SM2ModNMont r_modn_mont = { 0 };

    SM2JacobPointMont kG_mont = { 0 };
    SM2Point kG = { 0 };

    UInt256_FromBytes(digest, &_e);
    if (UInt256_Cmp(&_e, CONSTS_N) >= 0)
        UInt256_Sub(&_e, CONSTS_N, &_e);

    do
    {
        if (!RandomUInt256(&self->rand_alg, CONSTS_N_MINUS_ONE, &k))
            return SM2_ERR_RANDOM_FAILED;

        SM2JacobPointMont_MulG(&k, &kG_mont);
        SM2JacobPointMont_ToPoint(&kG_mont, &kG);

        if (UInt256_Cmp(&kG.x, CONSTS_N) >= 0)
            UInt256_Sub(&kG.x, CONSTS_N, &kG.x);

        SM2ModN_Add(e, &kG.x, r);
        if (UInt256_IsZero(r))
            continue;

        SM2ModN_Add(r, &k, &r_modn_mont);
        if (UInt256_IsZero(&r_modn_mont))
            continue;

        // convert to ModNMont
        SM2ModN_ToMont(r, &r_modn_mont);
        SM2ModN_ToMont(&k, &k);
        SM2ModN_MontMul(&r_modn_mont, &self->sk_modn_mont, s);
        SM2ModN_MontSub(&k, s, s);
        SM2ModN_MontMul(s, &self->inv_1_plus_sk_modn_mont, s);
        if (UInt256_IsZero(s))
            continue;

        SM2ModN_FromMont(s, s);
    } while (0);

    return 0;
}

int SM2_SignDigest(SM2* self, const uint8_t* digest, uint8_t* signature)
{
    int has_err = 0;
    UInt256 r_num = { 0 };
    UInt256 s_num = { 0 };

    if (!self->has_sk)
        return SM2_ERR_NEED_SK;

    has_err = _SM2_SignDigest(self, digest, &r_num, &s_num);
    if (has_err)
        return has_err;

    UInt256_ToBytes(&r_num, signature);
    UInt256_ToBytes(&s_num, signature + SM2_SIGN_R_LENGTH);
    return 0;
}

static
int _SM2_VerifyDigest(SM2* self, const uint8_t* digest, const UInt256* r, const UInt256* s)
{
    SM2ModN t = { 0 };
    UInt256 e = { 0 };
    SM2JacobPointMont tP_mont = { 0 };
    SM2JacobPointMont kG_mont = { 0 };
    SM2Point kG = { 0 };

    if (UInt256_IsZero(r) || UInt256_Cmp(r, CONSTS_N_MINUS_ONE) > 0)
        return SM2_ERR_INVALID_SIGN;

    if (UInt256_IsZero(s) || UInt256_Cmp(s, CONSTS_N_MINUS_ONE) > 0)
        return SM2_ERR_INVALID_SIGN;

    SM2ModN_Add(r, s, &t);
    if (UInt256_IsZero(&t))
        return SM2_ERR_INVALID_SIGN;

    UInt256_FromBytes(digest, &e);
    if (UInt256_Cmp(&e, CONSTS_N) >= 0)
        UInt256_Sub(&e, CONSTS_N, &e);

    SM2JacobPointMont_Mul(&t, &self->pk, &tP_mont);
    SM2JacobPointMont_MulG(s, &kG_mont);
    SM2JacobPointMont_Add(&kG_mont, &tP_mont, &kG_mont);
    SM2JacobPointMont_ToPoint(&kG_mont, &kG);
    if (UInt256_Cmp(&kG.x, CONSTS_N) >= 0)
        UInt256_Sub(&kG.x, CONSTS_N, &kG.x);

    SM2ModN_Add(&e, &kG.x, &t);
    if (UInt256_Cmp(&t, r) != 0)
        return SM2_ERR_INVALID_SIGN;

    return 0;
}

int SM2_VerifyDigest(SM2* self, const uint8_t* digest, const uint8_t* signature)
{
    int has_err = 0;
    UInt256 r_num = { 0 };
    UInt256 s_num = { 0 };

    if (!self->has_pk)
        return SM2_ERR_NEED_PK;

    UInt256_FromBytes(signature, &r_num);
    UInt256_FromBytes(signature + SM2_SIGN_R_LENGTH, &s_num);
    return _SM2_VerifyDigest(self, digest, &r_num, &s_num);
}

int SM2_Sign(SM2* self, const uint8_t* msg, uint64_t msg_len, uint8_t* signature)
{
    uint8_t digest[SM3_DIGEST_LENGTH] = { 0 };
    SM3 sm3 = { 0 };

    if (!self->has_sk)
        return SM2_ERR_NEED_SK;

    if (!self->has_pk)
        return SM2_ERR_NEED_PK;

    if (msg_len > SM2_MSG_MAX_LENGTH)
        return SM2_ERR_MSG_OVERFLOW;

    SM3_Init(&sm3);
    SM3_Update(&sm3, self->entity_info, SM2_ENTITYINFO_LENGTH);
    SM3_Update(&sm3, msg, msg_len);
    SM3_Digest(&sm3, digest);

    return SM2_SignDigest(self, digest, signature);
}

int SM2_Verify(SM2* self, const uint8_t* msg, uint64_t msg_len, const uint8_t* signature)
{
    uint8_t digest[SM3_DIGEST_LENGTH] = { 0 };
    SM3 sm3 = { 0 };

    if (!self->has_pk)
        return SM2_ERR_NEED_PK;

    if (msg_len > SM2_MSG_MAX_LENGTH)
        return SM2_ERR_MSG_OVERFLOW;

    SM3_Init(&sm3);
    SM3_Update(&sm3, self->entity_info, SM2_ENTITYINFO_LENGTH);
    SM3_Update(&sm3, msg, msg_len);
    SM3_Digest(&sm3, digest);

    return SM2_VerifyDigest(self, digest, signature);
}
