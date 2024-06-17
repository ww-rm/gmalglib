#include <stdint.h>
#include <gmalglib/bignum.h>
#include <gmalglib/sm9curve.h>
#include <gmalglib/sm9table.h>

#ifdef _DEBUG

#include <stdio.h>

#endif // _DEBUG

#define SCALARMUL_WSIZE          4
#define SCALARMUL_TSIZE          (1 << (SCALARMUL_WSIZE - 1))

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Constants >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// 0xB6400000_02A3A6F1_D603AB4F_F58EC745_21F2934B_1A7AEEDB_E56F9B27_E351457D
static const UInt256 _CONSTS_P = { .u32 = { 0xE351457D, 0xE56F9B27, 0x1A7AEEDB, 0x21F2934B, 0xF58EC745, 0xD603AB4F, 0x02A3A6F1, 0xB6400000 } };
static const UInt256* const CONSTS_P = &_CONSTS_P;

static const UInt256 _CONSTS_B = { .u64 = { 0x0000000000000005, 0x0, 0x0, 0x0 } };

// 0x93DE051D_62BF718F_F5ED0704_487D01D6_E1E40869_09DC3280_E8C4E481_7C66DDDD
// 0x21FE8DDA_4F21E607_63106512_5C395BBC_1C1C00CB_FA602435_0C464CD7_0A3EA616
static const SM9Point1 _CONSTS_POINT_G1 = {
    {.u32 = { 0x7C66DDDD, 0xE8C4E481, 0x09DC3280, 0xE1E40869, 0x487D01D6, 0xF5ED0704, 0x62BF718F, 0x93DE051D } },
    {.u32 = { 0x0A3EA616, 0x0C464CD7, 0xFA602435, 0x1C1C00CB, 0x5C395BBC, 0x63106512, 0x4F21E607, 0x21FE8DDA } },
    0
};

// 0x85AEF3D0_78640C98_597B6027_B441A01F_F1DD2C19_0F5E93C4_54806C11_D8806141, 0x37227552_92130B08_D2AAB97F_D34EC120_EE265948_D19C17AB_F9B7213B_AF82D65B
// 0x17509B09_2E845C12_66BA0D26_2CBEE6ED_0736A96F_A347C8BD_856DC76B_84EBEB96, 0xA7CF28D5_19BE3DA6_5F317015_3D278FF2_47EFBA98_A71A0811_6215BBA5_C999A7C7
static const SM9Point2 _CONSTS_POINT_G2 = {
    {.fp1 = {{.u32 = { 0xAF82D65B, 0xF9B7213B, 0xD19C17AB, 0xEE265948, 0xD34EC120, 0xD2AAB97F, 0x92130B08, 0x37227552 }},
             {.u32 = { 0xD8806141, 0x54806C11, 0x0F5E93C4, 0xF1DD2C19, 0xB441A01F, 0x597B6027, 0x78640C98, 0x85AEF3D0 }}}},
    {.fp1 = {{.u32 = { 0xC999A7C7, 0x6215BBA5, 0xA71A0811, 0x47EFBA98, 0x3D278FF2, 0x5F317015, 0x19BE3DA6, 0xA7CF28D5 }}, 
             {.u32 = { 0x84EBEB96, 0x856DC76B, 0xA347C8BD, 0x0736A96F, 0x2CBEE6ED, 0x66BA0D26, 0x2E845C12, 0x17509B09 }}}},
    0
};

// 0xB6400000_02A3A6F1_D603AB4F_F58EC744_49F2934B_18EA8BEE_E56EE19C_D69ECF25
static const UInt256 _CONSTS_N = { .u32 = { 0xD69ECF25, 0xE56EE19C, 0x18EA8BEE, 0x49F2934B, 0xF58EC744, 0xD603AB4F, 0x02A3A6F1, 0xB6400000 } };

const UInt256* const SM9_PARAMS_P = &_CONSTS_P;
// NOTE: Parameter a is 0 in SM9
const UInt256* const SM9_PARAMS_B = &_CONSTS_B;
const SM9Point1* const SM9_PARAMS_G1 = &_CONSTS_POINT_G1;
const SM9Point2* const SM9_PARAMS_G2 = &_CONSTS_POINT_G2;
const UInt256* const SM9_PARAMS_N = &_CONSTS_N;

// 1, used for FromMont
static const UInt256 _CONSTS_ONE = { .u64 = {  1, 0, 0, 0 } };
static const SM9FP1* const CONSTS_FP1_ONE = &_CONSTS_ONE;

// p = 8u + 5, used for HasSqrt
static const UInt256 _CONSTS_U = { .u32 = { 0xFC6A28AF, 0x7CADF364, 0x634F5DDB, 0xA43E5269, 0xFEB1D8E8, 0x3AC07569, 0x005474DE, 0x16C80000 } };
static const UInt256* const CONSTS_U = &_CONSTS_U;
static const UInt256 _CONSTS_U_PLUS_ONE = { .u32 = { 0xFC6A28B0, 0x7CADF364, 0x634F5DDB, 0xA43E5269, 0xFEB1D8E8, 0x3AC07569, 0x005474DE, 0x16C80000 } };
static const UInt256* const CONSTS_U_PLUS_ONE = &_CONSTS_U_PLUS_ONE;
static const UInt256 _CONSTS_2U_PLUS_ONE = { .u32 = { 0xF8D4515F, 0xF95BE6C9, 0xC69EBBB6, 0x487CA4D2, 0xFD63B1D1, 0x7580EAD3, 0x00A8E9BC, 0x2D900000 } };
static const UInt256* const CONSTS_2U_PLUS_ONE = &_CONSTS_2U_PLUS_ONE;
static const SM9FP1Mont _CONSTS_FP1_MONT_P_MINUS_ONE = { .u32 = { 0xC6A28AFA, 0xCADF364F, 0x34F5DDB7, 0x43E52696, 0xEB1D8E8A, 0xAC07569F, 0x05474DE3, 0x6C800000 } };
static const SM9FP1Mont* const CONSTS_FP1_MONT_P_MINUS_ONE = &_CONSTS_FP1_MONT_P_MINUS_ONE;

// p - 2, used for Inv
static const UInt256 _CONSTS_P_MINUS_TWO = { .u32 = { 0xE351457B, 0xE56F9B27, 0x1A7AEEDB, 0x21F2934B, 0xF58EC745, 0xD603AB4F, 0x02A3A6F1, 0xB6400000 } };
static const UInt256* const CONSTS_P_MINUS_TWO = &_CONSTS_P_MINUS_TWO;

// -p, used for ModP operations
static const UInt256 _CONSTS_NEG_P = { .u32 = { 0x1CAEBA83, 0x1A9064D8, 0xE5851124, 0xDE0D6CB4, 0x0A7138BA, 0x29FC54B0, 0xFD5C590E, 0x49BFFFFF } };
static const UInt256* const CONSTS_NEG_P = &_CONSTS_NEG_P;
static const SM9FP1Mont* const CONSTS_FP1_MONT_ONE = &_CONSTS_NEG_P;

// -1 / p, used for MontMul
static const UInt256 _CONSTS_P_PRIME = { .u32 = { 0x2F2EE42B, 0x892BC42C, 0x13C8DBAF, 0x181AE396, 0x1522B137, 0x966A4B29, 0x558A13B3, 0xAFD2BAC5 } };
static const UInt256* const CONSTS_P_PRIME = &_CONSTS_P_PRIME;

// (2^256)^2, used for ToMont
static const SM9FP1 _CONSTS_FP1_R2 = { .u32 = { 0xB417E2D2, 0x27DEA312, 0xAE1A5D3F, 0x88F8105F, 0xD6706E7B, 0xE479B522, 0x56F62FBD, 0x2EA795A6 } };
static const SM9FP1* const CONSTS_FP1_R2 = &_CONSTS_FP1_R2;

// bR
static const SM9FP2Mont _CONSTS_FP2_MONT_BETA_B = { .fp1 = {
    { .u64 = { 0x0, 0x0, 0x0, 0x0 }},
    { .u32 = { 0xC8C71995, 0xB9F2C1E8, 0x46A377FC, 0x125DF8F2, 0x49188D1C, 0x25E650D0, 0xED866F63, 0x043FFFFF }}
}};
static const SM9FP2Mont* const CONSTS_FP2_MONT_BETA_B = &_CONSTS_FP2_MONT_BETA_B;
static const SM9FP1Mont* const CONSTS_FP1_MONT_B = &_CONSTS_FP2_MONT_BETA_B.fp1[1];

// G1 Point
static const SM9JacobPoint1Mont _CONSTS_JACOB_G1 = {
    {.u32 = { 0x9860501B, 0x22E935E2, 0x0073282C, 0xA946FD5E, 0x17A649BE, 0xEFD0CEC8, 0x869140B5, 0x5129787C } },
    {.u32 = { 0xEB87F7C7, 0xEE779649, 0xEC30A576, 0x15563CBD, 0x2824EFBF, 0x32635391, 0x63C39828, 0x72157177 } },
    {.u32 = { 0x1CAEBA83, 0x1A9064D8, 0xE5851124, 0xDE0D6CB4, 0x0A7138BA, 0x29FC54B0, 0xFD5C590E, 0x49BFFFFF } }
};
static const SM9JacobPoint1Mont* const CONSTS_JACOB_G1 = &_CONSTS_JACOB_G1;

// G2 Point
static const SM9JacobPoint2Mont _CONSTS_JACOB_G2 = {
    {.fp1 = {{.u32 = { 0x8CE2DA8F, 0x260226A6, 0xDBF6C06B, 0x7EE5645E, 0xB1495444, 0xF8F57C82, 0xBC47C4D1, 0x61FCF018 }},
             {.u32 = { 0x2750A8A6, 0xDB6DB482, 0x5121F134, 0x84C6135A, 0x88791D41, 0x1874032F, 0xB85F3A37, 0x905112F2 }}}},
    {.fp1 = {{.u32 = { 0x9171C24A, 0xC03F138F, 0xA15A3CA7, 0x92FBAB45, 0x2FF77CDB, 0x2445561E, 0xC0F62ECE, 0x108495E0 }},
             {.u32 = { 0x4C89BFBB, 0xF7B82DAC, 0xA49DC12F, 0x3706F3F6, 0xD3EEF769, 0x1E29DE93, 0xC76A5D53, 0x81E448C3 }}}},
    {.fp1 = {{.u32 = { 0x1CAEBA83, 0x1A9064D8, 0xE5851124, 0xDE0D6CB4, 0x0A7138BA, 0x29FC54B0, 0xFD5C590E, 0x49BFFFFF }},
             {.u64 = { 0x0, 0x0, 0x0, 0x0 }}}}
};
static const SM9JacobPoint2Mont* const CONSTS_JACOB_G2 = &_CONSTS_JACOB_G2;

// factors used in FP12 frobenius

// 0x2D40A38C_F6983351_711E5F99_520347CC_57D778A9_F8FF4C8A_4C949C7F_A2A96686 * 2^256
static const SM9FP1Mont _CONSTS_FP1_MONT_FP12_FROB_F5 = { .u32 = { 0xDCC34107, 0x048BAA79, 0xFE76C161, 0x5E2E7AC4, 0x365BD4BC, 0x99399754, 0x819B0E13, 0xAF91AEAC } };
static const SM9FP1Mont* const CONSTS_FP1_MONT_FP12_FROB_F5 = &_CONSTS_FP1_MONT_FP12_FROB_F5;

// 0x00000000_00000000_F3000000_02A3A6F2_78027235_4F8B78F4_D5FC1196_7BE65333 * 2^256
static const SM9FP1Mont _CONSTS_FP1_MONT_FP12_FROB_F4 = { .u32 = { 0x94E9C1C4, 0x81054FCD, 0x8CE2DF3E, 0x4C0E91CB, 0xE8AEDFB4, 0x4877B452, 0x8B491776, 0x88F53E74 } };
static const SM9FP1Mont* const CONSTS_FP1_MONT_FP12_FROB_F4 = &_CONSTS_FP1_MONT_FP12_FROB_F4;

// 0x6C648DE5_DC0A3F2C_F55ACC93_EE0BAF15_9F9D4118_06DC5177_F5B21FD3_DA24D011 * 2^256
static const SM9FP1Mont _CONSTS_FP1_MONT_FP12_FROB_F3 = { .u32 = { 0x3EE72529, 0x39B4EF0F, 0x08582782, 0xDB043BF5, 0x54AC91E3, 0xB8554AB0, 0x5498CAB5, 0x9848EEC2 } };
static const SM9FP1Mont* const CONSTS_FP1_MONT_FP12_FROB_F3 = &_CONSTS_FP1_MONT_FP12_FROB_F3;

// 0x00000000_00000000_F3000000_02A3A6F2_78027235_4F8B78F4_D5FC1196_7BE65334 * 2^256
static const SM9FP1Mont _CONSTS_FP1_MONT_FP12_FROB_F2 = { .u32 = { 0xCE4736CA, 0xB626197D, 0x57ED0186, 0x08296B35, 0xFD91512A, 0x9C705DB2, 0x8601C992, 0x1C753E74 } };
static const SM9FP1Mont* const CONSTS_FP1_MONT_FP12_FROB_F2 = &_CONSTS_FP1_MONT_FP12_FROB_F2;

// 0x3F23EA58_E5720BDB_843C6CFA_9C086749_47C5C86E_0DDD04ED_A91D8354_377B698B * 2^256
static const SM9FP1Mont _CONSTS_FP1_MONT_FP12_FROB_F1 = { .u32 = { 0x4575299F, 0x1A98DFBD, 0x245C54FD, 0x9EC8547B, 0x13DF846C, 0xF51F5EAC, 0xD5A16393, 0x9EF74015 } };
static const SM9FP1Mont* const CONSTS_FP1_MONT_FP12_FROB_F1 = &_CONSTS_FP1_MONT_FP12_FROB_F1;

