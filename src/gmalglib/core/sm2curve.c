#include <stdint.h>
#include <gmalglib/bignum.h>
#include <gmalglib/sm2curve.h>

#ifdef _DEBUG

#include <stdio.h>

#endif // _DEBUG


// 0xFFFFFFFE_FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF_00000000_FFFFFFFF_FFFFFFFF
static const UInt256 _CONSTS_P = { .u32 = {
    0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE
} };
static const UInt256* const CONSTS_P = &_CONSTS_P;

// -3
// 0xFFFFFFFE_FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF_00000000_FFFFFFFF_FFFFFFFC
static const UInt256 _CONSTS_A = { .u32 = {
    0xFFFFFFFC, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE
} };

// 0x28E9FA9E_9D9F5E34_4D5A9E4B_CF6509A7_F39789F5_15AB8F92_DDBCBD41_4D940E93
static const UInt256 _CONSTS_B = { .u32 = {
    0x4D940E93, 0xDDBCBD41, 0x15AB8F92, 0xF39789F5,
    0xCF6509A7, 0x4D5A9E4B, 0x9D9F5E34, 0x28E9FA9E
} };

// 0x32C4AE2C_1F198119_5F990446_6A39C994_8FE30BBF_F2660BE1_715A4589_334C74C7
// 0xBC3736A2_F4F6779C_59BDCEE3_6B692153_D0A9877C_C62A4740_02DF32E5_2139F0A0
static const SM2Point _CONSTS_POINT_G = {
    {.u32 = { 0x334C74C7, 0x715A4589, 0xF2660BE1, 0x8FE30BBF, 0x6A39C994, 0x5F990446, 0x1F198119, 0x32C4AE2C } },
    {.u32 = { 0x2139F0A0, 0x02DF32E5, 0xC62A4740, 0xD0A9877C, 0x6B692153, 0x59BDCEE3, 0xF4F6779C, 0xBC3736A2 } },
    0
};

// 0xFFFFFFFE_FFFFFFFF_FFFFFFFF_FFFFFFFF_7203DF6B_21C6052B_53BBF409_39D54123
static const UInt256 _CONSTS_N = { .u32 = {
    0x39D54123, 0x53BBF409, 0x21C6052B, 0x7203DF6B, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE
} };

const UInt256* const SM2_PARAMS_P = &_CONSTS_P;
const UInt256* const SM2_PARAMS_A = &_CONSTS_A;
const UInt256* const SM2_PARAMS_B = &_CONSTS_B;
const SM2Point* const SM2_PARAMS_G = &_CONSTS_POINT_G;
const UInt256* const SM2_PARAMS_N = &_CONSTS_N;

// 1
static const UInt256 _CONSTS_ONE = { .u64 = {  1, 0, 0, 0 } };
static const SM2ModP* const CONSTS_MODP_ONE = &_CONSTS_ONE;

// p = 4u + 3
static const UInt256 _CONSTS_U_PLUS_ONE = { .u32 = {
    0x00000000, 0x40000000, 0xC0000000, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xBFFFFFFF, 0x3FFFFFFF
} };
static const UInt256* const CONSTS_U_PLUS_ONE = &_CONSTS_U_PLUS_ONE;

// p - 2
static const UInt256 _CONSTS_P_MINUS_TWO = { .u32 = {
    0xFFFFFFFD, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE
} };
static const UInt256* const CONSTS_P_MINUS_TWO = &_CONSTS_P_MINUS_TWO;

// -p
static const UInt256 _CONSTS_NEG_P = { .u32 = {
    0x00000001, 0x00000000, 0xFFFFFFFF, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000001
} };
static const UInt256* const CONSTS_NEG_P = &_CONSTS_NEG_P;
static const SM2ModPMont* const CONSTS_MODP_MONT_ONE = &_CONSTS_NEG_P;

// -1 / p
static const UInt256 _CONSTS_P_PRIME = { .u32 = {
    0x00000001, 0x00000000, 0x00000001, 0xFFFFFFFF, 
    0x00000000, 0xFFFFFFFE, 0x00000001, 0xFFFFFFFC,
} };
static const UInt256* const CONSTS_P_PRIME = &_CONSTS_P_PRIME;

