#ifdef _EXE
#include <stdio.h>
#include "sm3.h"

uint8_t digest[32] = { 0 };
SM3 sm3 = { 0 };

void test1()
{
    uint8_t ans[32] = {
        0x1a, 0xb2, 0x1d, 0x83, 0x55, 0xcf, 0xa1, 0x7f, 0x8e, 0x61, 0x19, 0x48, 0x31, 0xe8, 0x1a, 0x8f,
        0x22, 0xbe, 0xc8, 0xc7, 0x28, 0xfe, 0xfb, 0x74, 0x7e, 0xd0, 0x35, 0xeb, 0x50, 0x82, 0xaa, 0x2b
    };
    int ok = 1;

    SM3_Init(&sm3);
    SM3_Digest(&sm3, digest);
    SM3_Update(&sm3, NULL, 0);

    printf("========== SM3 Test #1 ==========\n");
    printf("Message\t:\t<Empty>\n");
    printf("Digest\t:\t");

    for (int i = 0; i < 32; i++)
    {
        printf("%02x", digest[i]);
        if (digest[i] != ans[i])
        {
            ok = 0;
        }
    }
    printf("\n");
    printf("Test #1\t:\t%s\n", ok ? "OK" : "Failed");
}

void test2()
{
    uint8_t msg[] = "abc";
    uint8_t msg2[] = "dabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd";

    uint8_t ans[32] = {
        0x66, 0xc7, 0xf0, 0xf4, 0x62, 0xee, 0xed, 0xd9, 0xd1, 0xf2, 0xd4, 0x6b, 0xdc, 0x10, 0xe4, 0xe2,
        0x41, 0x67, 0xc4, 0x87, 0x5c, 0xf2, 0xf7, 0xa2, 0x29, 0x7d, 0xa0, 0x2b, 0x8f, 0x4b, 0xa8, 0xe0
    };
    uint8_t ans2[32] = {
         0xde, 0xbe, 0x9f, 0xf9, 0x22, 0x75, 0xb8, 0xa1, 0x38, 0x60, 0x48, 0x89, 0xc1, 0x8e, 0x5a, 0x4d,
         0x6f, 0xdb, 0x70, 0xe5, 0x38, 0x7e, 0x57, 0x65, 0x29, 0x3d, 0xcb, 0xa3, 0x9c, 0x0c, 0x57, 0x32
    };

    int ok = 1;

    SM3_Init(&sm3);

    SM3_Update(&sm3, msg, 3);
    SM3_Digest(&sm3, digest);

    printf("========== SM3 Test #2 ==========\n");
    printf("Message\t:\t%s\n", msg);
    printf("Digest\t:\t");

    for (int i = 0; i < 32; i++)
    {
        printf("%02x", digest[i]);
        if (digest[i] != ans[i])
        {
            ok = 0;
        }
    }
    printf("\n");

    SM3_Update(&sm3, msg2, 61);
    SM3_Digest(&sm3, digest);

    printf("Message\t:\t%s%s\n", msg, msg2);
    printf("Digest\t:\t");

    for (int i = 0; i < 32; i++)
    {
        printf("%02x", digest[i]);
        if (digest[i] != ans2[i])
        {
            ok = 0;
        }
    }
    printf("\n");

    printf("Test #2\t:\t%s\n", ok ? "OK" : "Failed");
}

void test3()
{
    uint8_t msg[] = "123456781234567812345678123456781234567812345678123456781234";

    uint8_t ans[32] = {
         0xee, 0x80, 0x05, 0x4c, 0x07, 0x59, 0x90, 0xac, 0x9f, 0x15, 0x51, 0x52, 0x7d, 0x0f, 0xf3, 0x07, 
         0x8d, 0xb1, 0x2a, 0xd0, 0x07, 0x65, 0xa0, 0x23, 0x7d, 0xa5, 0xb7, 0xdd, 0xea, 0xfd, 0xf1, 0x2e
    };

    int ok = 1;

    SM3_Init(&sm3);

    SM3_Update(&sm3, msg, 60);
    SM3_Digest(&sm3, digest);

    printf("========== SM3 Test #3 ==========\n");
    printf("Message\t:\t%s\n", msg);
    printf("Digest\t:\t");

    for (int i = 0; i < 32; i++)
    {
        printf("%02x", digest[i]);
        if (digest[i] != ans[i])
        {
            ok = 0;
        }
    }
    printf("\n");

    printf("Test #3\t:\t%s\n", ok ? "OK" : "Failed");
}

