#include <stdio.h>
#include <gmalglib/sm4.h>

SM4 sm4 = { 0 };
uint8_t key[SM4_KEY_LENGTH] = { 
    0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 
    0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10 
};
uint8_t block[SM4_BLOCK_LENGTH] = { 0 };

void test1()
{
    uint8_t plain[SM4_BLOCK_LENGTH] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
        0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10
    };
    uint8_t cipher[SM4_BLOCK_LENGTH] = { 
        0x68, 0x1e, 0xdf, 0x34, 0xd2, 0x06, 0x96, 0x5e, 
        0x86, 0xb3, 0xe9, 0x4f, 0x53, 0x6e, 0x42, 0x46
    };
    int ok = 1;

    SM4_Init(&sm4, key);
    SM4_Encrypt(&sm4, plain, block);

    printf("========== SM4 Test #1 ==========\n");
    for (int i = 0; i < SM4_BLOCK_LENGTH; i++)
    {
        if (block[i] != cipher[i])
        {
            ok = 0;
        }
    }

    SM4_Decrypt(&sm4, cipher, block);
    for (int i = 0; i < SM4_BLOCK_LENGTH; i++)
    {
        if (block[i] != plain[i])
        {
            ok = 0;
        }
    }

    printf("Test #1\t:\t%s\n", ok ? "OK" : "Failed");
}

void test2()
{
    uint8_t plain[SM4_BLOCK_LENGTH] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
        0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10
    };
    uint8_t cipher[SM4_BLOCK_LENGTH] = {
        0x59, 0x52, 0x98, 0xc7, 0xc6, 0xfd, 0x27, 0x1f, 
        0x04, 0x02, 0xf8, 0x04, 0xc3, 0x3d, 0x3f, 0x66
    };
    int ok = 1;

    SM4_Init(&sm4, key);
    SM4_Encrypt(&sm4, plain, block);
    for (int i = 0; i < 999999; i++)
    {
        SM4_Encrypt(&sm4, block, block);
    }

    printf("========== SM4 Test #2 ==========\n");
    for (int i = 0; i < SM4_BLOCK_LENGTH; i++)
    {
        if (block[i] != cipher[i])
        {
            ok = 0;
        }
    }

    SM4_Decrypt(&sm4, cipher, block);
    for (int i = 0; i < 999999; i++)
    {
        SM4_Decrypt(&sm4, block, block);
    }

    for (int i = 0; i < SM4_BLOCK_LENGTH; i++)
    {
        if (block[i] != plain[i])
        {
            ok = 0;
        }
    }

    printf("Test #2\t:\t%s\n", ok ? "OK" : "Failed");
}

int main()
{
    test1();
    test2();

    return 0;
}