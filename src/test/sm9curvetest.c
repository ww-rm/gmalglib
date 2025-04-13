#include <stdio.h>
#include <assert.h>
#include <gmalglib/sm9curve.h>
#include <Windows.h>

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
    UInt256 e = { .u8 = {7, 9} };

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
    //UInt256 x = { .u32 = { 0x80A892FF, 0xABC66E3C, 0xA7137D39, 0x720B17EC, 0x3F32DA5C, 0x4E0A6600, 0xA51F3AB8, 0x787ED7B8 } };
    //UInt256 y = { .u32 = { 0x4C57A7B1, 0x3F220F64, 0x9A8C49DC, 0x20287127, 0x1354900B, 0xB9FF85A3, 0x91E5ADC4, 0x769DE617 } };
    UInt256 x = { .u32 = { 0xAAAC8A0F, 0x67B4FED5, 0xD9DC5E20, 0xAB02C308, 0xFB2CB434, 0x91DE44CE, 0xF4659144, 0x1EDEE2C3 } };
    UInt256 y = { .u32 = { 0x37BE9A4C, 0x602BBC5E, 0x74AFF3B3, 0xDE81A533, 0x74C0F78F, 0x83BB7341, 0x5ECA35AB, 0x1C9B4C43 } };
    uint8_t pk[SM9_POINT1BYTES_MAX_LENGTH] = { 0x04 };
    UInt256_ToBytes(&x, pk + 1);
    UInt256_ToBytes(&y, pk + 33);

    SM9JacobPoint1Mont PK = { 0 };
    SM9Point1 PKK = { 0 };
    assert(SM9JacobPoint1Mont_FromBytes(pk, 65, &PK) == 0);

    pk[0] = 0x02;
    SM9JacobPoint1Mont_SetInf(&PK);
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
    UInt256 e = { .u8 = { 7, 9, 13 } };

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

void test_convert2_1()
{
    UInt256 x1 = { .u32 = { 0x1B94C408, 0x0AB1B679, 0x5E392CFB, 0x1CE0711C, 0x41B56501, 0xE48AFF4B, 0x3084F733, 0x9F64080B } };
    UInt256 x0 = { .u32 = { 0x96EA5E32, 0x8F14D656, 0x386A92DD, 0x414D2177, 0x24A3B573, 0x6CE843ED, 0x152D1F78, 0x29DBA116 } };
    UInt256 y1 = { .u32 = { 0xD0EF1C25, 0xE89E1408, 0x1A77F335, 0xAD3E2FDB, 0x47E3A0CB, 0xB57329F4, 0xABEA0112, 0x69850938 } };
    UInt256 y0 = { .u32 = { 0xB4E3216D, 0x0E75C05F, 0x5CDFF073, 0x1006E85F, 0xB7A46F74, 0x1A7CE027, 0xDDA532DA, 0x41E00A53 } };

    uint8_t pk[SM9_POINT2BYTES_MAX_LENGTH] = { 0x04 };

    UInt256_ToBytes(&x1, pk + 1);
    UInt256_ToBytes(&x0, pk + 1 + 32);
    UInt256_ToBytes(&y1, pk + 1 + 64);
    UInt256_ToBytes(&y0, pk + 1 + 96);

    SM9JacobPoint2Mont PK = { 0 };
    SM9Point2 PKK = { 0 };
    assert(SM9JacobPoint2Mont_FromBytes(pk, 65 + 64, &PK) == 0);

    pk[0] = 0x03;
    SM9JacobPoint2Mont_SetInf(&PK);
    assert(SM9JacobPoint2Mont_FromBytes(pk, 65, &PK) == 0);
    //SM9JacobPoint2Mont_Print(&PK);

    SM9JacobPoint2Mont_ToPoint(&PK, &PKK);
    assert(UInt256_Cmp(&y1, PKK.y.fp1 + 1) == 0 && UInt256_Cmp(&y0, PKK.y.fp1) == 0);

    printf("SM9 Convert 2.1 Test OK.\n");
}

void test_convert2_2()
{
    UInt256 x1 = { .u32 = { 0xECF41683, 0x40B2D31E, 0x37B64141, 0x9A059D35, 0x38301A13, 0xCC4785E9, 0x2C8C8796, 0x94736ACD } };
    UInt256 x0 = { .u32 = { 0x9A19B158, 0xB4F8C0B5, 0x28420B1C, 0xCF3C2F4F, 0x42979ACC, 0x3DBD9E53, 0xF5D8BC6C, 0x115BAE85 } };
    UInt256 y1 = { .u32 = { 0xA7D3B55F, 0xFA7BA92C, 0x4753FE74, 0xC447F384, 0xF7BEAF71, 0xCD760A0C, 0x70DA7600, 0x7AA5E475 } };
    UInt256 y0 = { .u32 = { 0xCDEB75C1, 0x32B50E31, 0x119EA447, 0x9D56734F, 0x4796D94C, 0x1DCE0870, 0xE7F7BFB5, 0x27538A62 } };

    uint8_t pk[SM9_POINT2BYTES_MAX_LENGTH] = { 0x04 };

    UInt256_ToBytes(&x1, pk + 1);
    UInt256_ToBytes(&x0, pk + 1 + 32);
    UInt256_ToBytes(&y1, pk + 1 + 64);
    UInt256_ToBytes(&y0, pk + 1 + 96);

    SM9JacobPoint2Mont PK = { 0 };
    SM9Point2 PKK = { 0 };
    assert(SM9JacobPoint2Mont_FromBytes(pk, 65 + 64, &PK) == 0);

    pk[0] = 0x03;
    SM9JacobPoint2Mont_SetInf(&PK);
    assert(SM9JacobPoint2Mont_FromBytes(pk, 65, &PK) == 0);
    //SM9JacobPoint2Mont_Print(&PK);

    SM9JacobPoint2Mont_ToPoint(&PK, &PKK);
    assert(UInt256_Cmp(&y1, PKK.y.fp1 + 1) == 0 && UInt256_Cmp(&y0, PKK.y.fp1) == 0);

    printf("SM9 Convert 2.2 Test OK.\n");
}

