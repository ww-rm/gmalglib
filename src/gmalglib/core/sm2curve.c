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

// 0xFFFFFFFE_FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF_00000000_FFFFFFFF_FFFFFFFC
static const UInt256 _CONSTS_A = { .u32 = {
    0xFFFFFFFC, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE
} };
static const UInt256* const CONSTS_A = &_CONSTS_A;

// 0x28E9FA9E_9D9F5E34_4D5A9E4B_CF6509A7_F39789F5_15AB8F92_DDBCBD41_4D940E93
static const UInt256 _CONSTS_B = { .u32 = {
    0x4D940E93, 0xDDBCBD41, 0x15AB8F92, 0xF39789F5,
    0xCF6509A7, 0x4D5A9E4B, 0x9D9F5E34, 0x28E9FA9E
} };
static const UInt256* const CONSTS_B = &_CONSTS_B;

// 0x32C4AE2C_1F198119_5F990446_6A39C994_8FE30BBF_F2660BE1_715A4589_334C74C7
static const SM2ModP _CONSTS_GX = { .u32 = {
    0x334C74C7, 0x715A4589, 0xF2660BE1, 0x8FE30BBF, 
    0x6A39C994, 0x5F990446, 0x1F198119, 0x32C4AE2C
} };
static const SM2ModP* const CONSTS_GX = &_CONSTS_GX;

// 0xBC3736A2_F4F6779C_59BDCEE3_6B692153_D0A9877C_C62A4740_02DF32E5_2139F0A0
static const SM2ModP _CONSTS_GY = { .u32 = {
    0x2139F0A0, 0x02DF32E5, 0xC62A4740, 0xD0A9877C, 
    0x6B692153, 0x59BDCEE3, 0xF4F6779C, 0xBC3736A2
} };
static const SM2ModP* const CONSTS_GY = &_CONSTS_GY;

// 0xFFFFFFFE_FFFFFFFF_FFFFFFFF_FFFFFFFF_7203DF6B_21C6052B_53BBF409_39D54123
static const UInt256 _CONSTS_N = { .u32 = {
    0x39D54123, 0x53BBF409, 0x21C6052B, 0x7203DF6B, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE
} };
static const UInt256* const CONSTS_N = &_CONSTS_N;

const UInt256* const SM2_PARAMS_P = &_CONSTS_P;
const UInt256* const SM2_PARAMS_A = &_CONSTS_A;
const UInt256* const SM2_PARAMS_B = &_CONSTS_B;
const UInt256* const SM2_PARAMS_GX = &_CONSTS_GX;
const UInt256* const SM2_PARAMS_GY = &_CONSTS_GY;
const UInt256* const SM2_PARAMS_N = &_CONSTS_N;

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

// 1
static const SM2ModP _CONSTS_MODP_ONE = { .u64 = {  1, 0, 0, 0 } };
static const SM2ModP* const CONSTS_MODP_ONE = &_CONSTS_MODP_ONE;

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

void SM2ModP_Sub(const SM2ModP* x, const SM2ModP* y, SM2ModP* z)
{
    if (UInt256_Sub(x, y, z))
    {
        UInt256_Sub(z, CONSTS_NEG_P, z);
    }
}

void SM2ModP_ToMont(const SM2ModP* x, SM2ModPMont* y)
{
    SM2ModP_MontMul(x, CONSTS_MODP_R2, y);
}

void SM2ModP_FromMont(const SM2ModPMont* x, SM2ModP* y)
{
    SM2ModP_MontMul(x, CONSTS_MODP_ONE, y);
}

void SM2ModP_MontAdd(const SM2ModPMont* x, const SM2ModPMont* y, SM2ModPMont* z)
{
    SM2ModP_Add(x, y, z);
}

void SM2ModP_MontSub(const SM2ModPMont* x, const SM2ModPMont* y, SM2ModPMont* z)
{
    SM2ModP_Sub(x, y, z);
}

void SM2ModP_MontMul(const SM2ModPMont* x, const SM2ModPMont* y, SM2ModPMont* z)
{
    UInt512 xy = { 0 };
    UInt512 z_tmp = { 0 };
    uint8_t carry = 0;

    UInt256_Mul(x, y, &xy);
    UInt256_Mul(&xy, CONSTS_P_PRIME, &z_tmp);
    UInt256_Mul(&z_tmp, CONSTS_P, &z_tmp);

    carry = UInt512_Add(&xy, &z_tmp, &z_tmp);
    (*z) = z_tmp.u256[1];
    
    if (carry)
    {
        UInt256_Add(z, CONSTS_NEG_P, z);
    }
    else if (UInt256_Cmp(z, CONSTS_P) >= 0)
    {
        UInt256_Sub(z, CONSTS_P, z);
    }
}

