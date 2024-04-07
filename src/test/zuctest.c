#include <stdio.h>
#include <gmalglib/zuc.h>

void test1()
{
    ZUC zuc = { 0 };
    uint8_t key[ZUC_KEY_LENGTH] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0 };
    uint8_t iv[ZUC_KEY_LENGTH] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0 };
    uint32_t word = 0;

    ZUC_Init(&zuc, key, iv);

    int ok = 1;
    printf("========== ZUC Test #1 ==========\n");

    word = ZUC_generate(&zuc, NULL);
    if (word != 0x27bede74)
    {
        ok = 0;
    }

    word = ZUC_generate(&zuc, NULL);
    if (word != 0x018082da)
    {
        ok = 0;
    }

    printf("Test #1\t:\t%s\n", ok ? "OK" : "Failed");
}

void test2()
{
    ZUC zuc = { 0 };
    uint8_t key[ZUC_KEY_LENGTH] = { 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };
    uint8_t iv[ZUC_KEY_LENGTH] = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };
    uint32_t word = 0;

    ZUC_Init(&zuc, key, iv);

    int ok = 1;
    printf("========== ZUC Test #2 ==========\n");

    word = ZUC_generate(&zuc, NULL);
    if (word != 0x0657cfa0)
    {
        ok = 0;
    }

    word = ZUC_generate(&zuc, NULL);
    if (word != 0x7096398b)
    {
        ok = 0;
    }

    printf("Test #2\t:\t%s\n", ok ? "OK" : "Failed");
}

void test3()
{
    ZUC zuc = { 0 };
    uint8_t key[ZUC_KEY_LENGTH] = {
        0x3D, 0x4C, 0x4B, 0xE9, 0x6A, 0x82, 0xFD, 0xAE, 
        0xB5, 0x8F, 0x64, 0x1D, 0xB1, 0x7B, 0x45, 0x5B
    };
    uint8_t iv[ZUC_KEY_LENGTH] = {
        0x84, 0x31, 0x9A, 0xA8, 0xDE, 0x69, 0x15, 0xCA, 
        0x1F, 0x6B, 0xDA, 0x6B, 0xFB, 0xD8, 0xC7, 0x66
    };
    uint32_t word = 0;

    ZUC_Init(&zuc, key, iv);

    int ok = 1;
    printf("========== ZUC Test #3 ==========\n");

    word = ZUC_generate(&zuc, NULL);
    if (word != 0x14f1c272)
    {
        ok = 0;
    }

    word = ZUC_generate(&zuc, NULL);
    if (word != 0x3279c419)
    {
        ok = 0;
    }

    printf("Test #3\t:\t%s\n", ok ? "OK" : "Failed");
}

int main()
{
    test1();
    test2();
    test3();

    return 0;
}
