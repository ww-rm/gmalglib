#include <stdio.h>
#include <assert.h>
#include <gmalglib/sm9curve.h>

void test_curve1()
{
    SM9Point1 _G = *SM9_PARAMS_G1;
    SM9JacobPoint1Mont G = { 0 };
    SM9JacobPoint1Mont R = {
        {.u32 = { 0x8F0FDE68, 0x08FDF254, 0x804D6DD4, 0xC80DDEBF, 0x2905B7CA, 0xC8CEF528, 0x34132464, 0x6007E084 }},
        {.u32 = { 0x962DACA2, 0x01040281, 0x896409D2, 0x859CBBBE, 0x4B059E38, 0x5F0DD9B7, 0xABF00FD4, 0x00C02CCF }},
        {.u32 = { 0x1CAEBA83, 0x1A9064D8, 0xE5851124, 0xDE0D6CB4, 0x0A7138BA, 0x29FC54B0, 0xFD5C590E, 0x49BFFFFF }}
    };
    SM9JacobPoint1Mont A = { 0 };
    SM9JacobPoint1Mont B = { 0 };
    SM9JacobPoint1Mont C = { 0 };
    UInt256 e = { 7, 9 };

    SM9JacobPoint1Mont_FromPoint(&_G, &G);
    assert(SM9JacobPoint1Mont_IsOnCurve(&G));

    A = G;
    SM9JacobPoint1Mont_Add(&G, &A, &A);
    //SM9JacobPoint1Mont_Print(&A);
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

void make_table1()
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

void test_curve2()
{
    SM9Point2 _G = *SM9_PARAMS_G2;
    SM9JacobPoint2Mont G = { 0 };
    SM9JacobPoint2Mont G_dbl = {
        {.fp1 = {{.u32 = { 0x70C32949, 0x814E681A, 0xB140479A, 0x14C6A2BF, 0x99E0B315, 0x95CC9ED9, 0x72DAACE1, 0x43723007 }},
                 {.u32 = { 0xDCF3D8C2, 0xFB7EB8A0, 0x276FCF4E, 0xF03B823F, 0x76822AC0, 0xF72E796B, 0x354177C4, 0x50406B3E }}}},
        {.fp1 = {{.u32 = { 0xF5F34A70, 0x9AEDF94A, 0x668E95D3, 0xC073DDFD, 0x042ACC9A, 0x4C5EDF11, 0x8FF6AE1D, 0x80A5EA22 }},
                 {.u32 = { 0xA9A03880, 0x6B979082, 0xCD2CE885, 0x0052AE25, 0x126B4F55, 0x600DF0A6, 0xF4373FAC, 0x07E0D5A3 }}}},
        {.fp1 = {{.u32 = { 0x1CAEBA83, 0x1A9064D8, 0xE5851124, 0xDE0D6CB4, 0x0A7138BA, 0x29FC54B0, 0xFD5C590E, 0x49BFFFFF }},
                 {.u64 = { 0x0, 0x0, 0x0, 0x0 }}}}
    };
    SM9JacobPoint2Mont A = { 0 };
    SM9JacobPoint2Mont B = { 0 };
    SM9JacobPoint2Mont C = { 0 };
    UInt256 e = { 7, 9, 13 };

    SM9JacobPoint2Mont_FromPoint(&_G, &G);
    assert(SM9JacobPoint2Mont_IsOnCurve(&G));

    A = G;
    SM9JacobPoint2Mont_Dbl(&A, &A);
    //SM9JacobPoint2Mont_Print(&A);
    //SM9JacobPoint2Mont_Print(&G_dbl);
    assert(SM9JacobPoint2Mont_IsEqual(&A, &G_dbl));

    SM9JacobPoint2Mont_Mul(&e, &G, &A);
    //SM9JacobPoint2Mont_Add(&G, &A, &A);
    //SM9JacobPoint2Mont_Add(&G, &A, &A);
    //SM9JacobPoint2Mont_Add(&G, &A, &A);
    //SM9JacobPoint2Mont_Add(&G, &A, &A);
    //SM9JacobPoint2Mont_Add(&G, &A, &A);
    SM9JacobPoint2Mont_MulG2(&e, &B);

    assert(SM9JacobPoint2Mont_IsEqual(&A, &B));

    SM9JacobPoint2Mont_Neg(&A, &B);
    SM9JacobPoint2Mont_Add(&A, &B, &B);
    assert(SM9JacobPoint2Mont_IsInf(&B));

    SM9JacobPoint2Mont_Mul(SM9_PARAMS_N, &A, &C);
    assert(SM9JacobPoint2Mont_IsInf(&C));

    printf("SM9 Curve 2 Test OK.\n");
}

int main()
{
    test_curve1();

    test_convert1();

    //make_table1();

    test_curve2();

    return 0;
}