// CONSTS_FP1_MONT_FP12_FROB_F0 = 0x1 * 2^256

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Constants <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> FP1 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

static
void SM9FP1_Add(const SM9FP1* x, const SM9FP1* y, SM9FP1* z)
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
void SM9FP1_Sub(const SM9FP1* x, const SM9FP1* y, SM9FP1* z)
{
    if (UInt256_Sub(x, y, z))
    {
        UInt256_Sub(z, CONSTS_NEG_P, z);
    }
}

static
void SM9FP1_Neg(const SM9FP1* x, SM9FP1* y)
{
    if (UInt256_Sub(CONSTS_P, x, y))
    {
        UInt256_Sub(y, CONSTS_NEG_P, y);
    }
}

static
void SM9FP1_Div2(const SM9FP1* x, SM9FP1* y)
{
    uint64_t carry = 0;
    if (x->u64[0] & 0x1)
    {
        carry = UInt256_Add(x, CONSTS_P, y);
    }
    else
    {
        *y = *x;
    }

    y->u64[0] = (y->u64[0] >> 1) | ((y->u64[1] & 0x1) << 63);
    y->u64[1] = (y->u64[1] >> 1) | ((y->u64[2] & 0x1) << 63);
    y->u64[2] = (y->u64[2] >> 1) | ((y->u64[3] & 0x1) << 63);
    y->u64[3] = (y->u64[3] >> 1) | ((carry & 0x1) << 63);
}

static
void SM9FP1_MontMul(const SM9FP1Mont* x, const SM9FP1Mont* y, SM9FP1Mont* z)
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
    *z = z_tmp->u256[1];

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
void SM9FP1_ToMont(const SM9FP1* x, SM9FP1Mont* y)
{
    SM9FP1_MontMul(x, CONSTS_FP1_R2, y);
}

static
void SM9FP1_FromMont(const SM9FP1Mont* x, SM9FP1* y)
{
    SM9FP1_MontMul(x, CONSTS_FP1_ONE, y);
}

static
void SM9FP1_MontPow(const SM9FP1Mont* x, const UInt256* e, SM9FP1Mont* y)
{
    int32_t i = 0;
    int32_t j = 0;
    int32_t w = 0;
    uint32_t wvalue = 0;
    SM9FP1Mont y_tmp = { 0 };

    // pre-compute, save odd points, 1, 3, 5, ..., 2^w - 1
    SM9FP1Mont table[SCALARMUL_TSIZE] = { *x };
    SM9FP1_MontMul(x, x, &y_tmp);
    for (i = 0; i < SCALARMUL_TSIZE - 1; i++)
    {
        SM9FP1_MontMul(table + i, &y_tmp, table + i + 1);
    }

    y_tmp = *CONSTS_FP1_MONT_ONE;
    i = 255;
    while (i >= 0)
    {
        wvalue = (e->u64[i / 64] >> (i % 64)) & 0x1;

        if (wvalue == 0)
        {
            SM9FP1_MontMul(&y_tmp, &y_tmp, &y_tmp);
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
                SM9FP1_MontMul(&y_tmp, &y_tmp, &y_tmp);
                i--;
            }

            SM9FP1_MontMul(&y_tmp, table + (wvalue >> 1), &y_tmp);
        }
    }

    *y = y_tmp;
}

static
int SM9FP1_MontHasSqrt(const SM9FP1Mont* x, SM9FP1Mont* y)
{
    SM9FP1Mont y_tmp = { 0 };

    SM9FP1_MontPow(x, CONSTS_2U_PLUS_ONE, &y_tmp);
    if (UInt256_Cmp(&y_tmp, CONSTS_FP1_MONT_ONE) == 0)
    {
        SM9FP1_MontPow(x, CONSTS_U_PLUS_ONE, &y_tmp);
    }
    else if (UInt256_Cmp(&y_tmp, CONSTS_FP1_MONT_P_MINUS_ONE) == 0)
    {
        SM9FP1_Add(x, x, &y_tmp);
        SM9FP1_Add(&y_tmp, &y_tmp, &y_tmp);
        SM9FP1_MontPow(&y_tmp, CONSTS_U, &y_tmp);
        SM9FP1_MontMul(&y_tmp, x, &y_tmp);
        SM9FP1_Add(&y_tmp, &y_tmp, &y_tmp);
    }
    else
        return 0;

    *y = y_tmp;
    return 1;
}