// (2^256)^2
static const SM2ModP _CONSTS_MODP_R2 = { .u32 = {
    0x00000003, 0x00000002, 0xFFFFFFFF, 0x00000002,
    0x00000001, 0x00000001, 0x00000002, 0x00000004
} };
static const SM2ModP* const CONSTS_MODP_R2 = &_CONSTS_MODP_R2;

// aR
static const SM2ModPMont _CONSTS_MODP_MONT_A = { .u32 = {
    0xFFFFFFFC, 0xFFFFFFFF, 0x00000003, 0xFFFFFFFC, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFB
} };
static const SM2ModPMont* const CONSTS_MODP_MONT_A = &_CONSTS_MODP_MONT_A;

// bR
static const SM2ModPMont _CONSTS_MODP_MONT_B = { .u32 = {
    0x2BC0DD42, 0x90D23063, 0xE9B537AB, 0x71CF379A, 
    0x5EA51C3C, 0x52798150, 0xBA20E2C8, 0x240FE188
} };
static const SM2ModPMont* const CONSTS_MODP_MONT_B = &_CONSTS_MODP_MONT_B;

// G Point
static const SM2JacobPointMont _CONSTS_JACOB_G = {
    {.u32 = {0xF418029E, 0x61328990, 0xDCA6C050, 0x3E7981ED, 0xAC24C3C3, 0xD6A1ED99, 0xE1C13B05, 0x91167A5E} },
    {.u32 = {0x3C2D0DDD, 0xC1354E59, 0x8D3295FA, 0xC1F5E578, 0x6E2A48F8, 0x8D4CFB06, 0x81D735BD, 0x63CD65D4} },
    {.u32 = {0x00000001, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001} }
};
static const SM2JacobPointMont* const CONSTS_JACOB_G = &_CONSTS_JACOB_G;

static 
void SM2ModP_Add(const SM2ModP* x, const SM2ModP* y, SM2ModP* z)
{
    if (UInt256_Add(x, y, z))
    {
        UInt256_Add(z, CONSTS_NEG_P, z);
    }
    else if (UInt256_Cmp(z, CONSTS_P) >= 0)
    {
        UInt256_Sub(z, CONSTS_P, z);
    }
}

static 
void SM2ModP_Sub(const SM2ModP* x, const SM2ModP* y, SM2ModP* z)
{
    if (UInt256_Sub(x, y, z))
    {
        UInt256_Sub(z, CONSTS_NEG_P, z);
    }
}

static 
void SM2ModP_MontMul(const SM2ModPMont* x, const SM2ModPMont* y, SM2ModPMont* z)
{
    UInt512 _xy = { 0 };
    UInt512* xy = &_xy;
    UInt512 _z_tmp = { 0 };
    UInt512* z_tmp = &_z_tmp;
    uint8_t carry = 0;

    UInt256_Mul(x, y, xy);
    UInt256_Mul(xy->u256, CONSTS_P_PRIME, z_tmp);
    UInt256_Mul(z_tmp->u256, CONSTS_P, z_tmp);

    carry = UInt512_Add(xy, z_tmp, z_tmp);
    (*z) = z_tmp->u256[1];

    if (carry)
    {
        UInt256_Add(z, CONSTS_NEG_P, z);
    }
    else if (UInt256_Cmp(z, CONSTS_P) >= 0)
    {
        UInt256_Sub(z, CONSTS_P, z);
    }
}

static 
void SM2ModP_ToMont(const SM2ModP* x, SM2ModPMont* y)
{
    SM2ModP_MontMul(x, CONSTS_MODP_R2, y);
}

static 
void SM2ModP_FromMont(const SM2ModPMont* x, SM2ModP* y)
{
    SM2ModP_MontMul(x, CONSTS_MODP_ONE, y);
}

static 
void SM2ModP_MontAdd(const SM2ModPMont* x, const SM2ModPMont* y, SM2ModPMont* z)
{
    SM2ModP_Add(x, y, z);
}

