#include <stdio.h>
#include <assert.h>
#include <gmalglib/sm9curve.h>

void test_curve1()
{
    SM9Point1 _G = *SM9_PARAMS_G1;
    SM9JacobPoint1Mont G = { 0 };
    SM9JacobPoint1Mont R = {
        {.u32 = { 0x8013BA8A, 0x4B131575, 0xE5C40522, 0xC1766D14, 0xBCDB3562, 0xC2934A22, 0xD2488DDF, 0x2B25F38A }},
        {.u32 = { 0x617676AF, 0xD1A4E6F3, 0x58214CBB, 0x035E0834, 0x1C809B69, 0x5B50BE74, 0xC8E95BAF, 0x2ADC9853 }},
        {.u32 = { 0xF3BEAA11, 0xF77F916B, 0xBDE65C11, 0x08B9E630, 0x5ABB1839, 0x8EC2FBD2, 0xC4E3895E, 0x2DEAE2EE }}
    };
    SM9JacobPoint1Mont A = { 0 };
    SM9JacobPoint1Mont B = { 0 };
    SM9JacobPoint1Mont C = { 0 };
    UInt256 e = { 7, 9 };

    SM9JacobPoint1Mont_FromPoint(&_G, &G);
    assert(SM9JacobPoint1Mont_IsOnCurve(&G));

    A = G;
    SM9JacobPoint1Mont_Add(&G, &A, &A);
    assert(SM9JacobPoint1Mont_IsEqual(&A, &R));

    //SM9JacobPoint1Mont_Add(&G, &A, &A);
    //SM9JacobPoint1Mont_Add(&G, &A, &A);
    //SM9JacobPoint1Mont_Add(&G, &A, &A);
    //SM9JacobPoint1Mont_Add(&G, &A, &A);
    //SM9JacobPoint1Mont_Add(&G, &A, &A);
    SM9JacobPoint1Mont_Mul(&e, &G, &A);

    SM9JacobPoint1Mont_MulG1(&e, &B);

    assert(SM9JacobPoint1Mont_IsEqual(&A, &B));

    SM9JacobPoint1Mont_Neg(&A, &B);
    SM9JacobPoint1Mont_Add(&A, &B, &B);
    assert(SM9JacobPoint1Mont_IsInf(&B));

    SM9JacobPoint1Mont_Mul(SM9_PARAMS_N, &A, &C);
    assert(SM9JacobPoint1Mont_IsInf(&C));

    printf("SM9 Curve 1 Test OK.\n");
}

void test_convert1()
{
    UInt256 x = { .u32 = { 0x80A892FF, 0xABC66E3C, 0xA7137D39, 0x720B17EC, 0x3F32DA5C, 0x4E0A6600, 0xA51F3AB8, 0x787ED7B8 } };
    UInt256 y = { .u32 = { 0x4C57A7B1, 0x3F220F64, 0x9A8C49DC, 0x20287127, 0x1354900B, 0xB9FF85A3, 0x91E5ADC4, 0x769DE617 } };
    uint8_t pk[SM9_POINT1BYTES_MAX_LENGTH] = { 0x04 };
    UInt256_ToBytes(&x, pk + 1);
    UInt256_ToBytes(&y, pk + 33);

    SM9JacobPoint1Mont PK = { 0 };
    SM9Point1 PKK = { 0 };
    assert(SM9JacobPoint1Mont_FromBytes(pk, 65, &PK) == 0);

    pk[0] = 0x03;
    assert(SM9JacobPoint1Mont_FromBytes(pk, 33, &PK) == 0);

    SM9JacobPoint1Mont_ToPoint(&PK, &PKK);
    assert(UInt256_Cmp(&y, &PKK.y) == 0);

    printf("SM9 Convert 1 Test OK.\n");
}

void make_table()
{
    for (int i = 0; i < 32; i++) {
        SM9JacobPoint1Mont G = { 0 };
        SM9JacobPoint1Mont P = { 0 };

        SM9JacobPoint1Mont_SetInf(&P);

        // u8[i]G
        SM9JacobPoint1Mont_FromPoint(SM9_PARAMS_G1, &G);
        UInt256 k = { 0 };
        k.u8[i] = 0x01;
        SM9JacobPoint1Mont_Mul(&k, &G, &G);

        printf("    // u8[%d]\n", i);
        for (int j = 0; j < 256; j++)
        {
            printf("    {{ .u64 = { ");
            printf("0x%016llX", P.x.u64[0]);
            for (int n = 1; n < 4; n++)
                printf(", 0x%016llX", P.x.u64[n]);
            printf(" }},\n");

            printf("     { .u64 = { ");
            printf("0x%016llX", P.y.u64[0]);
            for (int n = 1; n < 4; n++)
                printf(", 0x%016llX", P.y.u64[n]);
            printf(" }},\n");

            printf("     { .u64 = { ");
            printf("0x%016llX", P.z.u64[0]);
            for (int n = 1; n < 4; n++)
                printf(", 0x%016llX", P.z.u64[n]);
            printf(" }}},\n");

            SM9JacobPoint1Mont_Add(&P, &G, &P);
        }
        printf("\n");
    }
}


int main()
{
    test_curve1();

    test_convert1();

    //make_table();

    return 0;
}