static
void SM9FP1_MontInv(const SM9FP1Mont* x, SM9FP1Mont* y)
{
    SM9FP1_MontPow(x, CONSTS_P_MINUS_TWO, y);
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FP1 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> FP1 Points >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

int SM9JacobPoint1Mont_IsInf(const SM9JacobPoint1Mont* X)
{
    return UInt256_IsZero(&X->z);
}

void SM9JacobPoint1Mont_SetInf(SM9JacobPoint1Mont* X)
{
    X->x = *CONSTS_FP1_MONT_ONE;
    X->y = *CONSTS_FP1_MONT_ONE;
    UInt256_SetZero(&X->z);
}

int SM9JacobPoint1Mont_IsOnCurve(const SM9JacobPoint1Mont* X)
{
    SM9FP1Mont left = { 0 };
    SM9FP1Mont right = { 0 };
    SM9FP1Mont tmp = { 0 };

    if (UInt256_Cmp(&X->z, CONSTS_FP1_MONT_ONE) == 0)
    {
        // left = y^2
        SM9FP1_MontMul(&X->y, &X->y, &left);

        // right = x^3 + b
        SM9FP1_MontMul(&X->x, &X->x, &right);
        SM9FP1_MontMul(&right, &X->x, &right);
        SM9FP1_Add(&right, CONSTS_FP1_MONT_B, &right);
    }
    else
    {

        // left = y^2
        SM9FP1_MontMul(&X->y, &X->y, &left);                    // y^2

        // right = x^3 + bz^6
        SM9FP1_MontMul(&X->z, &X->z, &tmp);                     // z^2
        SM9FP1_MontMul(&tmp, &tmp, &right);                     // z^4
        SM9FP1_MontMul(&tmp, &right, &right);                   // z^6
        SM9FP1_MontMul(CONSTS_FP1_MONT_B, &right, &right);      // bz^6
        SM9FP1_MontMul(&X->x, &X->x, &tmp);                     // x^2
        SM9FP1_MontMul(&tmp, &X->x, &tmp);                      // x^3
        SM9FP1_Add(&tmp, &right, &right);                       // x^3 + bz^6
    }

    return UInt256_Cmp(&left, &right) == 0;
}

int SM9JacobPoint1Mont_IsEqual(const SM9JacobPoint1Mont* X, const SM9JacobPoint1Mont* Y)
{
    SM9FP1Mont t1 = { 0 };
    SM9FP1Mont t2 = { 0 };
    SM9FP1Mont t4 = { 0 };
    SM9FP1Mont t5 = { 0 };

    if (UInt256_Cmp(&X->z, CONSTS_FP1_MONT_ONE) == 0 && UInt256_Cmp(&Y->z, CONSTS_FP1_MONT_ONE) == 0)
        return UInt256_Cmp(&X->x, &Y->x) == 0 && UInt256_Cmp(&X->y, &Y->y) == 0;

    // t1 = x1 * z2^2
    // t2 = x2 * z1^2
    // t4 = y1 * z2^3
    // t5 = y2 * z1^3
    SM9FP1_MontMul(&Y->z, &Y->z, &t4);         // z2^2
    SM9FP1_MontMul(&X->z, &X->z, &t5);         // z1^2

    SM9FP1_MontMul(&X->x, &t4, &t1);           // x1 * z2^2
    SM9FP1_MontMul(&Y->x, &t5, &t2);           // x2 * z1^2

    if (UInt256_Cmp(&t1, &t2) != 0)
        return 0;

    SM9FP1_MontMul(&X->y, &t4, &t4);           // y1 * z2^2
    SM9FP1_MontMul(&t4, &Y->z, &t4);           // y1 * z2^3

    SM9FP1_MontMul(&Y->y, &t5, &t5);           // y2 * z1^2
    SM9FP1_MontMul(&t5, &X->z, &t5);           // y2 * z1^3

    return UInt256_Cmp(&t4, &t5) == 0;
}

void SM9JacobPoint1Mont_ToPoint(const SM9JacobPoint1Mont* X, SM9Point1* Y)
{
    SM9FP1Mont z_inv = { 0 };
    if (SM9JacobPoint1Mont_IsInf(X))
    {
        Y->is_inf = 1;
    }
    else
    {
        Y->is_inf = 0;
        if (UInt256_Cmp(&X->z, CONSTS_FP1_MONT_ONE) == 0)
        {
            SM9FP1_FromMont(&X->x, &Y->x);
            SM9FP1_FromMont(&X->y, &Y->y);
        }
        else
        {
            SM9FP1_MontInv(&X->z, &z_inv);
            SM9FP1_MontMul(&X->y, &z_inv, &Y->y);
            SM9FP1_MontMul(&z_inv, &z_inv, &z_inv);
            SM9FP1_MontMul(&X->x, &z_inv, &Y->x);
            SM9FP1_MontMul(&Y->y, &z_inv, &Y->y);

            SM9FP1_FromMont(&Y->x, &Y->x);
            SM9FP1_FromMont(&Y->y, &Y->y);
        }
    }
}

void SM9JacobPoint1Mont_FromPoint(const SM9Point1* X, SM9JacobPoint1Mont* Y)
{
    if (X->is_inf)
    {
        SM9JacobPoint1Mont_SetInf(Y);
    }
    else
    {
        SM9FP1_ToMont(&X->x, &Y->x);
        SM9FP1_ToMont(&X->y, &Y->y);
        Y->z = *CONSTS_FP1_MONT_ONE;
    }
}

static
int SM9Point1_IsOnCurve(const SM9Point1* X)
{
    SM9FP1Mont left = { 0 };
    SM9FP1Mont right = { 0 };
    SM9FP1Mont x = { 0 };
    SM9FP1Mont y = { 0 };

    if (X->is_inf)
        return 1;

    SM9FP1_ToMont(&X->x, &x);
    SM9FP1_ToMont(&X->y, &y);

    // left = y^2
    SM9FP1_MontMul(&y, &y, &left);

    // right = x^3 + b
    SM9FP1_MontMul(&x, &x, &right);
    SM9FP1_MontMul(&right, &x, &right);
    SM9FP1_Add(&right, CONSTS_FP1_MONT_B, &right);

    return UInt256_Cmp(&left, &right) == 0;
}

uint64_t SM9Point1_ToBytes(const SM9Point1* X, int pc_mode, uint8_t* bytes)
{
    if (X->is_inf)
    {
        bytes[0] = 0x00;
        return 1;
    }

    if (pc_mode == SM9_PCMODE_COMPRESS)
    {
        if (X->y.u8[0] & 0x1)
            bytes[0] = 0x03;
        else
            bytes[0] = 0x02;
        UInt256_ToBytes(&X->x, bytes + 1);
        return 33;
    }
    else if (pc_mode == SM9_PCMODE_MIX)
    {
        if (X->y.u8[0] & 0x1)
            bytes[0] = 0x07;
        else
            bytes[0] = 0x06;
        UInt256_ToBytes(&X->x, bytes + 1);
        UInt256_ToBytes(&X->y, bytes + 33);
        return 65;
    }
    else
    {
        bytes[0] = 0x04;
        UInt256_ToBytes(&X->x, bytes + 1);
        UInt256_ToBytes(&X->y, bytes + 33);
        return 65;
    }

    return 0;
}

int SM9Point1_FromBytes(const uint8_t* bytes, uint64_t bytes_len, SM9Point1* X)
{
    uint8_t pc = bytes[0];
    int ylsb = 0;

    if (pc == 0x00)
    {
        if (bytes_len != 1)
            return SM9CURVE_ERR_INVALIDPC;

        X->is_inf = 1;
    }
    else if (pc == 0x04 || pc == 0x06 || pc == 0x07)
    {
        if (bytes_len != SM9_POINT1BYTES_FULL_LENGTH)
            return SM9CURVE_ERR_INVALIDPC;

        UInt256_FromBytes(bytes + 1, &X->x);
        UInt256_FromBytes(bytes + 33, &X->y);
        X->is_inf = 0;
        if (!SM9Point1_IsOnCurve(X))
            return SM9CURVE_ERR_NOTONCURVE;
    }
    else if (pc == 0x02 || pc == 0x03)
    {
        if (bytes_len != SM9_POINT1BYTES_HALF_LENGTH)
            return SM9CURVE_ERR_INVALIDPC;

        UInt256_FromBytes(bytes + 1, &X->x);
        if (UInt256_Cmp(&X->x, CONSTS_P) >= 0)
            return SM9CURVE_ERR_NOTONCURVE;

        SM9FP1_ToMont(&X->x, &X->x);

        // compute y, x^3 + b
        SM9FP1_MontMul(&X->x, &X->x, &X->y);
        SM9FP1_MontMul(&X->x, &X->y, &X->y);
        SM9FP1_Add(&X->y, CONSTS_FP1_MONT_B, &X->y);
        if (!SM9FP1_MontHasSqrt(&X->y, &X->y))
            return SM9CURVE_ERR_NOTONCURVE;

        SM9FP1_FromMont(&X->x, &X->x);
        SM9FP1_FromMont(&X->y, &X->y);
        ylsb = X->y.u8[0] & 0x1;
        if ((pc == 0x02 && ylsb) || (pc == 0x03 && !ylsb))
        {
            SM9FP1_Neg(&X->y, &X->y);
        }
    }
    else
    {
        return SM9CURVE_ERR_INVALIDPC;
    }

    return 0;
}

uint64_t SM9JacobPoint1Mont_ToBytes(const SM9JacobPoint1Mont* X, int pc_mode, uint8_t* bytes)
{
    SM9Point1 pt = { 0 };
    SM9JacobPoint1Mont_ToPoint(X, &pt);
    return SM9Point1_ToBytes(&pt, pc_mode, bytes);
}

int SM9JacobPoint1Mont_FromBytes(const uint8_t* bytes, uint64_t bytes_len, SM9JacobPoint1Mont* X)
{
    SM9Point1 pt = { 0 };
    int ret = SM9Point1_FromBytes(bytes, bytes_len, &pt);
    if (ret != 0)
        return ret;

    SM9JacobPoint1Mont_FromPoint(&pt, X);
    return 0;
}

static
void _SM9JacobPoint1Mont_Dbl(const SM9JacobPoint1Mont* X, SM9JacobPoint1Mont* Y)
{
    // order is IMPORTANT to avoid data overwrite

    SM9FP1Mont t1 = { 0 };
    SM9FP1Mont t2 = { 0 };
    SM9FP1Mont t3 = { 0 };

    // a == 0 (mod p)
    // t1 = 3x^2
    SM9FP1_MontMul(&X->x, &X->x, &t1);          // x^2
    SM9FP1_Add(&t1, &t1, &t2);                  // 2x^2
    SM9FP1_Add(&t1, &t2, &t1);                  // 3x^2

    // z' = 2yz
    SM9FP1_Add(&X->y, &X->y, &t3);              // 2y
    SM9FP1_MontMul(&t3, &X->z, &Y->z);

    // t3 = 8y^4
    SM9FP1_MontMul(&t3, &t3, &t2);              // 4y^2
    SM9FP1_MontMul(&t2, &t2, &t3);              // 16y^4
    SM9FP1_Div2(&t3, &t3);                      // 8y^4

    // t2 = 4xy^2
    SM9FP1_MontMul(&X->x, &t2, &t2);            // 4xy^2

    // x' = t1^2 - 2t2
    SM9FP1_MontMul(&t1, &t1, &Y->x);
    SM9FP1_Sub(&Y->x, &t2, &Y->x);
    SM9FP1_Sub(&Y->x, &t2, &Y->x);

    // y' = t1(t2 - x') - t3
    SM9FP1_Sub(&t2, &Y->x, &Y->y);
    SM9FP1_MontMul(&t1, &Y->y, &Y->y);
    SM9FP1_Sub(&Y->y, &t3, &Y->y);
}

void SM9JacobPoint1Mont_Dbl(const SM9JacobPoint1Mont* X, SM9JacobPoint1Mont* Y)
{
    if (SM9JacobPoint1Mont_IsInf(X))
    {
        *Y = *X;
    }
    else
    {
        _SM9JacobPoint1Mont_Dbl(X, Y);
    }
}

static
void _SM9JacobPoint1Mont_Add(const SM9JacobPoint1Mont* X, const SM9JacobPoint1Mont* Y, SM9JacobPoint1Mont* Z)
{
    SM9FP1Mont t1 = { 0 };
    SM9FP1Mont t2 = { 0 };
    SM9FP1Mont t3 = { 0 };
    SM9FP1Mont t4 = { 0 };
    SM9FP1Mont t5 = { 0 };
    SM9FP1Mont t6 = { 0 };

    // t1 = x1 * z2^2
    // t2 = x2 * z1^2
    // t4 = y1 * z2^3
    // t5 = y2 * z1^3
    SM9FP1_MontMul(&Y->z, &Y->z, &t4);          // z2^2
    SM9FP1_MontMul(&X->z, &X->z, &t5);          // z1^2

    SM9FP1_MontMul(&X->x, &t4, &t1);            // x1 * z2^2
    SM9FP1_MontMul(&Y->x, &t5, &t2);            // x2 * z1^2

    SM9FP1_MontMul(&X->y, &t4, &t4);            // y1 * z2^2
    SM9FP1_MontMul(&t4, &Y->z, &t4);            // y1 * z2^3

    SM9FP1_MontMul(&Y->y, &t5, &t5);            // y2 * z1^2
    SM9FP1_MontMul(&t5, &X->z, &t5);            // y2 * z1^3

    if (UInt256_Cmp(&t1, &t2) == 0)
    {
        if (UInt256_Cmp(&t4, &t5) == 0)
            _SM9JacobPoint1Mont_Dbl(X, Z);
        else
            SM9JacobPoint1Mont_SetInf(Z);
        return;
    }

    // t3 = t1 - t2
    // t6 = t4 - t5
    SM9FP1_Sub(&t1, &t2, &t3);
    SM9FP1_Sub(&t4, &t5, &t6);

    // z3 = z1 * z2 * t3
    SM9FP1_MontMul(&X->z, &Y->z, &Z->z);        // z1 * z2
    SM9FP1_MontMul(&Z->z, &t3, &Z->z);          // z1 * z2 * t3

    // x3 = t6^2 - (t1 + t2) * t3^2
    SM9FP1_MontMul(&t6, &t6, &Z->x);            // t6^2
    SM9FP1_Add(&t1, &t2, &t2);                  // t1 + t2
    SM9FP1_MontMul(&t3, &t3, &t5);              // t3^2
    SM9FP1_MontMul(&t2, &t5, &t2);              // (t1 + t2) * t3^2
    SM9FP1_Sub(&Z->x, &t2, &Z->x);              // t6^2 - (t1 + t2) * t3^2

    // y3 = t6 * (t1 * t3^2 - x3) - t4 * t3^3
    SM9FP1_MontMul(&t1, &t5, &Z->y);            // t1 * t3^2
    SM9FP1_Sub(&Z->y, &Z->x, &Z->y);            // t1 * t3^2 - x3
    SM9FP1_MontMul(&t6, &Z->y, &Z->y);          // t6 * (t1 * t3^2 - x3)
    SM9FP1_MontMul(&t4, &t3, &t3);              // t4 * t3
    SM9FP1_MontMul(&t3, &t5, &t3);              // t4 * t3^3
    SM9FP1_Sub(&Z->y, &t3, &Z->y);              // t6 * (t1 * t3^2 - x3) - t4 * t3^3
}

void SM9JacobPoint1Mont_Add(const SM9JacobPoint1Mont* X, const SM9JacobPoint1Mont* Y, SM9JacobPoint1Mont* Z)
{
    if (SM9JacobPoint1Mont_IsInf(X))
    {
        *Z = *Y;
    }
    else if (SM9JacobPoint1Mont_IsInf(Y))
    {
        *Z = *X;
    }
    else
    {
        _SM9JacobPoint1Mont_Add(X, Y, Z);
    }
}

static
void _SM9JacobPoint1Mont_Mul_SlidingWindow(const UInt256* k, const SM9JacobPoint1Mont* X, SM9JacobPoint1Mont* Y)
{
    int32_t i = 0;
    int32_t j = 0;
    int32_t w = 0;
    uint32_t wvalue = 0;
    SM9JacobPoint1Mont Y_tmp = { 0 };

    // pre-compute, save odd points, 1, 3, 5, ..., 2^w - 1
    SM9JacobPoint1Mont table[SCALARMUL_TSIZE] = { *X };
    SM9JacobPoint1Mont_Dbl(X, &Y_tmp);
    for (i = 0; i < SCALARMUL_TSIZE - 1; i++)
    {
        SM9JacobPoint1Mont_Add(table + i, &Y_tmp, table + i + 1);
    }

    SM9JacobPoint1Mont_SetInf(&Y_tmp);
    i = 255;
    while (i >= 0)
    {
        wvalue = (k->u64[i / 64] >> (i % 64)) & 0x1;

        if (wvalue == 0)
        {
            SM9JacobPoint1Mont_Dbl(&Y_tmp, &Y_tmp);
            i--;
        }
        else
        {
            // find a longest 1...1 bits in window size
            j = i;
            for (w = i - 1; w >= 0 && w > i - SCALARMUL_WSIZE; w--)
            {
                if ((k->u64[w / 64] >> (w % 64)) & 0x1)
                {
                    wvalue = (wvalue << (j - w)) | 0x1;
                    j = w;
                }
            }

            while (i >= j)
            {
                SM9JacobPoint1Mont_Dbl(&Y_tmp, &Y_tmp);
                i--;
            }

            SM9JacobPoint1Mont_Add(&Y_tmp, table + (wvalue >> 1), &Y_tmp);
        }
    }

    *Y = Y_tmp;
}

void SM9JacobPoint1Mont_Mul(const UInt256* k, const SM9JacobPoint1Mont* X, SM9JacobPoint1Mont* Y)
{
    _SM9JacobPoint1Mont_Mul_SlidingWindow(k, X, Y);
}

void SM9JacobPoint1Mont_Neg(const SM9JacobPoint1Mont* X, SM9JacobPoint1Mont* Y)
{
    Y->x = X->x;
    SM9FP1_Neg(&X->y, &Y->y);
    Y->z = X->z;
}

void SM9JacobPoint1Mont_Sub(const SM9JacobPoint1Mont* X, const SM9JacobPoint1Mont* Y, SM9JacobPoint1Mont* Z)
{
    SM9JacobPoint1Mont neg_Y = { 0 };
    SM9JacobPoint1Mont_Neg(Y, &neg_Y);
    SM9JacobPoint1Mont_Add(X, &neg_Y, Z);
}

void SM9JacobPoint1Mont_MulG1(const UInt256* k, SM9JacobPoint1Mont* X)
{
    uint32_t i;
    uint32_t part_k;
    SM9JacobPoint1Mont_SetInf(X);

    for (i = 0; i < 32; i++)
    {
        part_k = k->u8[i];
        if (part_k > 0)
        {
            SM9JacobPoint1Mont_Add(X, SM9_MULG1_TABLE_U8 + i * 256 + part_k, X);
        }
    }
}

#ifdef _DEBUG

void SM9FP1Mont_Print(const SM9FP1Mont* x)
{
    SM9FP1 xx = { 0 };
    SM9FP1_FromMont(x, &xx);
    UInt256_Print(&xx, 4);
}

void SM9Point1_Print(const SM9Point1* X)
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

void SM9JacobPoint1Mont_Print(const SM9JacobPoint1Mont* X)
{
    SM9Point1 t = { 0 };
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

    SM9JacobPoint1Mont_ToPoint(X, &t);
    SM9Point1_Print(&t);
}

#endif // _DEBUG

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FP1 Points <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> FP2 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

static
void SM9FP2_Add(const SM9FP2* x, const SM9FP2* y, SM9FP2* z)
{
    SM9FP1_Add(x->fp1 + 1, y->fp1 + 1, z->fp1 + 1);
    SM9FP1_Add(x->fp1, y->fp1, z->fp1);
}

static
void SM9FP2_Sub(const SM9FP2* x, const SM9FP2* y, SM9FP2* z)
{
    SM9FP1_Sub(x->fp1 + 1, y->fp1 + 1, z->fp1 + 1);
    SM9FP1_Sub(x->fp1, y->fp1, z->fp1);
}

static
void SM9FP2_Neg(const SM9FP2* x, SM9FP2* y)
{
    SM9FP1_Neg(x->fp1 + 1, y->fp1 + 1);
    SM9FP1_Neg(x->fp1, y->fp1);
}

static
void SM9FP2_Div2(const SM9FP2* x, SM9FP2* y)
{
    SM9FP1_Div2(x->fp1 + 1, y->fp1 + 1);
    SM9FP1_Div2(x->fp1, y->fp1);
}

static
void SM9FP2_MontMul(const SM9FP2Mont* x, const SM9FP2Mont* y, SM9FP2Mont* z)
{
    SM9FP2Mont z_tmp = { 0 };
    const SM9FP1Mont* x1 = x->fp1 + 1;
    const SM9FP1Mont* x0 = x->fp1;
    const SM9FP1Mont* y1 = y->fp1 + 1;
    const SM9FP1Mont* y0 = y->fp1;
    SM9FP1Mont* z1 = z_tmp.fp1 + 1;
    SM9FP1Mont* z0 = z_tmp.fp1;

    SM9FP1Mont _x1y1 = { 0 };
    SM9FP1Mont _x0y0 = { 0 };
    const SM9FP1Mont* x1y1 = &_x1y1;
    const SM9FP1Mont* x0y0 = &_x0y0;

    SM9FP1_MontMul(x1, y1, &_x1y1);
    SM9FP1_MontMul(x0, y0, &_x0y0);

    // z1 = (x1 + x0)(y1 + y0) - x1y1 - x0y0
    SM9FP1_Add(x1, x0, z1);
    SM9FP1_Add(y1, y0, z0);
    SM9FP1_MontMul(z1, z0, z1);
    SM9FP1_Sub(z1, x1y1, z1);
    SM9FP1_Sub(z1, x0y0, z1);

    // z0 = x0y0 - 2(x1y1)
    SM9FP1_Sub(x0y0, x1y1, z0);
    SM9FP1_Sub(z0, x1y1, z0);

    *z = z_tmp;
}

static inline
void _SM9FP2_MontMulU(const SM9FP2Mont* x, SM9FP2Mont* y)
{
    // U = (1, 0)
    // (1, 0) * (a1, a0) = (a0, -2a1)
    y->fp1[1] = x->fp1[0];
    SM9FP1_Add(x->fp1 + 1, x->fp1 + 1, y->fp1);
    SM9FP1_Neg(y->fp1, y->fp1);
}

static
void SM9FP2_MontMulFP1(const SM9FP2Mont* x, const SM9FP1Mont* y, SM9FP2Mont* z)
{
    SM9FP1_MontMul(x->fp1 + 1, y, z->fp1 + 1);
    SM9FP1_MontMul(x->fp1, y, z->fp1);
}

static
void SM9FP2_ToMont(const SM9FP2* x, SM9FP2Mont* y)
{
    SM9FP1_ToMont(x->fp1 + 1, y->fp1 + 1);
    SM9FP1_ToMont(x->fp1, y->fp1);
}

static
void SM9FP2_FromMont(const SM9FP2Mont* x, SM9FP2* y)
{
    SM9FP1_FromMont(x->fp1 + 1, y->fp1 + 1);
    SM9FP1_FromMont(x->fp1, y->fp1);
}

static
int SM9FP2_MontHasSqrt(const SM9FP2Mont* x, SM9FP2Mont* y)
{
    SM9FP2Mont y_tmp = { 0 };
    const SM9FP1Mont* x1 = x->fp1 + 1;
    const SM9FP1Mont* x0 = x->fp1;
    SM9FP1Mont* y1 = y_tmp.fp1 + 1;
    SM9FP1Mont* y0 = y_tmp.fp1;

    // w^2 = x0^2 + 2(x1^2)
    SM9FP1Mont w = { 0 };
    SM9FP1_MontMul(x0, x0, y0);
    SM9FP1_MontMul(x1, x1, y1);
    SM9FP1_Add(y0, y1, &w);
    SM9FP1_Add(&w, y1, &w);
    if (!SM9FP1_MontHasSqrt(&w, &w))
        return 0;

    // y0^2 = (x0 + w) / 2, or y0^2 = (x0 - w) / 2
    SM9FP1_Add(x0, &w, y0);
    SM9FP1_Div2(y0, y0);
    if (!SM9FP1_MontHasSqrt(y0, y0))
    {
        // try negative w
        SM9FP1_Sub(x0, &w, y0);
        SM9FP1_Div2(y0, y0);
        if (!SM9FP1_MontHasSqrt(y0, y0))
            return 0;
    }

    // y1 = x1 / 2y0
    SM9FP1_MontInv(y0, y1);
    SM9FP1_MontMul(x1, y1, y1);
    SM9FP1_Div2(y1, y1);

    *y = y_tmp;
    return 1;
}

static
void SM9FP2_MontInv(const SM9FP2Mont* x, SM9FP2Mont* y)
{
    const SM9FP1Mont* x1 = x->fp1 + 1;
    const SM9FP1Mont* x0 = x->fp1;

    SM9FP2Mont y_tmp = { 0 };
    SM9FP1Mont* y1 = y_tmp.fp1 + 1;
    SM9FP1Mont* y0 = y_tmp.fp1;

    // det = x0^2 + 2(x1^2)
    SM9FP1_MontMul(x0, x0, y0);
    SM9FP1_MontMul(x1, x1, y1);
    SM9FP1_Add(y0, y1, y0);
    SM9FP1_Add(y0, y1, y0);
    SM9FP1_MontInv(y0, y0);

    // y1 =  -x1 / det
    SM9FP1_MontMul(y0, x1, y1);
    SM9FP1_Neg(y1, y1);

    // y0 = x0 / det
    SM9FP1_MontMul(y0, x0, y0);

    *y = y_tmp;
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FP2 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> FP2 Points >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

int SM9JacobPoint2Mont_IsInf(const SM9JacobPoint2Mont* X)
{
    return UInt256_IsZero(X->z.fp1 + 1) && UInt256_IsZero(X->z.fp1);
}

void SM9JacobPoint2Mont_SetInf(SM9JacobPoint2Mont* X)
{
    UInt256_SetZero(X->x.fp1 + 1); X->x.fp1[0] = *CONSTS_FP1_MONT_ONE;
    UInt256_SetZero(X->y.fp1 + 1); X->y.fp1[0] = *CONSTS_FP1_MONT_ONE;
    UInt256_SetZero(X->z.fp1 + 1); UInt256_SetZero(X->z.fp1);
}

int SM9JacobPoint2Mont_IsOnCurve(const SM9JacobPoint2Mont* X)
{
    SM9FP2Mont left = { 0 };
    SM9FP2Mont right = { 0 };
    SM9FP2Mont tmp = { 0 };

    if (UInt256_IsZero(X->z.fp1 + 1) && UInt256_Cmp(X->z.fp1, CONSTS_FP1_MONT_ONE) == 0)
    {
        // left = y^2
        SM9FP2_MontMul(&X->y, &X->y, &left);

        // right = x^3 + b
        SM9FP2_MontMul(&X->x, &X->x, &right);
        SM9FP2_MontMul(&right, &X->x, &right);
        SM9FP2_Add(&right, CONSTS_FP2_MONT_BETA_B, &right);
    }
    else
    {

        // left = y^2
        SM9FP2_MontMul(&X->y, &X->y, &left);                        // y^2

        // right = x^3 + bz^6
        SM9FP2_MontMul(&X->z, &X->z, &tmp);                         // z^2
        SM9FP2_MontMul(&tmp, &tmp, &right);                         // z^4
        SM9FP2_MontMul(&tmp, &right, &right);                       // z^6
        SM9FP2_MontMul(CONSTS_FP2_MONT_BETA_B, &right, &right);     // bz^6
        SM9FP2_MontMul(&X->x, &X->x, &tmp);                         // x^2
        SM9FP2_MontMul(&tmp, &X->x, &tmp);                          // x^3
        SM9FP2_Add(&tmp, &right, &right);                           // x^3 + bz^6
    }

    return UInt256_Cmp(left.fp1 + 1, right.fp1 + 1) == 0 && UInt256_Cmp(left.fp1, right.fp1) == 0;
}

int SM9JacobPoint2Mont_IsEqual(const SM9JacobPoint2Mont* X, const SM9JacobPoint2Mont* Y)
{
    SM9FP2Mont t1 = { 0 };
    SM9FP2Mont t2 = { 0 };
    SM9FP2Mont t4 = { 0 };
    SM9FP2Mont t5 = { 0 };

    if (UInt256_IsZero(X->z.fp1 + 1) && UInt256_Cmp(X->z.fp1, CONSTS_FP1_MONT_ONE) == 0 &&
        UInt256_IsZero(Y->z.fp1 + 1) && UInt256_Cmp(Y->z.fp1, CONSTS_FP1_MONT_ONE) == 0)
        return UInt256_Cmp(X->x.fp1 + 1, Y->x.fp1 + 1) == 0 && UInt256_Cmp(X->x.fp1, Y->x.fp1) == 0;

    // t1 = x1 * z2^2
    // t2 = x2 * z1^2
    // t4 = y1 * z2^3
    // t5 = y2 * z1^3
    SM9FP2_MontMul(&Y->z, &Y->z, &t4);         // z2^2
    SM9FP2_MontMul(&X->z, &X->z, &t5);         // z1^2

    SM9FP2_MontMul(&X->x, &t4, &t1);           // x1 * z2^2
    SM9FP2_MontMul(&Y->x, &t5, &t2);           // x2 * z1^2

    if (UInt256_Cmp(t1.fp1 + 1, t2.fp1 + 1) != 0 || UInt256_Cmp(t1.fp1, t2.fp1) != 0)
        return 0;

    SM9FP2_MontMul(&X->y, &t4, &t4);           // y1 * z2^2
    SM9FP2_MontMul(&t4, &Y->z, &t4);           // y1 * z2^3

    SM9FP2_MontMul(&Y->y, &t5, &t5);           // y2 * z1^2
    SM9FP2_MontMul(&t5, &X->z, &t5);           // y2 * z1^3

    return UInt256_Cmp(t4.fp1 + 1, t5.fp1 + 1) == 0 && UInt256_Cmp(t4.fp1, t5.fp1) == 0;
}

void SM9JacobPoint2Mont_ToPoint(const SM9JacobPoint2Mont* X, SM9Point2* Y)
{
    SM9FP2Mont z_inv = { 0 };
    if (SM9JacobPoint2Mont_IsInf(X))
    {
        Y->is_inf = 1;
    }
    else
    {
        Y->is_inf = 0;
        if (UInt256_IsZero(X->z.fp1 + 1) && UInt256_Cmp(X->z.fp1, CONSTS_FP1_MONT_ONE) == 0)
        {
            SM9FP2_FromMont(&X->x, &Y->x);
            SM9FP2_FromMont(&X->y, &Y->y);
        }
        else
        {
            SM9FP2_MontInv(&X->z, &z_inv);
            SM9FP2_MontMul(&X->y, &z_inv, &Y->y);
            SM9FP2_MontMul(&z_inv, &z_inv, &z_inv);
            SM9FP2_MontMul(&X->x, &z_inv, &Y->x);
            SM9FP2_MontMul(&Y->y, &z_inv, &Y->y);

            SM9FP2_FromMont(&Y->x, &Y->x);
            SM9FP2_FromMont(&Y->y, &Y->y);
        }
    }
}

void SM9JacobPoint2Mont_FromPoint(const SM9Point2* X, SM9JacobPoint2Mont* Y)
{
    if (X->is_inf)
    {
        SM9JacobPoint2Mont_SetInf(Y);
    }
    else
    {
        SM9FP2_ToMont(&X->x, &Y->x);
        SM9FP2_ToMont(&X->y, &Y->y);
        UInt256_SetZero(Y->z.fp1 + 1); Y->z.fp1[0] = *CONSTS_FP1_MONT_ONE;
    }
}

static
int SM9Point2_IsOnCurve(const SM9Point2* X)
{
    SM9FP2Mont left = { 0 };
    SM9FP2Mont right = { 0 };
    SM9FP2Mont x = { 0 };
    SM9FP2Mont y = { 0 };

    if (X->is_inf)
        return 1;

    SM9FP2_ToMont(&X->x, &x);
    SM9FP2_ToMont(&X->y, &y);

    // left = y^2
    SM9FP2_MontMul(&y, &y, &left);

    // right = x^3 + b
    SM9FP2_MontMul(&x, &x, &right);
    SM9FP2_MontMul(&right, &x, &right);
    SM9FP2_Add(&right, CONSTS_FP2_MONT_BETA_B, &right);

    return UInt256_Cmp(left.fp1 + 1, right.fp1 + 1) == 0 && UInt256_Cmp(left.fp1, right.fp1) == 0;
}

uint64_t SM9Point2_ToBytes(const SM9Point2* X, int pc_mode, uint8_t* bytes)
{
    if (X->is_inf)
    {
        bytes[0] = 0x00;
        return 1;
    }

    if (pc_mode == SM9_PCMODE_COMPRESS)
    {
        if (X->y.fp1->u8[0] & 0x1)
            bytes[0] = 0x03;
        else
            bytes[0] = 0x02;

        UInt256_ToBytes(X->x.fp1 + 1, bytes + 1);
        UInt256_ToBytes(X->x.fp1, bytes + 1 + 32);
        return SM9_POINT2BYTES_HALF_LENGTH;
    }
    else if (pc_mode == SM9_PCMODE_MIX)
    {
        if (X->y.fp1->u8[0] & 0x1)
            bytes[0] = 0x07;
        else
            bytes[0] = 0x06;

        UInt256_ToBytes(X->x.fp1 + 1, bytes + 1);
        UInt256_ToBytes(X->x.fp1, bytes + 1 + 32);
        UInt256_ToBytes(X->y.fp1 + 1, bytes + 1 + 64);
        UInt256_ToBytes(X->y.fp1, bytes + 1 + 96);
        return SM9_POINT2BYTES_FULL_LENGTH;
    }
    else
    {
        bytes[0] = 0x04;

        UInt256_ToBytes(X->x.fp1 + 1, bytes + 1);
        UInt256_ToBytes(X->x.fp1, bytes + 1 + 32);
        UInt256_ToBytes(X->y.fp1 + 1, bytes + 1 + 64);
        UInt256_ToBytes(X->y.fp1, bytes + 1 + 96);
        return SM9_POINT2BYTES_FULL_LENGTH;
    }

    return 0;
}

int SM9Point2_FromBytes(const uint8_t* bytes, uint64_t bytes_len, SM9Point2* X)
{
    uint8_t pc = bytes[0];
    int ylsb = 0;

    if (pc == 0x00)
    {
        if (bytes_len != 1)
            return SM9CURVE_ERR_INVALIDPC;

        X->is_inf = 1;
    }
    else if (pc == 0x04 || pc == 0x06 || pc == 0x07)
    {
        if (bytes_len != SM9_POINT2BYTES_FULL_LENGTH)
            return SM9CURVE_ERR_INVALIDPC;

        UInt256_FromBytes(bytes + 1, X->x.fp1 + 1);
        UInt256_FromBytes(bytes + 1 + 32, X->x.fp1);
        UInt256_FromBytes(bytes + 1 + 64, X->y.fp1 + 1);
        UInt256_FromBytes(bytes + 1 + 96, X->y.fp1);
        X->is_inf = 0;
        if (!SM9Point2_IsOnCurve(X))
            return SM9CURVE_ERR_NOTONCURVE;
    }
    else if (pc == 0x02 || pc == 0x03)
    {
        if (bytes_len != SM9_POINT2BYTES_HALF_LENGTH)
            return SM9CURVE_ERR_INVALIDPC;

        UInt256_FromBytes(bytes + 1, X->x.fp1 + 1);
        UInt256_FromBytes(bytes + 1 + 32, X->x.fp1);
        if (UInt256_Cmp(X->x.fp1 + 1, CONSTS_P) >= 0 || UInt256_Cmp(X->x.fp1, CONSTS_P) >= 0)
            return SM9CURVE_ERR_NOTONCURVE;

        SM9FP2_ToMont(&X->x, &X->x);

        // compute y, x^3 + b
        SM9FP2_MontMul(&X->x, &X->x, &X->y);
        SM9FP2_MontMul(&X->x, &X->y, &X->y);
        SM9FP2_Add(&X->y, CONSTS_FP2_MONT_BETA_B, &X->y);

        if (!SM9FP2_MontHasSqrt(&X->y, &X->y))
            return SM9CURVE_ERR_NOTONCURVE;

        SM9FP2_FromMont(&X->x, &X->x);
        SM9FP2_FromMont(&X->y, &X->y);
        ylsb = X->y.fp1->u8[0] & 0x1;
        if ((pc == 0x02 && ylsb) || (pc == 0x03 && !ylsb))
        {
            SM9FP2_Neg(&X->y, &X->y);
        }
    }
    else
    {
        return SM9CURVE_ERR_INVALIDPC;
    }

    return 0;
}

uint64_t SM9JacobPoint2Mont_ToBytes(const SM9JacobPoint2Mont* X, int pc_mode, uint8_t* bytes)
{
    SM9Point2 pt = { 0 };
    SM9JacobPoint2Mont_ToPoint(X, &pt);
    return SM9Point2_ToBytes(&pt, pc_mode, bytes);
}

int SM9JacobPoint2Mont_FromBytes(const uint8_t* bytes, uint64_t bytes_len, SM9JacobPoint2Mont* X)
{
    SM9Point2 pt = { 0 };
    int ret = SM9Point2_FromBytes(bytes, bytes_len, &pt);
    if (ret != 0)
        return ret;

    SM9JacobPoint2Mont_FromPoint(&pt, X);
    return 0;
}

static
void _SM9JacobPoint2Mont_Dbl(const SM9JacobPoint2Mont* X, SM9JacobPoint2Mont* Y)
{
    // order is IMPORTANT to avoid data overwrite

    SM9FP2Mont t1 = { 0 };
    SM9FP2Mont t2 = { 0 };
    SM9FP2Mont t3 = { 0 };

    // a == 0 (mod p)
    // t1 = 3x^2
    SM9FP2_MontMul(&X->x, &X->x, &t1);          // x^2
    SM9FP2_Add(&t1, &t1, &t2);                  // 2x^2
    SM9FP2_Add(&t1, &t2, &t1);                  // 3x^2

    // z' = 2yz
    SM9FP2_Add(&X->y, &X->y, &t3);              // 2y
    SM9FP2_MontMul(&t3, &X->z, &Y->z);

    // t3 = 8y^4
    SM9FP2_MontMul(&t3, &t3, &t2);              // 4y^2
    SM9FP2_MontMul(&t2, &t2, &t3);              // 16y^4
    SM9FP2_Div2(&t3, &t3);                      // 8y^4

    // t2 = 4xy^2
    SM9FP2_MontMul(&X->x, &t2, &t2);            // 4xy^2

    // x' = t1^2 - 2t2
    SM9FP2_MontMul(&t1, &t1, &Y->x);
    SM9FP2_Sub(&Y->x, &t2, &Y->x);
    SM9FP2_Sub(&Y->x, &t2, &Y->x);

    // y' = t1(t2 - x') - t3
    SM9FP2_Sub(&t2, &Y->x, &Y->y);
    SM9FP2_MontMul(&t1, &Y->y, &Y->y);
    SM9FP2_Sub(&Y->y, &t3, &Y->y);
}

void SM9JacobPoint2Mont_Dbl(const SM9JacobPoint2Mont* X, SM9JacobPoint2Mont* Y)
{
    if (SM9JacobPoint2Mont_IsInf(X))
    {
        *Y = *X;
    }
    else
    {
        _SM9JacobPoint2Mont_Dbl(X, Y);
    }
}

static
void _SM9JacobPoint2Mont_Add(const SM9JacobPoint2Mont* X, const SM9JacobPoint2Mont* Y, SM9JacobPoint2Mont* Z)
{
    SM9FP2Mont t1 = { 0 };
    SM9FP2Mont t2 = { 0 };
    SM9FP2Mont t3 = { 0 };
    SM9FP2Mont t4 = { 0 };
    SM9FP2Mont t5 = { 0 };
    SM9FP2Mont t6 = { 0 };

    // t1 = x1 * z2^2
    // t2 = x2 * z1^2
    // t4 = y1 * z2^3
    // t5 = y2 * z1^3
    SM9FP2_MontMul(&Y->z, &Y->z, &t4);          // z2^2
    SM9FP2_MontMul(&X->z, &X->z, &t5);          // z1^2

    SM9FP2_MontMul(&X->x, &t4, &t1);            // x1 * z2^2
    SM9FP2_MontMul(&Y->x, &t5, &t2);            // x2 * z1^2

    SM9FP2_MontMul(&X->y, &t4, &t4);            // y1 * z2^2
    SM9FP2_MontMul(&t4, &Y->z, &t4);            // y1 * z2^3

    SM9FP2_MontMul(&Y->y, &t5, &t5);            // y2 * z1^2
    SM9FP2_MontMul(&t5, &X->z, &t5);            // y2 * z1^3

    if (UInt256_Cmp(t1.fp1 + 1, t2.fp1 + 1) == 0 && UInt256_Cmp(t1.fp1, t2.fp1) == 0)
    {
        if (UInt256_Cmp(t4.fp1 + 1, t5.fp1 + 1) == 0 && UInt256_Cmp(t4.fp1, t5.fp1) == 0)
            _SM9JacobPoint2Mont_Dbl(X, Z);
        else
            SM9JacobPoint2Mont_SetInf(Z);
        return;
    }

    // t3 = t1 - t2
    // t6 = t4 - t5
    SM9FP2_Sub(&t1, &t2, &t3);
    SM9FP2_Sub(&t4, &t5, &t6);

    // z3 = z1 * z2 * t3
    SM9FP2_MontMul(&X->z, &Y->z, &Z->z);        // z1 * z2
    SM9FP2_MontMul(&Z->z, &t3, &Z->z);          // z1 * z2 * t3

    // x3 = t6^2 - (t1 + t2) * t3^2
    SM9FP2_MontMul(&t6, &t6, &Z->x);            // t6^2
    SM9FP2_Add(&t1, &t2, &t2);                  // t1 + t2
    SM9FP2_MontMul(&t3, &t3, &t5);              // t3^2
    SM9FP2_MontMul(&t2, &t5, &t2);              // (t1 + t2) * t3^2
    SM9FP2_Sub(&Z->x, &t2, &Z->x);              // t6^2 - (t1 + t2) * t3^2

    // y3 = t6 * (t1 * t3^2 - x3) - t4 * t3^3
    SM9FP2_MontMul(&t1, &t5, &Z->y);            // t1 * t3^2
    SM9FP2_Sub(&Z->y, &Z->x, &Z->y);            // t1 * t3^2 - x3
    SM9FP2_MontMul(&t6, &Z->y, &Z->y);          // t6 * (t1 * t3^2 - x3)
    SM9FP2_MontMul(&t4, &t3, &t3);              // t4 * t3
    SM9FP2_MontMul(&t3, &t5, &t3);              // t4 * t3^3
    SM9FP2_Sub(&Z->y, &t3, &Z->y);              // t6 * (t1 * t3^2 - x3) - t4 * t3^3
}

void SM9JacobPoint2Mont_Add(const SM9JacobPoint2Mont* X, const SM9JacobPoint2Mont* Y, SM9JacobPoint2Mont* Z)
{
    if (SM9JacobPoint2Mont_IsInf(X))
    {
        *Z = *Y;
    }
    else if (SM9JacobPoint2Mont_IsInf(Y))
    {
        *Z = *X;
    }
    else
    {
        _SM9JacobPoint2Mont_Add(X, Y, Z);
    }
}

static
void _SM9JacobPoint2Mont_Mul_SlidingWindow(const UInt256* k, const SM9JacobPoint2Mont* X, SM9JacobPoint2Mont* Y)
{
    int32_t i = 0;
    int32_t j = 0;
    int32_t w = 0;
    uint32_t wvalue = 0;
    SM9JacobPoint2Mont Y_tmp = { 0 };

    // pre-compute, save odd points, 1, 3, 5, ..., 2^w - 1
    SM9JacobPoint2Mont table[SCALARMUL_TSIZE] = { *X };
    SM9JacobPoint2Mont_Dbl(X, &Y_tmp);
    for (i = 0; i < SCALARMUL_TSIZE - 1; i++)
    {
        SM9JacobPoint2Mont_Add(table + i, &Y_tmp, table + i + 1);
    }

    SM9JacobPoint2Mont_SetInf(&Y_tmp);
    i = 255;
    while (i >= 0)
    {
        wvalue = (k->u64[i / 64] >> (i % 64)) & 0x1;

        if (wvalue == 0)
        {
            SM9JacobPoint2Mont_Dbl(&Y_tmp, &Y_tmp);
            i--;
        }
        else
        {
            // find a longest 1...1 bits in window size
            j = i;
            for (w = i - 1; w >= 0 && w > i - SCALARMUL_WSIZE; w--)
            {
                if ((k->u64[w / 64] >> (w % 64)) & 0x1)
                {
                    wvalue = (wvalue << (j - w)) | 0x1;
                    j = w;
                }
            }

            while (i >= j)
            {
                SM9JacobPoint2Mont_Dbl(&Y_tmp, &Y_tmp);
                i--;
            }

            SM9JacobPoint2Mont_Add(&Y_tmp, table + (wvalue >> 1), &Y_tmp);
        }
    }

    *Y = Y_tmp;
}

void SM9JacobPoint2Mont_Mul(const UInt256* k, const SM9JacobPoint2Mont* X, SM9JacobPoint2Mont* Y)
{
    _SM9JacobPoint2Mont_Mul_SlidingWindow(k, X, Y);
}

void SM9JacobPoint2Mont_Neg(const SM9JacobPoint2Mont* X, SM9JacobPoint2Mont* Y)
{
    Y->x = X->x;
    SM9FP2_Neg(&X->y, &Y->y);
    Y->z = X->z;
}

void SM9JacobPoint2Mont_Sub(const SM9JacobPoint2Mont* X, const SM9JacobPoint2Mont* Y, SM9JacobPoint2Mont* Z)
{
    SM9JacobPoint2Mont neg_Y = { 0 };
    SM9JacobPoint2Mont_Neg(Y, &neg_Y);
    SM9JacobPoint2Mont_Add(X, &neg_Y, Z);
}

void SM9JacobPoint2Mont_MulG2(const UInt256* k, SM9JacobPoint2Mont* X)
{
    uint32_t i;
    uint32_t part_k;
    SM9JacobPoint2Mont_SetInf(X);

    for (i = 0; i < 32; i++)
    {
        part_k = k->u8[i];
        if (part_k > 0)
        {
            SM9JacobPoint2Mont_Add(X, SM9_MULG2_TABLE_U8 + i * 256 + part_k, X);
        }
    }
}

#ifdef _DEBUG

void SM9FP2Mont_Print(const SM9FP2Mont* x)
{
    SM9FP1 xx = { 0 };
    SM9FP1_FromMont(x->fp1 + 1, &xx);
    UInt256_Print(&xx, 4);
    printf(",\n");
    SM9FP1_FromMont(x->fp1, &xx);
    UInt256_Print(&xx, 4);
}

void SM9Point2_Print(const SM9Point2* X)
{
    printf("{ ");
    if (X->is_inf)
    {
        printf("<INF>");
    }
    else
    {
        printf("x: { ");
        UInt256_Print(X->x.fp1 + 1, 4);
        printf(",\n");
        printf("       ");
        UInt256_Print(X->x.fp1, 4);
        printf(" },\n");

        printf("  y: { ");
        UInt256_Print(X->y.fp1 + 1, 4);
        printf(",\n");
        printf("       ");
        UInt256_Print(X->y.fp1, 4);
        printf(" }");
    }
    printf(" }\n");
}

void SM9JacobPoint2Mont_Print(const SM9JacobPoint2Mont* X)
{
    SM9Point2 t = { 0 };
    printf("{ ");

    printf("x: { ");
    UInt256_Print(X->x.fp1 + 1, 4);
    printf(",\n");
    printf("       ");
    UInt256_Print(X->x.fp1, 4);
    printf(" },\n");

    printf("  y: { ");
    UInt256_Print(X->y.fp1 + 1, 4);
    printf(",\n");
    printf("       ");
    UInt256_Print(X->y.fp1, 4);
    printf(" },\n");

    printf("  z: { ");
    UInt256_Print(X->z.fp1 + 1, 4);
    printf(",\n");
    printf("       ");
    UInt256_Print(X->z.fp1, 4);
    printf(" }}\n");

    SM9JacobPoint2Mont_ToPoint(X, &t);
    SM9Point2_Print(&t);
}

#endif // _DEBUG

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FP2 Points <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> FP4 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

static
void SM9FP4_Add(const SM9FP4* x, const SM9FP4* y, SM9FP4* z)
{
    SM9FP1_Add(x->fp1 + 3, y->fp1 + 3, z->fp1 + 3);
    SM9FP1_Add(x->fp1 + 2, y->fp1 + 2, z->fp1 + 2);
    SM9FP1_Add(x->fp1 + 1, y->fp1 + 1, z->fp1 + 1);
    SM9FP1_Add(x->fp1, y->fp1, z->fp1);
}

static
void SM9FP4_Sub(const SM9FP4* x, const SM9FP4* y, SM9FP4* z)
{
    SM9FP1_Sub(x->fp1 + 3, y->fp1 + 3, z->fp1 + 3);
    SM9FP1_Sub(x->fp1 + 2, y->fp1 + 2, z->fp1 + 2);
    SM9FP1_Sub(x->fp1 + 1, y->fp1 + 1, z->fp1 + 1);
    SM9FP1_Sub(x->fp1, y->fp1, z->fp1);
}

static
void SM9FP4_MontMul(const SM9FP4Mont* x, const SM9FP4Mont* y, SM9FP4Mont* z)
{
    SM9FP4Mont z_tmp = { 0 };
    const SM9FP2Mont* x1 = x->fp2 + 1;
    const SM9FP2Mont* x0 = x->fp2;
    const SM9FP2Mont* y1 = y->fp2 + 1;
    const SM9FP2Mont* y0 = y->fp2;
    SM9FP2Mont* z1 = z_tmp.fp2 + 1;
    SM9FP2Mont* z0 = z_tmp.fp2;

    SM9FP2Mont _x1y1 = { 0 };
    SM9FP2Mont _x0y0 = { 0 };
    const SM9FP2Mont* x1y1 = &_x1y1;
    const SM9FP2Mont* x0y0 = &_x0y0;

    SM9FP2_MontMul(x1, y1, &_x1y1);
    SM9FP2_MontMul(x0, y0, &_x0y0);

    // z1 = (x1 + x0)(y1 + y0) - x1y1 - x0y0
    SM9FP2_Add(x1, x0, z1);
    SM9FP2_Add(y1, y0, z0);
    SM9FP2_MontMul(z1, z0, z1);
    SM9FP2_Sub(z1, x1y1, z1);
    SM9FP2_Sub(z1, x0y0, z1);

    // z0 = x0y0 + U(x1y1)
    _SM9FP2_MontMulU(x1y1, z0);
    SM9FP2_Add(x0y0, z0, z0);

    *z = z_tmp;
}

static inline
void _SM9FP4_MontMulU(const SM9FP4Mont* x, SM9FP4Mont* y)
{
    // U = ((0, 1), (0, 0))
    // (1, 0) * (b1, b0) = (b0, (1, 0) * b1)
    y->fp2[1] = x->fp2[0];
    _SM9FP2_MontMulU(x->fp2 + 1, y->fp2);
}

static
void SM9FP4_MontInv(const SM9FP4Mont* x, SM9FP4Mont* y)
{
    SM9FP4Mont y_tmp = { 0 };
    const SM9FP2Mont* x1 = x->fp2 + 1;
    const SM9FP2Mont* x0 = x->fp2;
    SM9FP2Mont* y1 = y_tmp.fp2 + 1;
    SM9FP2Mont* y0 = y_tmp.fp2;

    // det = x0^2 - U(x1^2)
    SM9FP2_MontMul(x1, x1, y0);
    _SM9FP2_MontMulU(y0, y1);
    SM9FP2_MontMul(x0, x0, y0);
    SM9FP2_Sub(y0, y1, y0);

    // 1 / det
    SM9FP2_MontInv(y0, y0);

    // y1 =  -x1 / det
    SM9FP2_MontMul(y0, x1, y1);
    SM9FP2_Neg(y1, y1);

    // y0 = x0 / det
    SM9FP2_MontMul(y0, x0, y0);

    *y = y_tmp;
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FP4 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> FP12 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void SM9FP12_MontMul(const SM9FP12Mont* x, const SM9FP12Mont* y, SM9FP12Mont* z)
{
    SM9FP12Mont z_tmp = { 0 };
    SM9FP4Mont m2 = { 0 }, m1 = { 0 }, m0 = { 0 };
    SM9FP4Mont tmp = { 0 };
    const SM9FP4Mont* x2 = x->fp4 + 2;
    const SM9FP4Mont* x1 = x->fp4 + 1;
    const SM9FP4Mont* x0 = x->fp4;
    const SM9FP4Mont* y2 = y->fp4 + 2;
    const SM9FP4Mont* y1 = y->fp4 + 1;
    const SM9FP4Mont* y0 = y->fp4;
    SM9FP4Mont* z2 = z_tmp.fp4 + 2;
    SM9FP4Mont* z1 = z_tmp.fp4 + 1;
    SM9FP4Mont* z0 = z_tmp.fp4;

    SM9FP4_MontMul(x2, y2, &m2);        // x2y2
    SM9FP4_MontMul(x1, y1, &m1);        // x1y1
    SM9FP4_MontMul(x0, y0, &m0);        // x0y0

    // z2 =   x2y0  +   x1y1  + x0y2 =   (x2 + x0)(y2 + y0) - x2y2 - x0y0  +   x1y1
    // z1 = U(x2y2) +   x1y0  + x0y1 =   (x1 + x0)(y1 + y0) - x1y1 - x0y0  + U(x2y2)
    // z0 = U(x2y1) + U(x1y2) + x0y0 = U((x2 + x1)(y2 + y1) - x2y2 - x1y1) +   x0y0

    // z2 = (x2 + x0)(y2 + y0) - x2y2 - x0y0 + x1y1
    SM9FP4_Add(x2, x0, &tmp);
    SM9FP4_Add(y2, y0, z2);
    SM9FP4_MontMul(&tmp, z2, z2);
    SM9FP4_Sub(z2, &m2, z2);
    SM9FP4_Sub(z2, &m0, z2);
    SM9FP4_Add(z2, &m1, z2);

    // z1 = (x1 + x0)(y1 + y0) - x1y1 - x0y0 + U(x2y2)
    SM9FP4_Add(x1, x0, &tmp);
    SM9FP4_Add(y1, y0, z1);
    SM9FP4_MontMul(&tmp, z1, z1);
    SM9FP4_Sub(z1, &m1, z1);
    SM9FP4_Sub(z1, &m0, z1);
    _SM9FP4_MontMulU(&m2, &tmp);
    SM9FP4_Add(z1, &tmp, z1);

    // z0 = U((x2 + x1)(y2 + y1) - x2y2 - x1y1) + x0y0
    SM9FP4_Add(x2, x1, &tmp);
    SM9FP4_Add(y2, y1, z0);
    SM9FP4_MontMul(&tmp, z0, z0);
    SM9FP4_Sub(z0, &m2, z0);
    SM9FP4_Sub(z0, &m1, z0);
    _SM9FP4_MontMulU(z0, &tmp);
    SM9FP4_Add(&tmp, &m0, z0);

    *z = z_tmp;
}

static
void SM9FP12_ToMont(const SM9FP12* x, SM9FP12Mont* y)
{
    for (int i = 0; i < 12; i++)
        SM9FP1_ToMont(x->fp1 + i, y->fp1 + i);
}


void SM9FP12_FromMont(const SM9FP12Mont* x, SM9FP12* y)
{
    for (int i = 0; i < 12; i++)
        SM9FP1_FromMont(x->fp1 + i, y->fp1 + i);
}

static
void SM9FP12_MontInv(const SM9FP12Mont* x, SM9FP12Mont* y)
{
    SM9FP12Mont y_tmp = { 0 };
    SM9FP4Mont m2 = { 0 }, m1 = { 0 }, m0 = { 0 };
    SM9FP4Mont tmp = { 0 };
    const SM9FP4Mont* x2 = x->fp4 + 2;
    const SM9FP4Mont* x1 = x->fp4 + 1;
    const SM9FP4Mont* x0 = x->fp4;
    SM9FP4Mont* y2 = y_tmp.fp4 + 2;
    SM9FP4Mont* y1 = y_tmp.fp4 + 1;
    SM9FP4Mont* y0 = y_tmp.fp4;
    SM9FP4Mont det = { 0 };

    // m2 = U(x2^2) -   x1x0
    SM9FP4_MontMul(x2, x2, &tmp);
    _SM9FP4_MontMulU(&tmp, &m2);
    SM9FP4_MontMul(x1, x0, &tmp);
    SM9FP4_Sub(&m2, &tmp, &m2);

    // m1 =   x1^2  -   x2x0
    SM9FP4_MontMul(x1, x1, &m1);
    SM9FP4_MontMul(x2, x0, &tmp);
    SM9FP4_Sub(&m1, &tmp, &m1);

    // m0 =   x0^2  - U(x2x1)
    SM9FP4_MontMul(x2, x1, &m0);
    _SM9FP4_MontMulU(&m0, &tmp);
    SM9FP4_MontMul(x0, x0, &m0);
    SM9FP4_Sub(&m0, &tmp, &m0);

    // det = U(x2)m2 + U(x1)m1 + x0m0
    _SM9FP4_MontMulU(x2, &tmp);
    SM9FP4_MontMul(&tmp, &m2, &det);
    _SM9FP4_MontMulU(x1, &tmp);
    SM9FP4_MontMul(&tmp, &m1, &tmp);
    SM9FP4_Add(&det, &tmp, &det);
    SM9FP4_MontMul(x0, &m0, &tmp);
    SM9FP4_Add(&det, &tmp, &det);

    // y2 = m1 / det
    // y1 = m2 / det
    // y0 = m0 / det
    SM9FP4_MontInv(&det, &det);
    SM9FP4_MontMul(&m1, &det, y2);
    SM9FP4_MontMul(&m2, &det, y1);
    SM9FP4_MontMul(&m0, &det, y0);

    *y = y_tmp;
}

void SM9FP12_MontPow(const SM9FP12Mont* x, const UInt256* e, SM9FP12Mont* y)
{
    int32_t i = 0;
    int32_t j = 0;
    int32_t w = 0;
    uint32_t wvalue = 0;
    SM9FP12Mont y_tmp = { 0 };

    // pre-compute, save odd points, 1, 3, 5, ..., 2^w - 1
    SM9FP12Mont table[SCALARMUL_TSIZE] = { *x };
    SM9FP12_MontMul(x, x, &y_tmp);
    for (i = 0; i < SCALARMUL_TSIZE - 1; i++)
    {
        SM9FP12_MontMul(table + i, &y_tmp, table + i + 1);
    }

    // set to 1 in fp12
    for (i = 1; i < 12; i++) UInt256_SetZero(y_tmp.fp1 + i); y_tmp.fp1[0] = *CONSTS_FP1_MONT_ONE;

    i = 255;
    while (i >= 0)
    {
        wvalue = (e->u64[i / 64] >> (i % 64)) & 0x1;

        if (wvalue == 0)
        {
            SM9FP12_MontMul(&y_tmp, &y_tmp, &y_tmp);
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
                SM9FP12_MontMul(&y_tmp, &y_tmp, &y_tmp);
                i--;
            }

            SM9FP12_MontMul(&y_tmp, table + (wvalue >> 1), &y_tmp);
        }
    }

    *y = y_tmp;
}

static
void SM9FP12_MontPowU64(const SM9FP12Mont* x, uint64_t e, SM9FP12Mont* y)
{
    int32_t i = 0;
    SM9FP12Mont _y_tmp = { 0 }, * y_tmp = &_y_tmp;
    y_tmp->fp1[0] = *CONSTS_FP1_MONT_ONE;

    // find first 1 bit
    for (i = 0; i < 64; i++)
    {
        if (e & 0x8000000000000000)
        {
            *y_tmp = *x;
            e <<= 1;
            i++;
            break;
        }
        e <<= 1;
    }

    for (; i < 64; i++)
    {
        SM9FP12_MontMul(y_tmp, y_tmp, y_tmp);
        if (e & 0x8000000000000000)
        {
            SM9FP12_MontMul(y_tmp, x, y_tmp);
        }
        e <<= 1;
    }

    *y = *y_tmp;
}

static
void SM9FP12_MontFrob1(const SM9FP12Mont* x, SM9FP12Mont* y)
{
    // (((p - w5, w5), (p - w2, w2)), ((p - w4, w4), (p - w1, w1)), ((p - w3, w3), (p - w0, w0)))
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F5, x->fp1 + 11, y->fp1 + 11); SM9FP1_Neg(y->fp1 + 11, y->fp1 + 11);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F5, x->fp1 + 10, y->fp1 + 10);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F2, x->fp1 + 9, y->fp1 + 9); SM9FP1_Neg(y->fp1 + 9, y->fp1 + 9);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F2, x->fp1 + 8, y->fp1 + 8);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F4, x->fp1 + 7, y->fp1 + 7); SM9FP1_Neg(y->fp1 + 7, y->fp1 + 7);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F4, x->fp1 + 6, y->fp1 + 6);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F1, x->fp1 + 5, y->fp1 + 5); SM9FP1_Neg(y->fp1 + 5, y->fp1 + 5);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F1, x->fp1 + 4, y->fp1 + 4);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F3, x->fp1 + 3, y->fp1 + 3); SM9FP1_Neg(y->fp1 + 3, y->fp1 + 3);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F3, x->fp1 + 2, y->fp1 + 2);
    SM9FP1_Neg(x->fp1 + 1, y->fp1 + 1);
    y->fp1[0] = x->fp1[0];
}

