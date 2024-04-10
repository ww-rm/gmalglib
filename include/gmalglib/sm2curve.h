#ifndef GMALGLIB_SM2CURVE_H
#define GMALGLIB_SM2CURVE_H

#include <stdint.h>
#include <gmalglib/bignum.h>

#define SM2_PARAMS_LENGTH           32

#define SM2_PCMODE_RAW              0
#define SM2_PCMODE_COMPRESS         1
#define SM2_PCMODE_MIX              2

typedef UInt256 SM2ModP;
typedef UInt256 SM2ModN;
typedef SM2ModP SM2ModPMont;
typedef SM2ModP SM2ModNMont;

typedef struct _SM2Point {
    SM2ModP x;
    SM2ModP y;
    int is_inf;
} SM2Point;

typedef struct _SM2JacobPointMont {
    SM2ModPMont x;
    SM2ModPMont y;
    SM2ModPMont z;
} SM2JacobPointMont;

#ifdef __cplusplus
extern "C" {
#endif

extern const UInt256* const SM2_PARAMS_P;
extern const UInt256* const SM2_PARAMS_A;
extern const UInt256* const SM2_PARAMS_B;
extern const UInt256* const SM2_PARAMS_GX;
extern const UInt256* const SM2_PARAMS_GY;
extern const UInt256* const SM2_PARAMS_N;

void SM2ModP_Add(const SM2ModP* x, const SM2ModP* y, SM2ModP* z);
void SM2ModP_Sub(const SM2ModP* x, const SM2ModP* y, SM2ModP* z);

void SM2ModP_ToMont(const SM2ModP* x, SM2ModPMont* y);
void SM2ModP_FromMont(const SM2ModPMont* x, SM2ModP* y);

void SM2ModP_MontAdd(const SM2ModPMont* x, const SM2ModPMont* y, SM2ModPMont* z);
void SM2ModP_MontSub(const SM2ModPMont* x, const SM2ModPMont* y, SM2ModPMont* z);
void SM2ModP_MontMul(const SM2ModPMont* x, const SM2ModPMont* y, SM2ModPMont* z);
void SM2ModP_MontPow(const SM2ModPMont* x, const UInt256* e, SM2ModPMont* y);
void SM2ModP_MontNeg(const SM2ModPMont* x, SM2ModPMont* y);
void SM2ModP_MontInv(const SM2ModPMont* x, SM2ModPMont* y);

uint64_t SM2Point_ToBytes(const SM2Point* X, uint8_t* bytes, int pc_mode);
void SM2Point_FromBytes(const uint8_t* bytes, uint64_t bytes_len, SM2Point* X);

void SM2Point_ToJacobMont(const SM2Point* X, SM2JacobPointMont* Y);
void SM2Point_FromJacobMont(const SM2JacobPointMont* X, SM2Point* Y);

int SM2JacobPointMont_IsInf(const SM2JacobPointMont* X);
void SM2JacobPointMont_SetInf(SM2JacobPointMont* X);
int SM2JacobPointMont_IsEqual(const SM2JacobPointMont* X, const SM2JacobPointMont* Y);
void SM2JacobPointMont_Add(const SM2JacobPointMont* X, const SM2JacobPointMont* Y, SM2JacobPointMont* Z);
void SM2JacobPointMont_Mul(const UInt256* k, const SM2JacobPointMont* X, SM2JacobPointMont* Y);
void SM2JacobPointMont_Neg(const SM2JacobPointMont* X, SM2JacobPointMont* Y);
void SM2JacobPointMont_Sub(const SM2JacobPointMont* X, const SM2JacobPointMont* Y, SM2JacobPointMont* Z);

#ifdef _DEBUG

void SM2Point_Print(const SM2Point* X);
void SM2JacobPointMont_Print(const SM2JacobPointMont* X);

#endif // _DEBUG


#ifdef __cplusplus
}
#endif

#endif // !GMALGLIB_SM2CURVE_H
