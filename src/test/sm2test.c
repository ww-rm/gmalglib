#include <stdio.h>
#include <assert.h>
#include <gmalglib/sm2.h>

UInt256 sm2_sk = { .u32 = {
    0x4DF7C5B8, 0x42FB81EF, 0x2860B51A, 0x88939369, 0xC6D39F95, 0x3F36E38A, 0x7B2144B1, 0x3945208F
} };

SM2Point sm2_pk = {
    {.u32 = {0x56F35020, 0x6BB08FF3, 0x1833FC07, 0x72179FAD, 0x1E4BC5C6, 0x50DD7D16, 0x1E5421A1, 0x09F9DF31}},
    {.u32 = {0x2DA9AD13, 0x6632F607, 0xF35E084A, 0x0AED05FB, 0x8CC1AA60, 0x2DC6EA71, 0xE26775A5, 0xCCEA490C}},
    0
};

UInt256 test_sign_rnd = { .u32 = {
    0xEAC1BC21, 0x6D54B80D, 0x3CDBE4CE, 0xEF3CC1FA, 0xD9C02DCC, 0x16680F3A, 0xD506861A, 0x59276E27
} };

int test_sign_rnd_proc(void* obj, uint64_t bytes_len, uint8_t* bytes)
{
    *(UInt256*)bytes = test_sign_rnd;
    return 1;
}

RandomAlg test_sign_rnd_alg = { test_sign_rnd_proc, NULL };

UInt256 test_sign_e = { .u32 = {
    0xC0D28640, 0xF4486FDF, 0x19CE7B31, 0x17E6AB5A, 0x534382EB, 0xACE692ED, 0xBA45ACCA, 0xF0B43E94
} };

UInt256 test_sign_r = { .u32 = {
    0xEEE720B3, 0x43AC7EAC, 0x27D5B741, 0x5944DA38, 0xE1BB81A1, 0x0EEAC513, 0x48D2C463, 0xF5A03B06
} };

UInt256 test_sign_s = { .u32 = {
    0x85BBC1AA, 0x840B69C4, 0x1F7F42D4, 0xBB9038FD, 0x0D421CA1, 0x763182BC, 0xDF212FD8, 0xB1B6AA29
} };

void test_sign_digest()
{
    uint8_t sk[SM2_SK_LENGTH] = { 0 };
    UInt256_ToBytes(&sm2_sk, sk);

    uint8_t pk[SM2_PK_MAX_LENGTH] = { 0x04 };
    UInt256_ToBytes(&sm2_pk.x, pk + 1);
    UInt256_ToBytes(&sm2_pk.y, pk + 33);

    uint8_t digest[SM3_DIGEST_LENGTH] = { 0 };
    uint8_t signature[SM2_SIGNATURE_LENGTH] = { 0 };
    uint8_t* r = signature;
    uint8_t* s = signature + SM2_SIGN_R_LENGTH;
    UInt256 r_num = { 0 };
    UInt256 s_num = { 0 };

    SM2 sm2 = { 0 };
    assert(SM2_Init(&sm2, sk, pk, 65, NULL, 0, SM2_PCMODE_RAW, &test_sign_rnd_alg) == 0);

    UInt256_ToBytes(&test_sign_e, digest);
    assert(SM2_SignDigest(&sm2, digest, signature) == 0);
    UInt256_FromBytes(r, &r_num);
    UInt256_FromBytes(s, &s_num);

    assert(UInt256_Cmp(&r_num, &test_sign_r) == 0);
    assert(UInt256_Cmp(&s_num, &test_sign_s) == 0);

    assert(SM2_VerifyDigest(&sm2, digest, signature) == 0);

    printf("SM2 Sign Digest Test OK.\n");
}

void test_sign()
{
    uint8_t sk[SM2_SK_LENGTH] = { 0 };
    UInt256_ToBytes(&sm2_sk, sk);

    uint8_t pk[SM2_PK_MAX_LENGTH] = { 0x03 };
    UInt256_ToBytes(&sm2_pk.x, pk + 1);

    uint8_t msg[15] = "message digest";
    uint64_t msg_len = 14;
    uint8_t signature[SM2_SIGNATURE_LENGTH] = { 0 };
    uint8_t* r = signature;
    uint8_t* s = signature + SM2_SIGN_R_LENGTH;
    UInt256 r_num = { 0 };
    UInt256 s_num = { 0 };

    SM2 sm2 = { 0 };
    assert(SM2_Init(&sm2, sk, pk, 33, NULL, 0, SM2_PCMODE_RAW, &test_sign_rnd_alg) == 0);

    assert(SM2_Sign(&sm2, msg, msg_len, signature) == 0);
    UInt256_FromBytes(r, &r_num);
    UInt256_FromBytes(s, &s_num);

    assert(UInt256_Cmp(&r_num, &test_sign_r) == 0);
    assert(UInt256_Cmp(&s_num, &test_sign_s) == 0);

    assert(SM2_Verify(&sm2, msg, msg_len, signature) == 0);

    printf("SM2 Sign Test OK.\n");
}

int main()
{
    printf("========== SM2 Test ==========\n");

    test_sign_digest();
    test_sign();

    return 0;
}