static
void SM9FP12_MontFrob2(const SM9FP12Mont* x, SM9FP12Mont* y)
{
    // (((p - w4, p - w4), (w4, w4)), ((p - w2, p - w2), (w2, w2)), ((p - w0, p - w0), (w0, w0)))
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F4, x->fp1 + 11, y->fp1 + 11); SM9FP1_Neg(y->fp1 + 11, y->fp1 + 11);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F4, x->fp1 + 10, y->fp1 + 10); SM9FP1_Neg(y->fp1 + 10, y->fp1 + 10);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F4, x->fp1 + 9, y->fp1 + 9);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F4, x->fp1 + 8, y->fp1 + 8);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F2, x->fp1 + 7, y->fp1 + 7); SM9FP1_Neg(y->fp1 + 7, y->fp1 + 7);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F2, x->fp1 + 6, y->fp1 + 6); SM9FP1_Neg(y->fp1 + 6, y->fp1 + 6);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F2, x->fp1 + 5, y->fp1 + 5);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F2, x->fp1 + 4, y->fp1 + 4);
    SM9FP1_Neg(x->fp1 + 3, y->fp1 + 3);
    SM9FP1_Neg(x->fp1 + 2, y->fp1 + 2);
    y->fp1[1] = x->fp1[1];
    y->fp1[0] = x->fp1[0];
}