void SM2ModP_MontPow(const SM2ModPMont* x, const UInt256* e, SM2ModPMont* y)
{
    int32_t i;
    uint32_t j;
    uint64_t tmp = 0;
    SM2ModPMont y_tmp = { 0 };
    y_tmp = *CONSTS_MODP_MONT_ONE;

    for (i = 3; i >= 0; i--)
    {
        tmp = e->u64[i];
        for (j = 0; j < 64; j++)
        {
            SM2ModP_MontMul(&y_tmp, &y_tmp, &y_tmp);
            if (tmp & 0x8000000000000000)
            {
                SM2ModP_MontMul(&y_tmp, x, &y_tmp);
            }
        }
        tmp <<= 1;
    }

    *y = y_tmp;
}

void SM2ModP_MontNeg(const SM2ModPMont* x, SM2ModPMont* y)
{
    SM2ModP_MontSub(CONSTS_P, x, y);
}

void SM2ModP_MontInv(const SM2ModPMont* x, SM2ModPMont* y)
{
    SM2ModP_MontPow(x, CONSTS_P_MINUS_TWO, y);
}

void SM2Point_ToJacobMont(const SM2Point* X, SM2JacobPointMont* Y)
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

void SM2Point_FromJacobMont(const SM2JacobPointMont* X, SM2Point* Y)
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
            SM2ModP_MontMul(&X->y, &z_inv, &Y->y);

            SM2ModP_FromMont(&Y->x, &Y->x);
            SM2ModP_FromMont(&Y->y, &Y->y);
        }
    }
}

int SM2JacobPointMont_IsInf(const SM2JacobPointMont* X)
{
    return X->z.u64[0] == 0 && X->z.u64[1] == 0 && X->z.u64[2] == 0 && X->z.u64[3] == 0;
}

void SM2JacobPointMont_SetInf(SM2JacobPointMont* X)
{
    X->x = *CONSTS_MODP_MONT_ONE;
    X->y = *CONSTS_MODP_MONT_ONE;
    X->z.u64[0] = X->z.u64[1] = X->z.u64[2] = X->z.u64[3] = 0;
}

void SM2JacobPointMont_Add(const SM2JacobPointMont* X, const SM2JacobPointMont* Y, SM2JacobPointMont* Z)
{

}

void SM2JacobPointMont_Dbl(const SM2JacobPointMont* X, SM2JacobPointMont* Y)
{
    // order is IMPORTANT to avoid data overwrite

    SM2ModPMont lambda1 = { 0 };
    SM2ModPMont lambda2 = { 0 };
    SM2ModPMont lambda3 = { 0 };

    // a == -3 (mod p)
    // lambda1 = 3(x + z^2)(x - z^2)
    SM2ModP_MontMul(&X->z, &X->z, &lambda1);        // z^2
    SM2ModP_MontSub(&X->x, &lambda1, &lambda2);     // x - z^2
    SM2ModP_MontAdd(&X->x, &lambda1, &lambda1);     // x + z^2
    SM2ModP_MontMul(&lambda1, &lambda2, &lambda1);  // (x + z^2)(x - z^2)
    SM2ModP_MontAdd(&lambda1, &lambda1, &lambda2);  // 2(x + z^2)(x - z^2)
    SM2ModP_MontAdd(&lambda1, &lambda2, &lambda1);  // 3(x + z^2)(x - z^2)

    SM2ModP_MontAdd(&X->y, &X->y, &lambda3);        // 2y
    SM2ModP_MontMul(&lambda3, &lambda3, &lambda2);  // 4y^2
    SM2ModP_MontMul(&lambda2, &lambda3, &lambda3);  // 8y^3

    // lambda2 = 4xy^2
    SM2ModP_MontMul(&X->x, &lambda2, &lambda2);     // 4xy^2

    // lambda3 = 8y^4
    SM2ModP_MontAdd(&X->y, &lambda3, &lambda3);     // 8y^4

    // z' = 2yz
    SM2ModP_MontMul(&X->y, &X->z, &Y->z);
    SM2ModP_MontAdd(&Y->z, &Y->z, &Y->z);

    // y' = lambda1(lambda2 - x') - lambda3
    SM2ModP_MontSub(&lambda2, &Y->x, &Y->y);
    SM2ModP_MontMul(&lambda1, &Y->y, &Y->y);
    SM2ModP_MontSub(&Y->y, &lambda3, &Y->y);

    // x' = lambda1^2 - 2lambda2
    SM2ModP_MontMul(&lambda1, &lambda1, &Y->x);
    SM2ModP_MontAdd(&lambda2, &lambda2, &lambda1);
    SM2ModP_MontSub(&Y->x, &lambda1, &Y->x);
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
            SM2JacobPointMont_Dbl(&Y_tmp, &Y_tmp, &Y_tmp);
            if (tmp & 0x8000000000000000)
            {
                SM2JacobPointMont_Add(&Y_tmp, &X, &Y_tmp);
            }
        }
        tmp <<= 1;
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

#ifdef _DEBUG

void SM2JacobPointMont_Print(const SM2JacobPointMont* X)
{
    SM2Point pt = { 0 };
    SM2Point_FromJacobMont(X, &pt);
    printf("{ ");
    if (pt.is_inf)
    {
        printf("<INF>");
    }
    else
    {
        UInt256_Print(&pt.x, 4);
        printf(", \n");
        UInt256_Print(&pt.y, 4);
    }
    printf(" }\n");
}

#endif // _DEBUG
