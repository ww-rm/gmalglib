#ifndef GMALGLIB_SM9CURVE_H
#define GMALGLIB_SM9CURVE_H

#include <stdint.h>
#include <gmalglib/bignum.h>

#define SM9_PARAMS_LENGTH                           32

#define SM9_PCMODE_RAW                              0
#define SM9_PCMODE_COMPRESS                         1
#define SM9_PCMODE_MIX                              2
#define SM9_POINT1BYTES_INF_LENGTH                  1
#define SM9_POINT1BYTES_HALF_LENGTH                 (1 + SM9_PARAMS_LENGTH)
#define SM9_POINT1BYTES_FULL_LENGTH                 (SM9_POINT1BYTES_HALF_LENGTH + SM9_PARAMS_LENGTH)
#define SM9_POINT1BYTES_MAX_LENGTH                  SM9_POINT1BYTES_FULL_LENGTH
#define SM9_GET_POINT1BYTES_LENGTH(pc_mode)         (((pc_mode) == SM9_PCMODE_COMPRESS) ? SM9_POINT1BYTES_HALF_LENGTH : SM9_POINT1BYTES_FULL_LENGTH)

#define SM9CURVE_ERR_NOTONCURVE                     -1
#define SM9CURVE_ERR_INVALIDPC                      -2

typedef UInt256 SM9FP1;
typedef SM9FP1 SM9FP1Mont;

typedef union _SM9FP2 {
    SM9FP1 fp1[2];
} SM9FP2, SM9FP2Mont;

typedef union _SM9FP4 {
    SM9FP2 fp2[2];
    SM9FP1 fp1[4];
} SM9FP4, SM9FP4Mont;

typedef union _SM9FP12 {
    SM9FP4 fp4[3];
    SM9FP1 fp1[12];
} SM9FP12, SM9FP12Mont;

typedef struct _SM9Point1 {
    SM9FP1 x;
    SM9FP1 y;
    int is_inf;
} SM9Point1;

typedef struct _SM9Point2 {
    SM9FP2 x;
    SM9FP2 y;
    int is_inf;
} SM9Point2;

typedef struct _SM9JacobPoint1Mont {
    SM9FP1Mont x;
    SM9FP1Mont y;
    SM9FP1Mont z;
} SM9JacobPoint1Mont;

typedef struct _SM9JacobPoint2Mont {
    SM9FP2Mont x;
    SM9FP2Mont y;
    SM9FP2Mont z;
} SM9JacobPoint2Mont;

#ifdef __cplusplus
extern "C" {
#endif

extern const UInt256* const SM9_PARAMS_P;
extern const UInt256* const SM9_PARAMS_B;
extern const SM9Point1* const SM9_PARAMS_G1;
extern const UInt256* const SM9_PARAMS_N;

void SM9JacobPoint1Mont_ToPoint(const SM9JacobPoint1Mont* X, SM9Point1* Y);
void SM9JacobPoint1Mont_FromPoint(const SM9Point1* X, SM9JacobPoint1Mont* Y);

uint64_t SM9Point1_ToBytes(const SM9Point1* X, int pc_mode, uint8_t* bytes);
int SM9Point1_FromBytes(const uint8_t* bytes, uint64_t bytes_len, SM9Point1* X);
uint64_t SM9JacobPoint1Mont_ToBytes(const SM9JacobPoint1Mont* X, int pc_mode, uint8_t* bytes);
int SM9JacobPoint1Mont_FromBytes(const uint8_t* bytes, uint64_t bytes_len, SM9JacobPoint1Mont* X);

int SM9JacobPoint1Mont_IsInf(const SM9JacobPoint1Mont* X);
void SM9JacobPoint1Mont_SetInf(SM9JacobPoint1Mont* X);
int SM9JacobPoint1Mont_IsOnCurve(const SM9JacobPoint1Mont* X);
int SM9JacobPoint1Mont_IsEqual(const SM9JacobPoint1Mont* X, const SM9JacobPoint1Mont* Y);
void SM9JacobPoint1Mont_Dbl(const SM9JacobPoint1Mont* X, SM9JacobPoint1Mont* Y);
void SM9JacobPoint1Mont_Add(const SM9JacobPoint1Mont* X, const SM9JacobPoint1Mont* Y, SM9JacobPoint1Mont* Z);
void SM9JacobPoint1Mont_Mul(const UInt256* k, const SM9JacobPoint1Mont* X, SM9JacobPoint1Mont* Y);
void SM9JacobPoint1Mont_Neg(const SM9JacobPoint1Mont* X, SM9JacobPoint1Mont* Y);
void SM9JacobPoint1Mont_Sub(const SM9JacobPoint1Mont* X, const SM9JacobPoint1Mont* Y, SM9JacobPoint1Mont* Z);
void SM9JacobPoint1Mont_MulG1(const UInt256* k, SM9JacobPoint1Mont* X);

#ifdef _DEBUG

void SM9Point1_Print(const SM9Point1* X);
void SM9JacobPoint1Mont_Print(const SM9JacobPoint1Mont* X);

#endif // _DEBUG

#ifdef __cplusplus
}
#endif

#endif // !GMALGLIB_SM9CURVE_H
