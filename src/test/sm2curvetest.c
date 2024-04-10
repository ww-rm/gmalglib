#include <stdio.h>
#include <gmalglib/sm2curve.h>

void test_add()
{
    SM2Point G = { *SM2_PARAMS_GX, *SM2_PARAMS_GY };

    SM2JacobPointMont G_mont = { 0 };
    SM2Point_ToJacobMont(&G, &G_mont);
    SM2JacobPointMont_Print(&G_mont); printf("\n");

    UInt256 e = { 4 };
    SM2JacobPointMont H = G_mont;
    SM2JacobPointMont K = G_mont;
    SM2JacobPointMont S = G_mont;

    SM2JacobPointMont_Add(&H, &H, &H);
    SM2JacobPointMont_Add(&H, &H, &H);
    SM2JacobPointMont_Print(&H); printf("\n");

    SM2JacobPointMont_Add(&K, &K, &K);
    SM2JacobPointMont_Add(&K, &G_mont, &K);
    SM2JacobPointMont_Add(&K, &G_mont, &K);
    SM2JacobPointMont_Print(&K); printf("\n");

    SM2JacobPointMont_Mul(SM2_PARAMS_N, &S, &S);
    SM2JacobPointMont_Print(&S); printf("\n");

    printf("");
}

int main()
{
    printf("========== SM2Curve Test ==========\n");

    test_add();

    return 0;
}