static
void SM9FP12_MontFrob3(const SM9FP12Mont* x, SM9FP12Mont* y)
{
    // (((p - w3, w3), (w0, p - w0)), ((p - w0, w0), (p - w3, w3)), ((w3, p - w3), (p - w0, w0)))
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F3, x->fp1 + 11, y->fp1 + 11); SM9FP1_Neg(y->fp1 + 11, y->fp1 + 11);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F3, x->fp1 + 10, y->fp1 + 10);
    y->fp1[9] = x->fp1[9];
    SM9FP1_Neg(x->fp1 + 8, y->fp1 + 8);
    SM9FP1_Neg(x->fp1 + 7, y->fp1 + 7);
    y->fp1[6] = x->fp1[6];
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F3, x->fp1 + 5, y->fp1 + 5); SM9FP1_Neg(y->fp1 + 5, y->fp1 + 5);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F3, x->fp1 + 4, y->fp1 + 4);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F3, x->fp1 + 3, y->fp1 + 3);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F3, x->fp1 + 2, y->fp1 + 2); SM9FP1_Neg(y->fp1 + 2, y->fp1 + 2);
    SM9FP1_Neg(x->fp1 + 1, y->fp1 + 1);
    y->fp1[0] = x->fp1[0];
}

static
void SM9FP12_MontFrob6(const SM9FP12Mont* x, SM9FP12Mont* y)
{
    // (((p - w0, p - w0), (w0, w0)), ((w0, w0), (p - w0, p - w0)), ((p - w0, p - w0), (w0, w0)))
    SM9FP1_Neg(x->fp1 + 11, y->fp1 + 11);
    SM9FP1_Neg(x->fp1 + 10, y->fp1 + 10);
    y->fp1[9] = x->fp1[9];
    y->fp1[8] = x->fp1[8];
    y->fp1[7] = x->fp1[7];
    y->fp1[6] = x->fp1[6];
    SM9FP1_Neg(x->fp1 + 5, y->fp1 + 5);
    SM9FP1_Neg(x->fp1 + 4, y->fp1 + 4);
    SM9FP1_Neg(x->fp1 + 3, y->fp1 + 3);
    SM9FP1_Neg(x->fp1 + 2, y->fp1 + 2);
    y->fp1[1] = x->fp1[1];
    y->fp1[0] = x->fp1[0];
}

