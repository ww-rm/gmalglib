#include <stdio.h>
#include <gmalglib/bignum.h>

void test1()
{
    int ok = 1;
    uint8_t carry = 0;
    UInt256 a = { .u32 = {
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    } };

    UInt256 b = { 0x01 };

    UInt256 c = { .u32 = {
        0x9A668565, 0xA5318698, 0x6B87F21A, 0x514A0B7F,
        0xE5901167, 0x6E0F8A15, 0x252E6A48, 0x09D3E970,
    } };

    UInt256 d = { .u32 = {
        0xA1A8FFB2, 0x65BDF98A, 0x3B9E57B5, 0x859BDCE4,
        0x62C72193, 0x8863E29A, 0x714D66CA, 0xFA18288A,
    } };

    UInt256 output1 = { 0 };
    UInt512 output2 = { 0 };

    UInt256 result1 = { .u32 = {
        0x3C0F8517, 0x0AEF8023, 0xA72649D0, 0xD6E5E863,
        0x485732FA, 0xF6736CB0, 0x967BD112, 0x03EC11FA
    } };
    UInt256 result2 = { .u32 = {
        0x07427A4D, 0xC08C72F2, 0xD016659A, 0x3451D164,
        0x7D37102C, 0x1A545884, 0x4C1EFC82, 0xF0443F1A
    } };

    UInt512 result3 = { .u32 = {
        0x89705B3A, 0xC6558A85, 0x3B687F64, 0x216B743A,
        0x9BE81155, 0xEDD23855, 0xB2BB6B4A, 0x73A91477,
        0xA4F8998C, 0xFB153350, 0x658D628A, 0x5130B972,
        0xB6FFFDE2, 0xEA801163, 0xD3E1AF21, 0x0999DF63
    } };

    carry = UInt256_Add(&a, &b, &output1);
    if (carry != 0x1)
    {
        ok = 0;
    }
    for (int i = 0; i < 32; i++)
    {
        if (output1.u8[i] != 0x00)
        {
            ok = 0;
        }
    }

    carry = UInt256_Add(&c, &d, &output1);
    if (carry != 0x1)
    {
        ok = 0;
    }
    for (int i = 0; i < 32; i++)
    {
        if (output1.u8[i] != result1.u8[i])
        {
            ok = 0;
        }
    }

    carry = UInt256_Sub(&d, &c, &output1);
    if (carry != 0)
    {
        ok = 0;
    }
    for (int i = 0; i < 32; i++)
    {
        if (output1.u8[i] != result2.u8[i])
        {
            ok = 0;
        }
    }

    UInt256_Mul(&c, &d, &output2);
    for (int i = 0; i < 64; i++)
    {
        if (output2.u8[i] != result3.u8[i])
        {
            ok = 0;
        }
    }

    printf("Test #1\t:\t%s\n", ok ? "OK" : "Failed");
}

int main()
{
    printf("========== BigNum Test ==========\n");
    test1();

    return 0;
}