static 
void SM2ModP_MontSub(const SM2ModPMont* x, const SM2ModPMont* y, SM2ModPMont* z)
{
    SM2ModP_Sub(x, y, z);
}

static 
void SM2ModP_MontPow(const SM2ModPMont* x, const UInt256* e, SM2ModPMont* y)
{
    int32_t i;
    uint32_t j;
    uint64_t tmp = 0;
    SM2ModPMont _y_tmp = *CONSTS_MODP_MONT_ONE;
    SM2ModPMont* y_tmp = &_y_tmp;

    for (i = 3; i >= 0; i--)
    {
        tmp = e->u64[i];
        for (j = 0; j < 64; j++)
        {
            SM2ModP_MontMul(y_tmp, y_tmp, y_tmp);
            if (tmp & 0x8000000000000000)
            {
                SM2ModP_MontMul(y_tmp, x, y_tmp);
            }
            tmp <<= 1;
        }
    }

    *y = *y_tmp;
}

static 
int SM2ModP_MontHasSqrt(const SM2ModPMont* x, SM2ModPMont* y)
{
    int ret = 0;
    SM2ModPMont y_tmp = { 0 };
    SM2ModPMont ysqr = { 0 };

    SM2ModP_MontPow(x, CONSTS_U_PLUS_ONE, &y_tmp);
    SM2ModP_MontMul(&y_tmp, &y_tmp, &ysqr);
    ret = UInt256_Cmp(x, &ysqr) == 0;

    *y = y_tmp;
    return ret;
}

static 
void SM2ModP_MontNeg(const SM2ModPMont* x, SM2ModPMont* y)
{
    if (UInt256_Sub(CONSTS_P, x, y))
    {
        UInt256_Sub(y, CONSTS_NEG_P, y);
    }
}

static 
void SM2ModP_MontInv(const SM2ModPMont* x, SM2ModPMont* y)
{
    SM2ModP_MontPow(x, CONSTS_P_MINUS_TWO, y);
}

int SM2JacobPointMont_IsInf(const SM2JacobPointMont* X)
{
    return UInt256_IsZero(&X->z);
}

void SM2JacobPointMont_SetInf(SM2JacobPointMont* X)
{
    X->x = *CONSTS_MODP_MONT_ONE;
    X->y = *CONSTS_MODP_MONT_ONE;
    X->z.u64[0] = X->z.u64[1] = X->z.u64[2] = X->z.u64[3] = 0;
}

int SM2JacobPointMont_IsOnCurve(const SM2JacobPointMont* X)
{
    SM2ModPMont left = { 0 };
    SM2ModPMont right = { 0 };
    SM2ModPMont tmp = { 0 };

    if (UInt256_Cmp(&X->z, CONSTS_MODP_MONT_ONE) == 0)
    {
        // left = y^2 + 3x
        SM2ModP_MontMul(&X->y, &X->y, &left);
        SM2ModP_MontAdd(&left, &X->x, &left);
        SM2ModP_MontAdd(&left, &X->x, &left);
        SM2ModP_MontAdd(&left, &X->x, &left);

        // right = x^3 + b
        SM2ModP_MontMul(&X->x, &X->x, &right);
        SM2ModP_MontMul(&right, &X->x, &right);
        SM2ModP_MontAdd(&right, CONSTS_MODP_MONT_B, &right);
    }
    else
    {
        SM2ModP_MontMul(&X->z, &X->z, &tmp);                    // z^2
        SM2ModP_MontMul(&tmp, &tmp, &left);                     // z^4
        SM2ModP_MontMul(&left, &tmp, &right);                   // z^6

        // left = y^2 + 3xz^4
        SM2ModP_MontMul(&X->x, &left, &tmp);                    // xz^4
        SM2ModP_MontAdd(&tmp, &tmp, &left);                     // 2xz^4
        SM2ModP_MontAdd(&left, &tmp, &left);                    // 3xz^4
        SM2ModP_MontMul(&X->y, &X->y, &tmp);                    // y^2
        SM2ModP_MontAdd(&tmp, &left, &left);                    // y^2 + 3xz^4

        // right = x^3 + bz^6
        SM2ModP_MontMul(&X->x, &X->x, &tmp);                    // x^2
        SM2ModP_MontMul(&tmp, &X->x, &tmp);                     // x^3
        SM2ModP_MontMul(&right, CONSTS_MODP_MONT_B, &right);    // bz^6
        SM2ModP_MontAdd(&tmp, &right, &right);                  // x^3 + bz^6
    }

    return UInt256_Cmp(&left, &right) == 0;
}