#ifdef _DEBUG

void SM9FP12Mont_Print(const SM9FP12Mont* x)
{
    SM9FP1 xx = { 0 };
    for (int i = 11; i > 0; i--)
    {
        SM9FP1_FromMont(x->fp1 + i, &xx);
        UInt256_Print(&xx, 4);
        printf(",\n");
    }
    SM9FP1_FromMont(x->fp1, &xx);
    UInt256_Print(&xx, 4);
}

#endif // _DEBUG

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FP12 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> R-ate Pairing >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

static
void _SM9Pairing_LinearDbl(const SM9JacobPoint2Mont* V, const SM9JacobPoint1Mont* Q, SM9FP12Mont* g_num, SM9FP12Mont* g_den)
{
    // NOTE: See https://ww-rm.github.io/posts/2024/06/16/sm9gfn/ for details

    // set fp12 zero
    for (int i = 0; i < 12; i++) UInt256_SetZero(g_num->fp1 + i);
    for (int i = 0; i < 12; i++) UInt256_SetZero(g_den->fp1 + i);

    // (((0, 6), (3, 9)), ((1, 7), (4, 10)), ((2, 8), (5, 11)))
    SM9FP2Mont* g_num_0 = g_num->fp2;       // (0, 6)
    SM9FP2Mont* g_num_2 = g_num->fp2 + 4;   // (2, 8)
    SM9FP2Mont* g_num_3 = g_num->fp2 + 1;   // (3, 9)
    SM9FP2Mont* g_den_3 = g_den->fp2 + 1;   // (3, 9)

    SM9FP1Mont _tmp_fp1 = { 0 }, * tmp_fp1 = &_tmp_fp1;
    SM9FP2Mont _tmp = { 0 }, * tmp = &_tmp;

    // t1 = yV

    // t2 = (3/2)xV^2
    SM9FP2_MontMul(&V->x, &V->x, g_num_2);          // xV^2
    SM9FP2_Add(g_num_2, g_num_2, tmp);
    SM9FP2_Add(tmp, g_num_2, g_num_2);
    SM9FP2_Div2(g_num_2, g_num_2);                  // (3/2)xV^2

    // g_num_0
    SM9FP2_MontMul(&V->y, &V->y, g_num_0);          // yV^2
    SM9FP2_MontMul(g_num_2, &V->x, tmp);            // (3/2)xV^3
    SM9FP2_Sub(g_num_0, tmp, g_num_0);              // yV^2 - (3/2)xV^3
    SM9FP1_MontMul(&Q->z, &Q->z, tmp_fp1);          // zQ^2
    SM9FP1_MontMul(tmp_fp1, &Q->z, tmp_fp1);        // zQ^3
    SM9FP2_MontMulFP1(g_num_0, tmp_fp1, g_num_0);   // zQ^3( yV^2 - (3/2)xV^3 )

    // g_num_2
    SM9FP2_MontMul(&V->z, &V->z, g_num_3);          // zV^2
    SM9FP2_MontMul(g_num_2, g_num_3, g_num_2);      // (3/2)xV^2 * zV^2
    SM9FP2_MontMulFP1(g_num_2, &Q->x, g_num_2);
    SM9FP2_MontMulFP1(g_num_2, &Q->z, g_num_2);     // (3/2)xQ * zQ * xV^2 * zV^2

    // g_num_3
    SM9FP2_MontMul(g_num_3, &V->z, g_num_3);        // zV^3
    SM9FP2_MontMul(&V->y, g_num_3, g_den_3);        // yV * zV^3
    SM9FP2_MontMulFP1(g_den_3, &Q->y, g_num_3);     // yQ * yV * zV^3
    SM9FP2_Neg(g_num_3, g_num_3);                   // - yQ * yV * zV^3

    // g_den_3
    SM9FP2_MontMulFP1(g_den_3, tmp_fp1, g_den_3);   // zQ^3 * yV * zV^3
}