void test4()
{
    uint8_t msg[] = "12345678123456781234567812345678123456781234567812345678123456781234";

    uint8_t ans[32] = {
          0x22, 0xa8, 0xcd, 0x20, 0x50, 0x74, 0x2d, 0xa0, 0x48, 0x8f, 0xf1, 0xa6, 0xef, 0x21, 0x96, 0x05, 
          0x2b, 0x8f, 0x01, 0xe9, 0xba, 0x03, 0x66, 0x2a, 0x71, 0xab, 0x8d, 0x28, 0x54, 0x3e, 0x47, 0x87
    };

    int ok = 1;

    SM3_Init(&sm3);

    SM3_Update(&sm3, msg, 68);
    SM3_Digest(&sm3, digest);

    printf("========== SM3 Test #4 ==========\n");
    printf("Message\t:\t%s\n", msg);
    printf("Digest\t:\t");

    for (int i = 0; i < 32; i++)
    {
        printf("%02x", digest[i]);
        if (digest[i] != ans[i])
        {
            ok = 0;
        }
    }
    printf("\n");

    printf("Test #4\t:\t%s\n", ok ? "OK" : "Failed");
}

void test5()
{
    uint8_t msg[] = "1234567812345678123456781234567812345678123456781234567812345678";
    uint8_t msg2[] = "1234567812345678123456781234567812345678123456781234567812345678";

    uint8_t ans[32] = {
        0x78, 0x83, 0xe6, 0x26, 0xd0, 0x7f, 0x17, 0x9e, 0x5a, 0x5e, 0x06, 0x44, 0x54, 0x62, 0xbd, 0x08,
        0xf0, 0x81, 0x56, 0xa8, 0xdd, 0xce, 0x5f, 0xe9, 0xe6, 0xda, 0xe4, 0xd6, 0xda, 0xd4, 0x9c, 0xf8
    };
    uint8_t ans2[32] = {
        0x16, 0xab, 0xfd, 0xd5, 0x7f, 0x52, 0x83, 0x74, 0x57, 0xd3, 0x6f, 0x7e, 0x3b, 0x5e, 0x80, 0x6e, 
        0x56, 0x8e, 0x3b, 0xda, 0x6a, 0xd9, 0x20, 0x25, 0x9f, 0xec, 0x4c, 0xeb, 0x5b, 0x38, 0x29, 0x21
    };

    int ok = 1;

    SM3_Init(&sm3);

    SM3_Update(&sm3, msg, 64);
    SM3_Digest(&sm3, digest);

    printf("========== SM3 Test #5 ==========\n");
    printf("Message\t:\t%s\n", msg);
    printf("Digest\t:\t");

    for (int i = 0; i < 32; i++)
    {
        printf("%02x", digest[i]);
        if (digest[i] != ans[i])
        {
            ok = 0;
        }
    }
    printf("\n");

    SM3_Update(&sm3, msg2, 64);
    SM3_Digest(&sm3, digest);

    printf("Message\t:\t%s%s\n", msg, msg2);
    printf("Digest\t:\t");

    for (int i = 0; i < 32; i++)
    {
        printf("%02x", digest[i]);
        if (digest[i] != ans2[i])
        {
            ok = 0;
        }
    }
    printf("\n");

    printf("Test #5\t:\t%s\n", ok ? "OK" : "Failed");
}

void test6()
{
    uint8_t msg[] = "12345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678";

    uint8_t ans[32] = {
        0x16, 0xab, 0xfd, 0xd5, 0x7f, 0x52, 0x83, 0x74, 0x57, 0xd3, 0x6f, 0x7e, 0x3b, 0x5e, 0x80, 0x6e,
        0x56, 0x8e, 0x3b, 0xda, 0x6a, 0xd9, 0x20, 0x25, 0x9f, 0xec, 0x4c, 0xeb, 0x5b, 0x38, 0x29, 0x21
    };

    int ok = 1;

    SM3_Init(&sm3);

    SM3_Update(&sm3, msg, 128);
    SM3_Digest(&sm3, digest);

    printf("========== SM3 Test #6 ==========\n");
    printf("Message\t:\t%s\n", msg);
    printf("Digest\t:\t");

    for (int i = 0; i < 32; i++)
    {
        printf("%02x", digest[i]);
        if (digest[i] != ans[i])
        {
            ok = 0;
        }
    }
    printf("\n");

    printf("Test #6\t:\t%s\n", ok ? "OK" : "Failed");
}


int main()
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();

    return 0;
}
#endif // _DEBUG