int SM2JacobPointMont_IsEqual(const SM2JacobPointMont* X, const SM2JacobPointMont* Y)
{
    SM2ModPMont t1 = { 0 };
    SM2ModPMont t2 = { 0 };
    SM2ModPMont t4 = { 0 };
    SM2ModPMont t5 = { 0 };

    if (UInt256_Cmp(&X->z, CONSTS_MODP_MONT_ONE) == 0 && UInt256_Cmp(&Y->z, CONSTS_MODP_MONT_ONE) == 0)
        return UInt256_Cmp(&X->x, &Y->x) == 0 && UInt256_Cmp(&X->y, &Y->y) == 0;

    // t1 = x1 * z2^2
    // t2 = x2 * z1^2
    // t4 = y1 * z2^3
    // t5 = y2 * z1^3
    SM2ModP_MontMul(&Y->z, &Y->z, &t4);         // z2^2
    SM2ModP_MontMul(&X->z, &X->z, &t5);         // z1^2

    SM2ModP_MontMul(&X->x, &t4, &t1);           // x1 * z2^2
    SM2ModP_MontMul(&Y->x, &t5, &t2);           // x2 * z1^2

    if (UInt256_Cmp(&t1, &t2) != 0)
        return 0;

    SM2ModP_MontMul(&X->y, &t4, &t4);           // y1 * z2^2
    SM2ModP_MontMul(&t4, &Y->z, &t4);           // y1 * z2^3

    SM2ModP_MontMul(&Y->y, &t5, &t5);           // y2 * z1^2
    SM2ModP_MontMul(&t5, &X->z, &t5);           // y2 * z1^3

    if (UInt256_Cmp(&t4, &t5) != 0)
        return 0;

    return 1;
}

void SM2JacobPointMont_ToPoint(const SM2JacobPointMont* X, SM2Point* Y)
{
    SM2ModPMont z_inv = { 0 };
    if (SM2JacobPointMont_IsInf(X))
    {
        Y->is_inf = 1;
    }
    else
    {
        Y->is_inf = 0;
        if (UInt256_Cmp(&X->z, CONSTS_MODP_MONT_ONE) == 0)
        {
            SM2ModP_FromMont(&X->x, &Y->x);
            SM2ModP_FromMont(&X->y, &Y->y);
        }
        else
        {
            SM2ModP_MontInv(&X->z, &z_inv);
            SM2ModP_MontMul(&X->y, &z_inv, &Y->y);
            SM2ModP_MontMul(&z_inv, &z_inv, &z_inv);
            SM2ModP_MontMul(&X->x, &z_inv, &Y->x);
            SM2ModP_MontMul(&Y->y, &z_inv, &Y->y);

            SM2ModP_FromMont(&Y->x, &Y->x);
            SM2ModP_FromMont(&Y->y, &Y->y);
        }
    }
}

void SM2JacobPointMont_FromPoint(const SM2Point* X, SM2JacobPointMont* Y)
{
    if (X->is_inf)
    {
        SM2JacobPointMont_SetInf(Y);
    }
    else
    {
        SM2ModP_ToMont(&X->x, &Y->x);
        SM2ModP_ToMont(&X->y, &Y->y);
        Y->z = *CONSTS_MODP_MONT_ONE;
    }
}