static
void SM9Pairing_LinearDbl(const SM9JacobPoint2Mont* V, const SM9JacobPoint1Mont* Q, SM9FP12Mont* g_num, SM9FP12Mont* g_den)
{
    // just return 1 in fp12
    if (SM9JacobPoint2Mont_IsInf(V) || SM9JacobPoint1Mont_IsInf(Q))
    {
        for (int i = 1; i < 12; i++) UInt256_SetZero(g_num->fp1 + i);
        g_num->fp1[0] = *CONSTS_FP1_MONT_ONE;

        for (int i = 1; i < 12; i++) UInt256_SetZero(g_den->fp1 + i);
        g_den->fp1[0] = *CONSTS_FP1_MONT_ONE;
    }
    else
    {
        _SM9Pairing_LinearDbl(V, Q, g_num, g_den);
    }
}

static
void _SM9Pairing_LinearAdd(const SM9JacobPoint2Mont* U, const SM9JacobPoint2Mont* V, const SM9JacobPoint1Mont* Q, SM9FP12Mont* g_num, SM9FP12Mont* g_den)
{
    // NOTE: See https://ww-rm.github.io/posts/2024/06/16/sm9gfn/ for details

    // set fp12 zero
    for (int i = 0; i < 12; i++) UInt256_SetZero(g_num->fp1 + i);
    for (int i = 0; i < 12; i++) UInt256_SetZero(g_den->fp1 + i);

    // (((0, 6), (3, 9)), ((1, 7), (4, 10)), ((2, 8), (5, 11)))
    SM9FP2Mont* g_num_0 = g_num->fp2;       // (0, 6)
    SM9FP2Mont* g_num_2 = g_num->fp2 + 4;   // (2, 8)
    SM9FP2Mont* g_num_3 = g_num->fp2 + 1;   // (3, 9)
    SM9FP2Mont* g_den_2 = g_den->fp2 + 4;   // (2, 8)
    SM9FP2Mont* g_den_3 = g_den->fp2 + 1;   // (3, 9)

    SM9FP2Mont _t1 = { 0 }, * t1 = &_t1;
    SM9FP2Mont _t2 = { 0 }, * t2 = &_t2;
    SM9FP1Mont _tmp_fp1 = { 0 }, * tmp_fp1 = &_tmp_fp1;
    SM9FP2Mont _tmp = { 0 }, * tmp = &_tmp;

    // t1-pre
    SM9FP2_MontMul(&V->z, &V->z, g_num_2);          // zV^2
    SM9FP2_MontMul(g_num_2, &U->x, t1);             // xU * zV^2
    SM9FP2_MontMul(&U->z, &U->z, t2);               // zU^2
    SM9FP2_MontMul(t2, &V->x, tmp);                 // xV * zU^2
    SM9FP2_Sub(t1, tmp, t1);                        // xU * zV^2 - xV * zU^2

    // t2-pre
    SM9FP2_MontMul(g_num_2, &V->z, g_num_3);        // zV^3
    SM9FP2_MontMul(g_num_3, &U->y, tmp);            // yU * zV^3
    SM9FP2_MontMul(t2, &U->z, t2);                  // zU^3
    SM9FP2_MontMul(t2, &V->y, t2);                  // yV * zU^3
    SM9FP2_Sub(tmp, t2, t2);                        // yU * zV^3 - yV * zU^3

    if (UInt256_IsZero(t1->fp1) && UInt256_IsZero(t1->fp1 + 1))
    {
        // U, V are same points
        if (UInt256_IsZero(t2->fp1) && UInt256_IsZero(t2->fp1 + 1))
        {
            // t1
            *t1 = V->y;                             // yV

            // t2
            SM9FP2_MontMul(&V->x, &V->x, t2);       // xV^2
            SM9FP2_Add(t2, t2, tmp);
            SM9FP2_Add(tmp, t2, t2);
            SM9FP2_Div2(t2, t2);                    // (3/2)xV^2
        }
        // U, V are opposite points
        else
        {
            // set g_num_3 to zero
            UInt256_SetZero(g_num_3->fp1 + 1); UInt256_SetZero(g_num_3->fp1);

            // g_num_0
            SM9FP1_MontMul(&Q->z, &Q->z, tmp_fp1);          // zQ^2
            SM9FP2_MontMulFP1(&V->x, tmp_fp1, g_num_0);     // zQ^2 * xV
            SM9FP2_Neg(g_num_0, g_num_0);                   // - zQ^2 * xV

            // g_den_2
            SM9FP2_MontMulFP1(g_num_2, tmp_fp1, g_den_2);   // zQ^2 * zV^2

            // g_num_2
            SM9FP2_MontMulFP1(g_num_2, &Q->x, g_num_2);     // xQ * zV^2

            return;
        }
    }
    // U, V are different points
    else
    {
        // t1
        SM9FP2_MontMul(t1, &U->z, t1);
        SM9FP2_MontMul(t1, &V->z, t1);              // zU * zV * (xU * zV^2 - xV * zU^2)

        // t2
        SM9FP2_MontMul(t2, &V->z, t2);              // zV( yU * zV^3 - yV * zU^3 )
    }

    // g_num_0
    SM9FP2_MontMul(t1, &V->y, g_num_0);             // t1 * yV
    SM9FP2_MontMul(t2, &V->x, tmp);                 // t2 * xV
    SM9FP2_Sub(g_num_0, tmp, g_num_0);              // t1 * yV - t2 * xV
    SM9FP1_MontMul(&Q->z, &Q->z, tmp_fp1);          // zQ^2
    SM9FP1_MontMul(tmp_fp1, &Q->z, tmp_fp1);        // zQ^3
    SM9FP2_MontMulFP1(g_num_0, tmp_fp1, g_num_0);   // zQ^3( t1 * yV - t2 * xV )

    // g_num_2
    SM9FP2_MontMul(g_num_2, t2, g_num_2);           // t2 * zV^2
    SM9FP2_MontMulFP1(g_num_2, &Q->z, g_num_2);
    SM9FP2_MontMulFP1(g_num_2, &Q->x, g_num_2);     // xQ * zQ * t2 * zV^2

    // g_num_3
    SM9FP2_MontMul(t1, g_num_3, g_den_3);           // t1 * zV^3
    SM9FP2_MontMulFP1(g_den_3, &Q->y, g_num_3);     // yQ * t1 * zV^3
    SM9FP2_Neg(g_num_3, g_num_3);                   // - yQ * t1 * zV^3

    // g_den_3
    SM9FP2_MontMulFP1(g_den_3, tmp_fp1, g_den_3);   // zQ ^ 3 * t1 * zV ^ 3
}

