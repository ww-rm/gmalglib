#include <stdio.h>
#include <assert.h>
#include <gmalglib/sm2curve.h>

void test_curve()
{
    SM2Point _G = *SM2_PARAMS_G;
    SM2JacobPointMont G = { 0 };
    SM2JacobPointMont R = { 
        {.u32 = {0xE63C48D6, 0x2EDA8381, 0xD5B614A6, 0x03243D78, 0x26014409, 0x7AEF3E86, 0x76A3B1F7, 0x398874C4}},
        {.u32 = {0xC1A73E40, 0x28385681, 0xF6CF4DC1, 0x29F04A88, 0x64921D48, 0x5A86A7AE, 0xDAF4FD47, 0x1FBBDFDD}},
        {.u32 = {0x785A1BBA, 0x826A9CB2, 0x1A652BF5, 0x83EBCAF1, 0xDC5491F1, 0x1A99F60C, 0x03AE6B7B, 0xC79ACBA9}}
    };
    SM2JacobPointMont A = { 0 };
    SM2JacobPointMont B = { 0 };
    SM2JacobPointMont C = { 0 };
    UInt256 e = { 7, 9 };

    SM2JacobPointMont_FromPoint(&_G, &G);
    assert(SM2JacobPointMont_IsOnCurve(&G));

    A = G;
    SM2JacobPointMont_Add(&G, &A, &A);
    assert(SM2JacobPointMont_IsEqual(&A, &R));

    //SM2JacobPointMont_Add(&G, &A, &A);
    //SM2JacobPointMont_Add(&G, &A, &A);
    //SM2JacobPointMont_Add(&G, &A, &A);
    //SM2JacobPointMont_Add(&G, &A, &A);
    //SM2JacobPointMont_Add(&G, &A, &A);
    SM2JacobPointMont_Mul(&e, &G, &A);

    SM2JacobPointMont_MulG(&e, &B);

    assert(SM2JacobPointMont_IsEqual(&A, &B));

    SM2JacobPointMont_Neg(&A, &B);
    SM2JacobPointMont_Add(&A, &B, &B);
    assert(SM2JacobPointMont_IsInf(&B));

    SM2JacobPointMont_Mul(SM2_PARAMS_N, &A, &C);
    assert(SM2JacobPointMont_IsInf(&C));

    printf("SM2 Curve Test OK.\n");
}

void test_convert()
{
    UInt256 x = { .u32 = {0x56F35020, 0x6BB08FF3, 0x1833FC07, 0x72179FAD, 0x1E4BC5C6, 0x50DD7D16, 0x1E5421A1, 0x09F9DF31} };
    UInt256 y = { .u32 = {0x2DA9AD13, 0x6632F607, 0xF35E084A, 0x0AED05FB, 0x8CC1AA60, 0x2DC6EA71, 0xE26775A5, 0xCCEA490C} };
    uint8_t pk[SM2_POINTBYTES_MAX_LENGTH] = { 0x04 };
    UInt256_ToBytes(&x, pk + 1);
    UInt256_ToBytes(&y, pk + 33);

    SM2JacobPointMont PK = { 0 };
    SM2Point PKK = { 0 };
    assert(SM2JacobPointMont_FromBytes(pk, 65, &PK) == 0);

    pk[0] = 0x03;
    assert(SM2JacobPointMont_FromBytes(pk, 33, &PK) == 0);

    SM2JacobPointMont_ToPoint(&PK, &PKK);
    assert(UInt256_Cmp(&y, &PKK.y) == 0);

    printf("SM2 Convert Test OK.\n");
}

void make_table()
{
    for (int i = 0; i < 32; i++) {
        SM2JacobPointMont G = { 0 };
        SM2JacobPointMont P = { 0 };

        SM2JacobPointMont_SetInf(&P);

        // u8[i]G
        SM2JacobPointMont_FromPoint(SM2_PARAMS_G, &G);
        UInt256 k = { 0 };
        k.u8[i] = 0x01;
        SM2JacobPointMont_Mul(&k, &G, &G);

        printf("    // u8[%d]\n", i);
        for (int j = 0; j < 256; j++)
        {
            printf("    {{.u32 = { ");
            printf("0x%08X", P.x.u32[0]);
            for (int n = 1; n < 8; n++)
                printf(", 0x%08X", P.x.u32[n]);
            printf(" }},\n");

            printf("     {.u32 = { ");
            printf("0x%08X", P.y.u32[0]);
            for (int n = 1; n < 8; n++)
                printf(", 0x%08X", P.y.u32[n]);
            printf(" }},\n");

            printf("     {.u32 = { ");
            printf("0x%08X", P.z.u32[0]);
            for (int n = 1; n < 8; n++)
                printf(", 0x%08X", P.z.u32[n]);
            printf(" }}},\n");

            SM2JacobPointMont_Add(&P, &G, &P);
        }
        printf("\n");
    }
}


int main()
{
    printf("========== SM2Curve Test ==========\n");

    test_curve();
    test_convert();

    //make_table();

    return 0;
}