uint64_t SM2JacobPointMont_ToBytes(const SM2JacobPointMont* X, int pc_mode, uint8_t* bytes)
{
    SM2Point pt = { 0 };
    SM2JacobPointMont_ToPoint(X, &pt);

    if (pt.is_inf)
    {
        bytes[0] = 0x00;
        return 1;
    }

    if (pc_mode == SM2_PCMODE_COMPRESS)
    {
        if (pt.y.u8[0] & 0x1)
            bytes[0] = 0x03;
        else
            bytes[0] = 0x02;
        UInt256_ToBytes(&pt.x, bytes + 1);
        return 33;
    }
    else if (pc_mode == SM2_PCMODE_MIX)
    {
        if (pt.y.u8[0] & 0x1)
            bytes[0] = 0x07;
        else
            bytes[0] = 0x06;
        UInt256_ToBytes(&pt.x, bytes + 1);
        UInt256_ToBytes(&pt.y, bytes + 33);
        return 65;
    }
    else
    {
        bytes[0] = 0x04;
        UInt256_ToBytes(&pt.x, bytes + 1);
        UInt256_ToBytes(&pt.y, bytes + 33);
        return 65;
    }

    return 0;
}

int SM2JacobPointMont_FromBytes(const uint8_t* bytes, uint64_t bytes_len, SM2JacobPointMont* X)
{
    SM2Point pt = { 0 };
    uint8_t pc = bytes[0];
    int ylsb = 0;

    if (pc == 0x00)
    {
        if (bytes_len != 1)
            return SM2CURVE_ERR_INVALIDPC;

        SM2JacobPointMont_SetInf(X);
    }
    else if (pc == 0x04 || pc == 0x06 || pc == 0x07)
    {
        if (bytes_len != SM2_POINTBYTES_FULL_LENGTH)
            return SM2CURVE_ERR_INVALIDPC;

        UInt256_FromBytes(bytes + 1, &pt.x);
        UInt256_FromBytes(bytes + 33, &pt.y);
        pt.is_inf = 0;
        SM2JacobPointMont_FromPoint(&pt, X);
        if (!SM2JacobPointMont_IsOnCurve(X))
            return SM2CURVE_ERR_NOTONCURVE;
    }
    else if (pc == 0x02 || pc == 0x03)
    {
        if (bytes_len != SM2_POINTBYTES_HALF_LENGTH)
            return SM2CURVE_ERR_INVALIDPC;

        UInt256_FromBytes(bytes + 1, &pt.x);
        if (UInt256_Cmp(&pt.x, CONSTS_P) >= 0)
            return SM2CURVE_ERR_NOTONCURVE;

        SM2ModP_ToMont(&pt.x, &X->x);
        X->z = *CONSTS_MODP_MONT_ONE;

        // compute y
        SM2ModP_MontMul(&X->x, &X->x, &X->y);
        SM2ModP_MontAdd(&X->y, CONSTS_MODP_MONT_A, &X->y);
        SM2ModP_MontMul(&X->x, &X->y, &X->y);
        SM2ModP_MontAdd(&X->y, CONSTS_MODP_MONT_B, &X->y);
        if (!SM2ModP_MontHasSqrt(&X->y, &X->y))
            return SM2CURVE_ERR_NOTONCURVE;

        SM2ModP_FromMont(&X->y, &pt.y);
        ylsb = pt.y.u8[0] & 0x1;
        if ((pc == 0x02 && ylsb) || (pc == 0x03 && !ylsb))
        {
            SM2ModP_MontNeg(&X->y, &X->y);
        }
    }
    else
    {
        return SM2CURVE_ERR_INVALIDPC;
    }

    return 0;
}