static
void SM9Pairing_LinearAdd(const SM9JacobPoint2Mont* U, const SM9JacobPoint2Mont* V, const SM9JacobPoint1Mont* Q, SM9FP12Mont* g_num, SM9FP12Mont* g_den)
{
    // just return 1 in fp12
    if (SM9JacobPoint2Mont_IsInf(U) || SM9JacobPoint2Mont_IsInf(V) || SM9JacobPoint1Mont_IsInf(Q))
    {
        for (int i = 1; i < 12; i++) UInt256_SetZero(g_num->fp1 + i);
        g_num->fp1[0] = *CONSTS_FP1_MONT_ONE;

        for (int i = 1; i < 12; i++) UInt256_SetZero(g_den->fp1 + i);
        g_den->fp1[0] = *CONSTS_FP1_MONT_ONE;
    }
    else
    {
        _SM9Pairing_LinearAdd(U, V, Q, g_num, g_den);
    }
}

//static
//void _SM9Pairing_Miller_DblAndAdd(const SM9JacobPoint2Mont* Q, const SM9JacobPoint1Mont* P, SM9JacobPoint2Mont* T, SM9FP12Mont* f_num, SM9FP12Mont* f_den)
//{
//    // 6t + 2 = 0b100100000000000000000000000000000000000010000101011101100100111110
//    int32_t i = 0;
//    SM9FP12Mont _g_num = { 0 }, * g_num = &_g_num;
//    SM9FP12Mont _g_den = { 0 }, * g_den = &_g_den;
//
//    // set T to Q and f to 1
//    SM9JacobPoint2Mont _T_tmp = *Q, * T_tmp = &_T_tmp;
//    for (i = 1; i < 12; i++) UInt256_SetZero(f_num->fp1 + i); f_num->fp1[0] = *CONSTS_FP1_MONT_ONE;
//    for (i = 1; i < 12; i++) UInt256_SetZero(f_den->fp1 + i); f_den->fp1[0] = *CONSTS_FP1_MONT_ONE;
//
//#define _MILLER_DBL \
//    SM9Pairing_LinearDbl(T_tmp, P, g_num, g_den), \
//    SM9FP12_MontMul(f_num, f_num, f_num), SM9FP12_MontMul(f_den, f_den, f_den), \
//    SM9FP12_MontMul(f_num, g_num, f_num), SM9FP12_MontMul(f_den, g_den, f_den), \
//    SM9JacobPoint2Mont_Dbl(T_tmp, T_tmp)
//
//#define _MILLER_ADD \
//    SM9Pairing_LinearAdd(T_tmp, Q, P, g_num, g_den), \
//    SM9FP12_MontMul(f_num, g_num, f_num), SM9FP12_MontMul(f_den, g_den, f_den), \
//    SM9JacobPoint2Mont_Add(T_tmp, Q, T_tmp)
//
//    _MILLER_DBL; 
//    _MILLER_DBL;
//    _MILLER_DBL; _MILLER_ADD;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL; _MILLER_ADD;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL; _MILLER_ADD;
//    _MILLER_DBL;
//    _MILLER_DBL; _MILLER_ADD;
//    _MILLER_DBL;
//    _MILLER_DBL; _MILLER_ADD;
//    _MILLER_DBL; _MILLER_ADD;
//    _MILLER_DBL; _MILLER_ADD;
//    _MILLER_DBL;
//    _MILLER_DBL; _MILLER_ADD;
//    _MILLER_DBL; _MILLER_ADD;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL; _MILLER_ADD;
//    _MILLER_DBL;
//    _MILLER_DBL;
//    _MILLER_DBL; _MILLER_ADD;
//    _MILLER_DBL; _MILLER_ADD;
//    _MILLER_DBL; _MILLER_ADD;
//    _MILLER_DBL; _MILLER_ADD;
//    _MILLER_DBL; _MILLER_ADD;
//    _MILLER_DBL;
//
//#undef _MILLER_DBL
//#undef _MILLER_ADD
//
//    *T = *T_tmp;
//}

static
void _SM9Pairing_Miller_NAF(const SM9JacobPoint2Mont* Q, const SM9JacobPoint1Mont* P, SM9JacobPoint2Mont* T, SM9FP12Mont* f_num, SM9FP12Mont* f_den)
{
    // 6t + 2 = 0b100100000000000000000000000000000000000010000101011101100100111110
    int32_t i = 0;
    SM9FP12Mont _g_num = { 0 }, * g_num = &_g_num;
    SM9FP12Mont _g_den = { 0 }, * g_den = &_g_den;
    SM9JacobPoint2Mont _Q_neg = { 0 }, * Q_neg = &_Q_neg;

    // set T to Q and f to 1
    SM9JacobPoint2Mont _T_tmp = *Q, * T_tmp = &_T_tmp;
    for (i = 1; i < 12; i++) UInt256_SetZero(f_num->fp1 + i); f_num->fp1[0] = *CONSTS_FP1_MONT_ONE;
    for (i = 1; i < 12; i++) UInt256_SetZero(f_den->fp1 + i); f_den->fp1[0] = *CONSTS_FP1_MONT_ONE;

    SM9JacobPoint2Mont_Neg(Q, Q_neg);

#define _MILLER_DBL \
    SM9Pairing_LinearDbl(T_tmp, P, g_num, g_den), \
    SM9FP12_MontMul(f_num, f_num, f_num), SM9FP12_MontMul(f_den, f_den, f_den), \
    SM9FP12_MontMul(f_num, g_num, f_num), SM9FP12_MontMul(f_den, g_den, f_den), \
    SM9JacobPoint2Mont_Dbl(T_tmp, T_tmp)

#define _MILLER_ADD \
    SM9Pairing_LinearAdd(T_tmp, Q, P, g_num, g_den), \
    SM9FP12_MontMul(f_num, g_num, f_num), SM9FP12_MontMul(f_den, g_den, f_den), \
    SM9JacobPoint2Mont_Add(T_tmp, Q, T_tmp)

#define _MILLER_SUB \
    SM9Pairing_LinearAdd(T_tmp, Q_neg, P, g_num, g_den), \
    SM9FP12_MontMul(f_num, g_num, f_num), SM9FP12_MontMul(f_den, g_den, f_den), \
    SM9JacobPoint2Mont_Add(T_tmp, Q_neg, T_tmp)

    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL; _MILLER_ADD;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL; _MILLER_ADD;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL; _MILLER_ADD;
    _MILLER_DBL;
    _MILLER_DBL; _MILLER_ADD;
    _MILLER_DBL; _MILLER_ADD;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL; _MILLER_SUB;
    _MILLER_DBL;
    _MILLER_DBL; _MILLER_SUB;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL; _MILLER_ADD;
    _MILLER_DBL;
    _MILLER_DBL; _MILLER_ADD;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL;
    _MILLER_DBL; _MILLER_SUB;
    _MILLER_DBL;

#undef _MILLER_DBL
#undef _MILLER_ADD
#undef _MILLER_SUB

    *T = *T_tmp;
}

static
void SM9Pairing_Miller(const SM9JacobPoint2Mont* Q, const SM9JacobPoint1Mont* P, SM9JacobPoint2Mont* T, SM9FP12Mont* f_num, SM9FP12Mont* f_den)
{
     _SM9Pairing_Miller_NAF(Q, P, T, f_num, f_den);
}

static
void SM9JacobPoint2Mont_Pi1(const SM9JacobPoint2Mont* x, SM9JacobPoint2Mont* y)
{
    // Frobenius 1 in FP12
    // (((    11,  5), (     8,  2)), ((    10,  4), (     7,  1)), ((     9,  3), (     6,  0)))
    // (((p - w5, w5), (p - w2, w2)), ((p - w4, w4), (p - w1, w1)), ((p - w3, w3), (p - w0, w0)))

    // x: (p - w0, w0)
    SM9FP1_Neg(x->x.fp1 + 1, y->x.fp1 + 1); y->x.fp1[0] = x->x.fp1[0];

    // y: (p - w0, w0)
    SM9FP1_Neg(x->y.fp1 + 1, y->y.fp1 + 1); y->y.fp1[0] = x->y.fp1[0];

    // z: (p - w1, w1)
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F1, x->z.fp1 + 1, y->z.fp1 + 1); SM9FP1_Neg(y->z.fp1 + 1, y->z.fp1 + 1);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F1, x->z.fp1, y->z.fp1);
}

static
void SM9JacobPoint2Mont_Pi2(const SM9JacobPoint2Mont* x, SM9JacobPoint2Mont* y)
{
    // Frobenius 2 in FP12
    // (((    11,      5), ( 8,  2)), ((    10,      4), ( 7,  1)), ((     9,      3), ( 6,  0)))
    // (((p - w4, p - w4), (w4, w4)), ((p - w2, p - w2), (w2, w2)), ((p - w0, p - w0), (w0, w0)))

    // x: (w0, w0)
    y->x = x->x;

    // y: (w0, w0)
    y->y = x->y;

    // z: (w2, w2)
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F2, x->z.fp1 + 1, y->z.fp1 + 1);
    SM9FP1_MontMul(CONSTS_FP1_MONT_FP12_FROB_F2, x->z.fp1, y->z.fp1);
}

static
void SM9Pairing_FinalExp(const SM9FP12Mont* f_in, SM9FP12Mont* f_out)
{
    SM9FP12Mont _f = { 0 }, * f = &_f;
    SM9FP12Mont _f_num = { 0 }, * f_num = &_f_num;
    SM9FP12Mont _f_den = { 0 }, * f_den = &_f_den;
    SM9FP12Mont _tmp1 = { 0 }, * tmp1 = &_tmp1;
    SM9FP12Mont _tmp2 = { 0 }, * tmp2 = &_tmp2;
    SM9FP12Mont _y = { 0 }, * y = &_y;

#define _M  SM9FP12_MontMul
#define _I  SM9FP12_MontInv
#define _P  SM9FP12_MontPowU64
#define _F1 SM9FP12_MontFrob1
#define _F2 SM9FP12_MontFrob2
#define _F3 SM9FP12_MontFrob3
#define _F6 SM9FP12_MontFrob6

    // easy part
    _F6(f_in, f);                           // F6(f)
    _I(f_in, tmp1);                         // I(f)
    _M(f, tmp1, f);                         // f = F6(f)I(f)
    _F2(f, tmp1);                           // F2(f)
    _M(tmp1, f, f);                         // f = F2(f)f

    // hard part
    // y6, y5, y4, y3, y2, y1, y0
    //  -,  -,  -,  -,  +,  -,  +

    // y0
    _F1(f, y);                              // f_p
    _F2(f, tmp1);                           // f_p2
    _M(y, tmp1, y);                         // f_p * f_p2
    _F3(f, tmp1);                           // f_p3
    _M(y, tmp1, f_num);                     // f_p * f_p2 * f_p3

    // y2
    _P(f, 0x600000000058F98A, tmp1);        // f_t
    _P(tmp1, 0x600000000058F98A, tmp2);     // f_t2
    _F2(tmp2, y);                           // f_t2_p2
    _P(y, 6, y);                            // f_t2_p2^6
    _M(f_num, y, f_num);                    // y2 * y0

    // y1
    _M(f, f, f_den);                        // f * f

    // y3
    _F1(tmp1, y);                           // f_t_p
    _P(y, 12, y);                           // f_t_p^12
    _M(f_den, y, f_den);                    // y3 * y1

    // y4
    _F1(tmp2, y);                           // f_t2_p
    _M(y, tmp1, y);                         // f_t2_p * f_t
    _P(y, 18, y);                           // (f_t2_p * f_t)^18
    _M(f_den, y, f_den);                    // y4 * y3 * y1

    // y5
    _P(tmp2, 30, y);                        // f_t2^30
    _M(f_den, y, f_den);                    // y5 * y4 * y3 * y1

    // y6
    _P(tmp2, 0x600000000058F98A, tmp1);     // f_t3
    _F1(tmp1, y);                           // f_t3_p
    _M(y, tmp1, y);                         // f_t3_p * f_t3
    _P(y, 36, y);                           // (f_t3_p * f_t3)^36
    _M(f_den, y, f_den);                    // y6 * y5 * y4 * y3 * y1

    _I(f_den, f_den);
    _M(f_num, f_den, f_out);

#undef _M
#undef _I
#undef _P
#undef _F1
#undef _F2
#undef _F3
#undef _F6
}

void SM9Pairing_RAte(const SM9JacobPoint1Mont* P, const SM9JacobPoint2Mont* Q, SM9FP12Mont* f)
{
    SM9JacobPoint2Mont _T = { 0 }, * T = &_T;
    SM9FP12Mont _f_num = { 0 }, * f_num = &_f_num;
    SM9FP12Mont _f_den = { 0 }, * f_den = &_f_den;
    SM9FP12Mont _g_num = { 0 }, * g_num = &_g_num;
    SM9FP12Mont _g_den = { 0 }, * g_den = &_g_den;
    SM9JacobPoint2Mont _Q_pi = { 0 }, * Q_pi = &_Q_pi;

    SM9Pairing_Miller(Q, P, T, f_num, f_den);

    SM9JacobPoint2Mont_Pi1(Q, Q_pi);
    SM9Pairing_LinearAdd(T, Q_pi, P, g_num, g_den);
    SM9FP12_MontMul(f_num, g_num, f_num); SM9FP12_MontMul(f_den, g_den, f_den);
    SM9JacobPoint2Mont_Add(T, Q_pi, T);

    SM9JacobPoint2Mont_Pi2(Q, Q_pi); 
    SM9JacobPoint2Mont_Neg(Q_pi, Q_pi);
    SM9Pairing_LinearAdd(T, Q_pi, P, g_num, g_den);
    SM9FP12_MontMul(f_num, g_num, f_num); SM9FP12_MontMul(f_den, g_den, f_den);

    SM9FP12_MontInv(f_den, f_den);
    SM9FP12_MontMul(f_num, f_den, f);
    SM9Pairing_FinalExp(f, f);
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< R-ate Pairing <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
