#include <stdio.h>
#include <gmalglib/sm2curve.h>


int main()
{
    printf("========== SM2Curve Test ==========\n");

    SM2Point G = { *SM2_PARAMS_GX, *SM2_PARAMS_GY };
    SM2Point_Print(&G);

    SM2JacobPointMont G_mont = { 0 };
    SM2Point_ToJacobMont(&G, &G_mont);
    SM2JacobPointMont_Print(&G_mont);

    SM2JacobPointMont_Dbl(&G_mont, &G_mont);
    SM2JacobPointMont_Print(&G_mont);

    return 0;
}