static 
void _SM2JacobPointMont_Dbl(const SM2JacobPointMont* X, SM2JacobPointMont* Y)
{
    // order is IMPORTANT to avoid data overwrite

    SM2ModPMont t1 = { 0 };
    SM2ModPMont t2 = { 0 };
    SM2ModPMont t3 = { 0 };

    // a == -3 (mod p)
    // t1 = 3(x + z^2)(x - z^2)
    SM2ModP_MontMul(&X->z, &X->z, &t1);         // z^2
    SM2ModP_MontSub(&X->x, &t1, &t2);           // x - z^2
    SM2ModP_MontAdd(&X->x, &t1, &t1);           // x + z^2
    SM2ModP_MontMul(&t1, &t2, &t1);             // (x + z^2)(x - z^2)
    SM2ModP_MontAdd(&t1, &t1, &t2);             // 2(x + z^2)(x - z^2)
    SM2ModP_MontAdd(&t1, &t2, &t1);             // 3(x + z^2)(x - z^2)

    SM2ModP_MontAdd(&X->y, &X->y, &t3);         // 2y
    SM2ModP_MontMul(&t3, &t3, &t2);             // 4y^2
    SM2ModP_MontMul(&t2, &t3, &t3);             // 8y^3

    // t2 = 4xy^2
    SM2ModP_MontMul(&X->x, &t2, &t2);           // 4xy^2

    // t3 = 8y^4
    SM2ModP_MontMul(&X->y, &t3, &t3);           // 8y^4

    // z' = 2yz
    SM2ModP_MontMul(&X->y, &X->z, &Y->z);
    SM2ModP_MontAdd(&Y->z, &Y->z, &Y->z);

    // x' = t1^2 - 2t2
    SM2ModP_MontMul(&t1, &t1, &Y->x);
    SM2ModP_MontSub(&Y->x, &t2, &Y->x);
    SM2ModP_MontSub(&Y->x, &t2, &Y->x);

    // y' = t1(t2 - x') - t3
    SM2ModP_MontSub(&t2, &Y->x, &Y->y);
    SM2ModP_MontMul(&t1, &Y->y, &Y->y);
    SM2ModP_MontSub(&Y->y, &t3, &Y->y);
}

static 
void _SM2JacobPointMont_Add(const SM2JacobPointMont* X, const SM2JacobPointMont* Y, SM2JacobPointMont* Z)
{
    SM2ModPMont t1 = { 0 };
    SM2ModPMont t2 = { 0 };
    SM2ModPMont t3 = { 0 };
    SM2ModPMont t4 = { 0 };
    SM2ModPMont t5 = { 0 };
    SM2ModPMont t6 = { 0 };

    // t1 = x1 * z2^2
    // t2 = x2 * z1^2
    // t4 = y1 * z2^3
    // t5 = y2 * z1^3
    SM2ModP_MontMul(&Y->z, &Y->z, &t4);         // z2^2
    SM2ModP_MontMul(&X->z, &X->z, &t5);         // z1^2

    SM2ModP_MontMul(&X->x, &t4, &t1);           // x1 * z2^2
    SM2ModP_MontMul(&Y->x, &t5, &t2);           // x2 * z1^2

    SM2ModP_MontMul(&X->y, &t4, &t4);           // y1 * z2^2
    SM2ModP_MontMul(&t4, &Y->z, &t4);           // y1 * z2^3

    SM2ModP_MontMul(&Y->y, &t5, &t5);           // y2 * z1^2
    SM2ModP_MontMul(&t5, &X->z, &t5);           // y2 * z1^3

    if (UInt256_Cmp(&t1, &t2) == 0)
    {
        if (UInt256_Cmp(&t4, &t5) == 0)
            _SM2JacobPointMont_Dbl(X, Z);
        else
            SM2JacobPointMont_SetInf(Z);
        return;
    }

    // t3 = t1 - t2
    // t6 = t4 - t5
    SM2ModP_MontSub(&t1, &t2, &t3);
    SM2ModP_MontSub(&t4, &t5, &t6);

    // z3 = z1 * z2 * t3
    SM2ModP_MontMul(&X->z, &Y->z, &Z->z);       // z1 * z2
    SM2ModP_MontMul(&Z->z, &t3, &Z->z);         // z1 * z2 * t3

    // x3 = t6^2 - (t1 + t2) * t3^2
    SM2ModP_MontMul(&t6, &t6, &Z->x);           // t6^2
    SM2ModP_MontAdd(&t1, &t2, &t2);             // t1 + t2
    SM2ModP_MontMul(&t3, &t3, &t5);             // t3^2
    SM2ModP_MontMul(&t2, &t5, &t2);             // (t1 + t2) * t3^2
    SM2ModP_MontSub(&Z->x, &t2, &Z->x);         // t6^2 - (t1 + t2) * t3^2

    // y3 = t6 * (t1 * t3^2 - x3) - t4 * t3^3
    SM2ModP_MontMul(&t1, &t5, &Z->y);           // t1 * t3^2
    SM2ModP_MontSub(&Z->y, &Z->x, &Z->y);       // t1 * t3^2 - x3
    SM2ModP_MontMul(&t6, &Z->y, &Z->y);         // t6 * (t1 * t3^2 - x3)
    SM2ModP_MontMul(&t4, &t3, &t3);             // t4 * t3
    SM2ModP_MontMul(&t3, &t5, &t3);             // t4 * t3^3
    SM2ModP_MontSub(&Z->y, &t3, &Z->y);         // t6 * (t1 * t3^2 - x3) - t4 * t3^3
}

