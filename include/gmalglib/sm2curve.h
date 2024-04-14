#ifndef GMALGLIB_SM2CURVE_H
#define GMALGLIB_SM2CURVE_H

#include <stdint.h>
#include <gmalglib/bignum.h>

#define SM2_PARAMS_LENGTH                           32

#define SM2_PCMODE_RAW                              0
#define SM2_PCMODE_COMPRESS                         1
#define SM2_PCMODE_MIX                              2
#define SM2_POINTBYTES_INF_LENGTH                   1
#define SM2_POINTBYTES_HALF_LENGTH                  (1 + SM2_PARAMS_LENGTH)
#define SM2_POINTBYTES_FULL_LENGTH                  (SM2_POINTBYTES_HALF_LENGTH + SM2_PARAMS_LENGTH)
#define SM2_POINTBYTES_MAX_LENGTH                   SM2_POINTBYTES_FULL_LENGTH
#define SM2_GET_POINTBYTES_LENGTH(pc_mode)          (((pc_mode) == SM2_PCMODE_COMPRESS) ? SM2_POINTBYTES_HALF_LENGTH : SM2_POINTBYTES_FULL_LENGTH)

#define SM2CURVE_ERR_NOTONCURVE                     -1
#define SM2CURVE_ERR_INVALIDPC                      -2

typedef UInt256 SM2ModP;
typedef SM2ModP SM2ModPMont;

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
extern const SM2Point* const SM2_PARAMS_G;
extern const UInt256* const SM2_PARAMS_N;

void SM2JacobPointMont_ToPoint(const SM2JacobPointMont* X, SM2Point* Y);
void SM2JacobPointMont_FromPoint(const SM2Point* X, SM2JacobPointMont* Y);

uint64_t SM2JacobPointMont_ToBytes(const SM2JacobPointMont* X, int pc_mode, uint8_t* bytes);
int SM2JacobPointMont_FromBytes(const uint8_t* bytes, uint64_t bytes_len, SM2JacobPointMont* X);

int SM2JacobPointMont_IsInf(const SM2JacobPointMont* X);
void SM2JacobPointMont_SetInf(SM2JacobPointMont* X);
int SM2JacobPointMont_IsOnCurve(const SM2JacobPointMont* X);
int SM2JacobPointMont_IsEqual(const SM2JacobPointMont* X, const SM2JacobPointMont* Y);
void SM2JacobPointMont_Add(const SM2JacobPointMont* X, const SM2JacobPointMont* Y, SM2JacobPointMont* Z);
void SM2JacobPointMont_Mul(const UInt256* k, const SM2JacobPointMont* X, SM2JacobPointMont* Y);
void SM2JacobPointMont_Neg(const SM2JacobPointMont* X, SM2JacobPointMont* Y);
void SM2JacobPointMont_Sub(const SM2JacobPointMont* X, const SM2JacobPointMont* Y, SM2JacobPointMont* Z);
void SM2JacobPointMont_MulG(const UInt256* k, SM2JacobPointMont* X);

#ifdef _DEBUG

void SM2Point_Print(const SM2Point* X);
void SM2JacobPointMont_Print(const SM2JacobPointMont* X);

#endif // _DEBUG


#ifdef __cplusplus
}
#endif

#endif // !GMALGLIB_SM2CURVE_H
