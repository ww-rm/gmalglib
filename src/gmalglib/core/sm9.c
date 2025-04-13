#include <stdint.h>
#include <stdlib.h>
#include <gmalglib/random.h>
#include <gmalglib/sm9curve.h>
#include <gmalglib/sm3.h>
#include <gmalglib/sm9.h>

#define SCALARMUL_WSIZE          4
#define SCALARMUL_TSIZE          (1 << (SCALARMUL_WSIZE - 1))

// 0xB6400000_02A3A6F1_D603AB4F_F58EC744_49F2934B_18EA8BEE_E56EE19C_D69ECF25
static const UInt256 _CONSTS_N = { .u32 = { 0xD69ECF25, 0xE56EE19C, 0x18EA8BEE, 0x49F2934B, 0xF58EC744, 0xD603AB4F, 0x02A3A6F1, 0xB6400000 } };
static const UInt256* const CONSTS_N = &_CONSTS_N;

// -n
static const UInt256 _CONSTS_NEG_N = { .u32 = { 0x296130DB, 0x1A911E63, 0xE7157411, 0xB60D6CB4, 0x0A7138BB, 0x29FC54B0, 0xFD5C590E, 0x49BFFFFF } };
static const UInt256* const CONSTS_NEG_N = &_CONSTS_NEG_N;
static const SM9ModNMont* const CONSTS_MODN_MONT_ONE = &_CONSTS_NEG_N;

// 1
static const UInt256 _CONSTS_ONE = { .u64 = { 1, 0, 0, 0 } };
static const SM9ModN* const CONSTS_MODN_ONE = &_CONSTS_ONE;

// n - 1
static const UInt256 _CONSTS_N_MINUS_ONE = { .u32 = { 0xD69ECF24, 0xE56EE19C, 0x18EA8BEE, 0x49F2934B, 0xF58EC744, 0xD603AB4F, 0x02A3A6F1, 0xB6400000 } };
static const UInt256* const CONSTS_N_MINUS_ONE = &_CONSTS_N_MINUS_ONE;

// n - 2
static const UInt256 _CONSTS_N_MINUS_TWO = { .u32 = { 0xD69ECF23, 0xE56EE19C, 0x18EA8BEE, 0x49F2934B, 0xF58EC744, 0xD603AB4F, 0x02A3A6F1, 0xB6400000 } };
static const UInt256* const CONSTS_N_MINUS_TWO = &_CONSTS_N_MINUS_TWO;

// -1 / n
static const UInt256 _CONSTS_N_PRIME = { .u32 = { 0x51974B53, 0x1D026623, 0x939A510D, 0xF590740D, 0x48175059, 0x205F4C4B, 0xCAA6FF31, 0xE3582ED4 } };
static const UInt256* const CONSTS_N_PRIME = &_CONSTS_N_PRIME;

// (2^256)^2
static const SM9ModN _CONSTS_MODN_R2 = { .u32 = { 0xCD750C35, 0x7598CD79, 0xBB6DAEAB, 0xE4A08110, 0x7D78A1F9, 0xBFEE4BAE, 0x63695D0E, 0x8894F5D1 } };
static const SM9ModN* const CONSTS_MODN_R2 = &_CONSTS_MODN_R2;

static
void SM9ModN_Add(const SM9ModN* x, const SM9ModN* y, SM9ModN* z)
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
void SM9ModN_Sub(const SM9ModN* x, const SM9ModN* y, SM9ModN* z)
{
    if (UInt256_Sub(x, y, z))
    {
        UInt256_Sub(z, CONSTS_NEG_N, z);
    }
}

static
void SM9ModN_MontMul(const SM9ModNMont* x, const SM9ModNMont* y, SM9ModNMont* z)
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
void SM9ModN_MontSqr(const SM9ModNMont* x, SM9ModNMont* y)
{
    UInt512 _xsqr = { 0 }, * xsqr = &_xsqr;
    UInt512 _y_tmp = { 0 }, * y_tmp = &_y_tmp;
    uint8_t carry = 0;

    UInt256_Sqr(x, xsqr);
    UInt256_Mul(xsqr->u256, CONSTS_N_PRIME, y_tmp);
    UInt256_Mul(y_tmp->u256, CONSTS_N, y_tmp);

    carry = UInt512_Add(xsqr, y_tmp, y_tmp);
    (*y) = y_tmp->u256[1];

    if (carry)
    {
        UInt256_Add(y, CONSTS_NEG_N, y);
    }
    else if (UInt256_Cmp(y, CONSTS_N) >= 0)
    {
        UInt256_Sub(y, CONSTS_N, y);
    }
}

static
void SM9ModN_ToMont(const SM9ModN* x, SM9ModNMont* y)
{
    SM2ModN_MontMul(x, CONSTS_MODN_R2, y);
}