void SM2JacobPointMont_Add(const SM2JacobPointMont* X, const SM2JacobPointMont* Y, SM2JacobPointMont* Z)
{
    if (SM2JacobPointMont_IsInf(X))
    {
        *Z = *Y;
    }
    else if (SM2JacobPointMont_IsInf(Y))
    {
        *Z = *X;
    }
    else
    {
        _SM2JacobPointMont_Add(X, Y, Z);
    }
}

void SM2JacobPointMont_Mul(const UInt256* k, const SM2JacobPointMont* X, SM2JacobPointMont* Y)
{
    int32_t i;
    uint32_t j;
    uint64_t tmp = 0;
    SM2JacobPointMont Y_tmp = { 0 };
    SM2JacobPointMont_SetInf(&Y_tmp);

    for (i = 3; i >= 0; i--)
    {
        tmp = k->u64[i];
        for (j = 0; j < 64; j++)
        {
            SM2JacobPointMont_Add(&Y_tmp, &Y_tmp, &Y_tmp);
            if (tmp & 0x8000000000000000)
            {
                SM2JacobPointMont_Add(&Y_tmp, X, &Y_tmp);
            }
            tmp <<= 1;
        }
    }

    *Y = Y_tmp;
}

void SM2JacobPointMont_Neg(const SM2JacobPointMont* X, SM2JacobPointMont* Y)
{
    Y->x = X->x;
    SM2ModP_MontNeg(&X->y, &Y->y);
    Y->z = X->z;
}

void SM2JacobPointMont_Sub(const SM2JacobPointMont* X, const SM2JacobPointMont* Y, SM2JacobPointMont* Z)
{
    SM2JacobPointMont neg_Y = { 0 };
    SM2JacobPointMont_Neg(Y, &neg_Y);
    SM2JacobPointMont_Add(X, &neg_Y, Z);
}

void SM2JacobPointMont_MulG(const UInt256* k, SM2JacobPointMont* X)
{
    SM2JacobPointMont_Mul(k, CONSTS_JACOB_G, X);
}

#ifdef _DEBUG

void SM2Point_Print(const SM2Point* X)
{
    printf("{ ");
    if (X->is_inf)
    {
        printf("<INF>");
    }
    else
    {
        printf("x: ");
        UInt256_Print(&X->x, 4);
        printf(",\n");
        printf("  y: ");
        UInt256_Print(&X->y, 4);
    }
    printf(" }\n");
}

void SM2JacobPointMont_Print(const SM2JacobPointMont* X)
{
    SM2Point t = { 0 };
    printf("{ ");
    printf("x: ");
    UInt256_Print(&X->x, 4);
    printf(", \n");
    printf("  y: ");
    UInt256_Print(&X->y, 4);
    printf(", \n");
    printf("  z: ");
    UInt256_Print(&X->z, 4);
    printf(" }\n");

    SM2JacobPointMont_ToPoint(X, &t);
    SM2Point_Print(&t);
}

#endif // _DEBUG