void make_table2()
{
    for (int i = 0; i < 32; i++) {
        SM9JacobPoint2Mont G = { 0 };
        SM9JacobPoint2Mont P = { 0 };

        SM9JacobPoint2Mont_SetInf(&P);

        // u8[i]G
        SM9JacobPoint2Mont_FromPoint(SM9_PARAMS_G2, &G);
        UInt256 k = { 0 };
        k.u8[i] = 0x01;
        SM9JacobPoint2Mont_Mul(&k, &G, &G);

        printf("    // u8[%d]\n", i);
        for (int j = 0; j < 256; j++)
        {
            printf("    {{.fp1 = {{.u64 = { ");
            printf("0x%016llX", P.x.fp1[0].u64[0]);
            for (int n = 1; n < 4; n++) printf(", 0x%016llX", P.x.fp1[0].u64[n]);
            printf(" }},\n");

            printf("              {.u64 = { ");
            printf("0x%016llX", P.x.fp1[1].u64[0]);
            for (int n = 1; n < 4; n++) printf(", 0x%016llX", P.x.fp1[1].u64[n]);
            printf(" }}}},\n");

            printf("     {.fp1 = {{.u64 = { ");
            printf("0x%016llX", P.y.fp1[0].u64[0]);
            for (int n = 1; n < 4; n++) printf(", 0x%016llX", P.y.fp1[0].u64[n]);
            printf(" }},\n");

            printf("              {.u64 = { ");
            printf("0x%016llX", P.y.fp1[1].u64[0]);
            for (int n = 1; n < 4; n++) printf(", 0x%016llX", P.y.fp1[1].u64[n]);
            printf(" }}}},\n");

            printf("     {.fp1 = {{.u64 = { ");
            printf("0x%016llX", P.z.fp1[0].u64[0]);
            for (int n = 1; n < 4; n++) printf(", 0x%016llX", P.z.fp1[0].u64[n]);
            printf(" }},\n");

            printf("              {.u64 = { ");
            printf("0x%016llX", P.z.fp1[1].u64[0]);
            for (int n = 1; n < 4; n++) printf(", 0x%016llX", P.z.fp1[1].u64[n]);
            printf(" }}}}},\n");

            SM9JacobPoint2Mont_Add(&P, &G, &P);
        }
        printf("\n");
    }
}

void test_fp12()
{
    //SM9FP12Mont x = { 0 };
    //for (int i = 1; i <= 12; i++)
    //{
    //    x.fp1[i - 1].u8[0] = i;
    //}
    //SM9FP12_ToMont(&x, &x);
    //SM9FP12Mont_Print(&x); printf("\n");

    //UInt256 e = { .u8 = { 1, 2, 3 } };
    ////SM9FP12_MontPow(&x, &e, &x);
    ////SM9FP12Mont_Print(&x); printf("\n");

    //SM9FP12_MontInv(&x, &x);
    //SM9FP12Mont_Print(&x); printf("\n");
}

void SM9FP12_FromMont(const SM9FP12Mont* x, SM9FP12* y);
void test_pairing()
{
    SM9JacobPoint1Mont G1 = { 0 };

    SM9JacobPoint2Mont G2 = { 0 };

    SM9FP12Mont f = { 0 };

    LARGE_INTEGER s, e;
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&s);

    SM9JacobPoint1Mont_FromPoint(SM9_PARAMS_G1, &G1);
    SM9JacobPoint2Mont_FromPoint(SM9_PARAMS_G2, &G2);
    //SM9JacobPoint1Mont_Dbl(&G1, &G1);
    //SM9JacobPoint2Mont_Dbl(&G2, &G2);

    //SM9JacobPoint1Mont_Print(&G1);
    //SM9JacobPoint2Mont_Print(&G2);

    for (int i = 0; i < 10000; i++)
    {
        SM9Pairing_RAte(&G1, &G2, &f);
        //SM9FP12_FromMont(&f, &f);
    }

    QueryPerformanceCounter(&e);

    double elapsed = (double)(e.QuadPart - s.QuadPart) / (double)freq.QuadPart;
    printf("Elapsed: %.6f ms\n", elapsed);

    //SM9Pairing_RAte(&G1, &G2, &f);
    //SM9FP12Mont_Print(&f);
}

int main()
{
    //test_curve1();

    //test_convert1();

    ////make_table1();

    //test_curve2();

    //test_convert2_1();
    //test_convert2_2();

    ////make_table2();

    ////test_fp12();

    test_pairing();

    return 0;
}