static
void SM9ModN_FromMont(const SM9ModNMont* x, SM9ModN* y)
{
    SM2ModN_MontMul(x, CONSTS_MODN_ONE, y);
}

static
void SM9ModN_MontPow(const SM9ModNMont* x, const UInt256* e, SM9ModNMont* y)
{
    int32_t i = 0;
    int32_t j = 0;
    int32_t w = 0;
    uint32_t wvalue = 0;
    SM9ModNMont y_tmp = { 0 };

    // pre-compute, save odd points, 1, 3, 5, ..., 2^w - 1
    SM9ModNMont table[SCALARMUL_TSIZE] = { *x };
    SM2ModN_MontSqr(x, &y_tmp);
    for (i = 0; i < SCALARMUL_TSIZE - 1; i++)
    {
        SM2ModN_MontMul(table + i, &y_tmp, table + i + 1);
    }

    y_tmp = *CONSTS_MODN_MONT_ONE;
    i = 255;
    while (i >= 0)
    {
        wvalue = (e->u64[i / 64] >> (i % 64)) & 0x1;

        if (wvalue == 0)
        {
            SM2ModN_MontSqr(&y_tmp, &y_tmp);
            i--;
        }
        else
        {
            // find a longest 1...1 bits in window size
            j = i;
            for (w = i - 1; w >= 0 && w > i - SCALARMUL_WSIZE; w--)
            {
                if ((e->u64[w / 64] >> (w % 64)) & 0x1)
                {
                    wvalue = (wvalue << (j - w)) | 0x1;
                    j = w;
                }
            }

            while (i >= j)
            {
                SM2ModN_MontSqr(&y_tmp, &y_tmp);
                i--;
            }

            SM2ModN_MontMul(&y_tmp, table + (wvalue >> 1), &y_tmp);
        }
    }

    *y = y_tmp;
}

static
void SM9ModN_MontInv(const SM9ModNMont* x, SM9ModNMont* y)
{
    SM2ModN_MontPow(x, CONSTS_N_MINUS_TWO, y);
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ModN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> SM9KGC >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

static
int _SM9KGC_IsMskValid(const SM9ModN* sk)
{
    return !UInt256_IsZero(sk) && UInt256_Cmp(sk, CONSTS_N_MINUS_ONE) <= 0;
}

int SM9KGC_IsMskForSigValid(const uint8_t* msk)
{
    SM9ModN msk_num = { 0 };
    UInt256_FromBytes(msk, &msk_num);
    return _SM9KGC_IsMskValid(&msk_num);
}
int SM9KGC_IsMskForEncValid(const uint8_t* msk)
{
    SM9ModN msk_num = { 0 };
    UInt256_FromBytes(msk, &msk_num);
    return _SM9KGC_IsMskValid(&msk_num);
}

int SM9KGC_IsMpkForSigValid(const uint8_t* mpk, uint64_t mpk_len)
{
    SM9JacobPoint2Mont P = { 0 };
    return mpk_len > 1 && SM9JacobPoint2Mont_FromBytes(mpk, mpk_len, &P) == 0;
}
int SM9KGC_IsMpkForEncValid(const uint8_t* mpk, uint64_t mpk_len)
{
    SM9JacobPoint1Mont P = { 0 };
    return mpk_len > 1 && SM9JacobPoint1Mont_FromBytes(mpk, mpk_len, &P) == 0;
}

int SM9KGC_IsMasterKeyPairForSig(const uint8_t* msk, const uint8_t* mpk, uint64_t mpk_len)
{
    SM9ModN msk_num = { 0 };
    SM9JacobPoint2Mont P = { 0 };
    SM9JacobPoint2Mont skG = { 0 };

    UInt256_FromBytes(msk, &msk_num);
    if (!_SM9KGC_IsMskValid(&msk_num))
        return 0;

    if (mpk_len <= 1 || SM9JacobPoint2Mont_FromBytes(mpk, mpk_len, &P) != 0)
        return 0;

    SM9JacobPoint2Mont_MulG2(&msk_num, &skG);

    return SM9JacobPoint2Mont_IsEqual(&P, &skG);
}
int SM9KGC_IsMasterKeyPairForEnc(const uint8_t* msk, const uint8_t* mpk, uint64_t mpk_len)
{
    SM9ModN msk_num = { 0 };
    SM9JacobPoint1Mont P = { 0 };
    SM9JacobPoint1Mont skG = { 0 };

    UInt256_FromBytes(msk, &msk_num);
    if (!_SM9KGC_IsMskValid(&msk_num))
        return 0;

    if (mpk_len <= 1 || SM9JacobPoint1Mont_FromBytes(mpk, mpk_len, &P) != 0)
        return 0;

    SM9JacobPoint1Mont_MulG1(&msk_num, &skG);

    return SM9JacobPoint1Mont_IsEqual(&P, &skG);
}

int SM9KGC_GetMpkForSig(const uint8_t* msk, int pc_mode, uint8_t* mpk)
{
    SM9ModN msk_num = { 0 };
    SM9JacobPoint2Mont P = { 0 };
    UInt256_FromBytes(msk, &msk_num);

    if (!_SM9KGC_IsMskValid(&msk_num))
        return SM9_ERR_INVALID_MSK;

    SM9JacobPoint2Mont_MulG2(&msk_num, &P);

    if (pc_mode != SM9_PCMODE_COMPRESS && pc_mode != SM9_PCMODE_MIX)
        pc_mode = SM9_PCMODE_RAW;

    SM9JacobPoint2Mont_ToBytes(&P, pc_mode, mpk);
    return 0;
}
int SM9KGC_GetMpkForEnc(const uint8_t* msk, int pc_mode, uint8_t* mpk)
{
    SM9ModN msk_num = { 0 };
    SM9JacobPoint1Mont P = { 0 };
    UInt256_FromBytes(msk, &msk_num);

    if (!_SM9KGC_IsMskValid(&msk_num))
        return SM9_ERR_INVALID_MSK;

    SM9JacobPoint1Mont_MulG1(&msk_num, &P);

    if (pc_mode != SM9_PCMODE_COMPRESS && pc_mode != SM9_PCMODE_MIX)
        pc_mode = SM9_PCMODE_RAW;

    SM9JacobPoint1Mont_ToBytes(&P, pc_mode, mpk);
    return 0;
}

int SM9KGC_ConvertMpkForSig(const uint8_t* mpk, uint64_t mpk_len, int pc_mode, uint8_t* mpk_converted)
{
    SM9JacobPoint2Mont P = { 0 };
    if (mpk_len <= 1 || SM9JacobPoint2Mont_FromBytes(mpk, mpk_len, &P) != 0)
        return SM9_ERR_INVALID_MPK;

    if (pc_mode != SM9_PCMODE_COMPRESS && pc_mode != SM9_PCMODE_MIX)
        pc_mode = SM9_PCMODE_RAW;

    SM9JacobPoint2Mont_ToBytes(&P, pc_mode, mpk_converted);
    return 0;
}
int SM9KGC_ConvertMpkForEnc(const uint8_t* mpk, uint64_t mpk_len, int pc_mode, uint8_t* mpk_converted)
{
    SM9JacobPoint1Mont P = { 0 };
    if (mpk_len <= 1 || SM9JacobPoint1Mont_FromBytes(mpk, mpk_len, &P) != 0)
        return SM9_ERR_INVALID_MPK;

    if (pc_mode != SM9_PCMODE_COMPRESS && pc_mode != SM9_PCMODE_MIX)
        pc_mode = SM9_PCMODE_RAW;

    SM9JacobPoint1Mont_ToBytes(&P, pc_mode, mpk_converted);
    return 0;
}

int SM9KGC_Init(
    SM9KGC* self,
    const uint8_t* hid_s, const uint8_t* msk_s, const uint8_t* mpk_s, uint64_t mpk_s_len,
    const uint8_t* hid_e, const uint8_t* msk_e, const uint8_t* mpk_e, uint64_t mpk_e_len,
    int pc_mode, RandomAlg* rand_alg
)
{
    // check hid
    self->has_hid_s = (hid_s != 0);
    if (hid_s) self->hid_s = *hid_s;
    self->has_hid_e = (hid_e != 0);
    if (hid_e) self->hid_e = *hid_e;

    // check and parse msk_s
    self->has_msk_s = 0;
    if (msk_s)
    {
        UInt256_FromBytes(msk_s, &self->msk_s);
        if (!_SM9KGC_IsMskValid(&self->msk_s))
            return SM9_ERR_INVALID_MSK;

        SM9ModN_ToMont(&self->msk_s, &self->msk_s_mont);
        self->has_msk_s = 1;
    }

    // check and parse msk_e
    self->has_msk_e = 0;
    if (msk_e)
    {
        UInt256_FromBytes(msk_e, &self->msk_e);
        if (!_SM9KGC_IsMskValid(&self->msk_e))
            return SM9_ERR_INVALID_MSK;

        SM9ModN_ToMont(&self->msk_e, &self->msk_e_mont);
        self->has_msk_e = 1;
    }

    // check and parse mpk_s
    self->has_mpk_s = 0;
    if (self->has_msk_s)
    {
        // omit passed mpk argument
        SM9JacobPoint2Mont_MulG2(&self->msk_s, &self->mpk_s);
        self->has_mpk_s = 1;
    }
    else if (mpk_s)
    {
        if (mpk_s_len <= 1 || SM9JacobPoint2Mont_FromBytes(mpk_s, mpk_s_len, &self->mpk_s) != 0)
            return SM9_ERR_INVALID_MPK;

        self->has_mpk_s = 1;
    }

    // check and parse mpk_e
    self->has_mpk_e = 0;
    if (self->has_msk_e)
    {
        // omit passed mpk argument
        SM9JacobPoint1Mont_MulG1(&self->msk_e, &self->mpk_e);
        self->has_mpk_e = 1;
    }
    else if (mpk_e)
    {
        if (mpk_e_len <= 1 || SM9JacobPoint1Mont_FromBytes(mpk_e, mpk_e_len, &self->mpk_e) != 0)
            return SM9_ERR_INVALID_MPK;

        self->has_mpk_e = 1;
    }

    // check and parse pc_mode, default to RAW
    if (pc_mode != SM9_PCMODE_COMPRESS && pc_mode != SM9_PCMODE_MIX)
        pc_mode = SM9_PCMODE_RAW;
    self->pc_mode = pc_mode;

    // store rand function and rand obj
    // if init failed, rand_alg should be untouched
    if (!rand_alg)
        self->rand_alg = *DEFAULT_RANDOM_ALGORITHM;
    else
        self->rand_alg = *rand_alg;
}

int SM9KGC_GenerateMasterKeyPairForSig(SM9KGC* self, uint8_t* msk, uint8_t* mpk)
{
    SM9ModN msk_num = { 0 };
    SM9JacobPoint2Mont P = { 0 };

    if (!RandomUInt256(&self->rand_alg, CONSTS_N_MINUS_ONE, &msk_num))
        return SM9_ERR_RANDOM_FAILED;

    SM9JacobPoint2Mont_MulG2(&msk_num, &P);

    UInt256_ToBytes(&msk_num, msk);
    SM9JacobPoint2Mont_ToBytes(&P, self->pc_mode, mpk);
    return 0;
}
int SM9KGC_GenerateMasterKeyPairForEnc(SM9KGC* self, uint8_t* msk, uint8_t* mpk)
{
    SM9ModN msk_num = { 0 };
    SM9JacobPoint1Mont P = { 0 };

    if (!RandomUInt256(&self->rand_alg, CONSTS_N_MINUS_ONE, &msk_num))
        return SM9_ERR_RANDOM_FAILED;

    SM9JacobPoint1Mont_MulG1(&msk_num, &P);

    UInt256_ToBytes(&msk_num, msk);
    SM9JacobPoint1Mont_ToBytes(&P, self->pc_mode, mpk);
    return 0;
}

static
void _H1(const uint8_t* uid, uint64_t uid_len, uint8_t hid, SM9ModN* hvalue)
{
    uint8_t prefix = 0x01;
    uint8_t hval_bytes[40] = { 0 };
    SM3 sm3 = { 0 };
    SM3_Init(&sm3);
    SM3_Update(&sm3, &prefix, 1);
    SM3_Update(&sm3, &uid, uid_len);
    SM3_Update(&sm3, &hid, 1);
    SM3_DeriveKey(&sm3, 40, hval_bytes);
    // TODO: make hvalue, barrett reduction
}

static
void _H2(const uint8_t* msg, uint64_t msg_len, SM9FP12 fp12num, SM9ModN* hvalue)
{
    uint8_t prefix = 0x02;
    uint8_t hval_bytes[40] = { 0 };
    SM3 sm3 = { 0 };
    uint8_t buffer[32] = { 0 };
    SM3_Init(&sm3);
    SM3_Update(&sm3, &prefix, 1);
    SM3_Update(&sm3, &msg, msg_len);
    for (int i = 11; i >= 0; i--)
    {
        UInt256_ToBytes(fp12num.fp1 + i, buffer);
        SM3_Update(&sm3, buffer, 32);
    }
    SM3_DeriveKey(&sm3, 40, hval_bytes);
    // TODO: make hvalue, barrett reduction
}

int SM9KGC_GenerateSkForSig(SM9KGC* self, const uint8_t* uid, uint64_t uid_len, uint8_t* sk)
{
    if (!self->has_hid_s)
        return SM9_ERR_NEED_HID;
    if (!self->has_msk_s)
        return SM9_ERR_NEED_MSK;

    SM9ModN t1 = { 0 };
    SM9ModN t2 = { 0 };
    SM9JacobPoint1Mont sk_pt = { 0 };

    uint8_t data; // TODO
}

int SM9KGC_GenerateSkForEnc(SM9KGC* self, const uint8_t* uid, uint64_t uid_len, uint8_t* sk)
{
    if (!self->has_hid_e)
        return SM9_ERR_NEED_HID;
    if (!self->has_msk_e)
        return SM9_ERR_NEED_MSK;
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< SM9KGC <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




































