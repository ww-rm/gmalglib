#include <stdint.h>
#include <gmalglib/bignum.h>


#ifdef _DEBUG

#include <stdio.h>

#endif // _DEBUG


void UInt256_ToBytes(const UInt256* x, uint8_t* bytes)
{
    uint32_t i;
    uint8_t tmp[32] = { 0 };
    for (i = 0; i < 32; i++)
    {
        tmp[i] = x->u8[31 - i];
    }
    for (i = 0; i < 32; i++)
    {
        bytes[i] = tmp[i];
    }
}

void UInt256_FromBytes(const uint8_t* bytes, UInt256* x)
{
    uint32_t i;
    uint8_t tmp[32] = { 0 };
    for (i = 0; i < 32; i++)
    {
        tmp[i] = bytes[31 - i];
    }
    for (i = 0; i < 32; i++)
    {
        x->u8[i] = tmp[i];
    }
}

int UInt256_Cmp(const UInt256* x, const UInt256* y)
{
    if (x->u64[3] > y->u64[3]) return  1;
    else if (x->u64[3] < y->u64[3]) return -1;
    else if (x->u64[2] > y->u64[2]) return  1;
    else if (x->u64[2] < y->u64[2]) return -1;
    else if (x->u64[1] > y->u64[1]) return  1;
    else if (x->u64[1] < y->u64[1]) return -1;
    else if (x->u64[0] > y->u64[0]) return  1;
    else if (x->u64[0] < y->u64[0]) return -1;

    return 0;
}

int UInt256_IsZero(const UInt256* x)
{
    return x->u64[0] == 0 && x->u64[1] == 0 && x->u64[2] == 0 && x->u64[3] == 0;
}

void UInt256_SetZero(UInt256* x)
{
    x->u64[0] = x->u64[1] = x->u64[2] = x->u64[3] = 0;
}

uint8_t UInt256_Add(const UInt256* x, const UInt256* y, UInt256* z)
{
    uint32_t i = 0;
    uint64_t carry = 0;
    uint64_t tmp = 0;
    for (i = 0; i < 4; i++)
    {
        tmp = x->u64[i] + carry;
        carry = tmp < x->u64[i];
        z->u64[i] = tmp + y->u64[i];
        carry += z->u64[i] < tmp;
    }
    return (uint8_t)carry;
}

uint8_t UInt256_Sub(const UInt256* x, const UInt256* y, UInt256* z)
{
    uint32_t i = 0;
    uint64_t borrow = 0;
    uint64_t tmp = 0;
    for (i = 0; i < 4; i++)
    {
        tmp = x->u64[i] - borrow;
        borrow = tmp > x->u64[i];
        z->u64[i] = tmp - y->u64[i];
        borrow += z->u64[i] > tmp;
    }
    return (uint8_t)borrow;
}

#ifdef __SIZEOF_INT128__

    void UInt256_Mul(const UInt256* x, const UInt256* y, UInt512* z)
    {
        UInt512 _z_tmp = { 0 };
        UInt512* z_tmp = &_z_tmp;
        __uint128_t carry = 0;

        for (int i = 0; i < 4; i++)
        {
            carry = 0;
            for (int j = 0; j < 4; j++)
            {
                carry += (__uint128_t)z_tmp->u64[i + j] + (__uint128_t)x->u64[i] * (__uint128_t)y->u64[j];
                z_tmp->u64[i + j] = (uint64_t)carry;
                carry >>= 64;
            }
            z_tmp->u64[i + 4] = (uint64_t)carry;
        }

        *z = *z_tmp;
    }

    void UInt256_Sqr(const UInt256* x, UInt512* y)
    {
        UInt512 _y_tmp = { 0 };
        UInt512* y_tmp = &_y_tmp;
        __uint128_t carry = 0;
        __uint128_t table[4][4] = { 0 };
        for (int i = 0; i < 4; i++)
        {
            table[i][i] = (__uint128_t)x->u64[i] * (__uint128_t)x->u64[i];
            for (int j = i + 1; j < 4; j++)
            {
                table[i][j] = table[j][i] = (__uint128_t)x->u64[i] * (__uint128_t)x->u64[j];
            }
        }

        for (int i = 0; i < 4; i++)
        {
            carry = 0;
            for (int j = 0; j < 4; j++)
            {
                carry += (__uint128_t)y_tmp->u64[i + j] + table[i][j];
                y_tmp->u64[i + j] = (uint64_t)carry;
                carry >>= 64;
            }
            y_tmp->u64[i + 4] = (uint64_t)carry;
        }

        *y = *y_tmp;
    }

#elif defined(_MSC_VER) && defined(_WIN64)

    #include <intrin.h>
    #pragma intrinsic(_umul128, _addcarry_u64)
    void UInt256_Mul(const UInt256* x, const UInt256* y, UInt512* z)
    {
        UInt512 _z_tmp = { 0 };
        UInt512* z_tmp = &_z_tmp;
        uint64_t carry = 0;
        uint64_t high = 0;

        for (int i = 0; i < 4; i++)
        {
            carry = 0;
            for (int j = 0; j < 4; j++)
            {
                carry = _addcarry_u64(0, z_tmp->u64[i + j], carry, z_tmp->u64 + i + j);
                carry += _addcarry_u64(0, z_tmp->u64[i + j], _umul128(x->u64[i], y->u64[j], &high), z_tmp->u64 + i + j);
                carry += high;
            }
            z_tmp->u64[i + 4] = carry;
        }

        *z = *z_tmp;
    }

    void UInt256_Sqr(const UInt256* x, UInt512* y)
    {
        UInt512 _y_tmp = { 0 };
        UInt512* y_tmp = &_y_tmp;
        uint64_t carry = 0;
        uint64_t high[4][4] = { 0 };
        uint64_t table[4][4] = { 0 };
        for (int i = 0; i < 4; i++)
        {
            table[i][i] = _umul128(x->u64[i], x->u64[i], &high[i][i]);
            for (int j = i + 1; j < 4; j++)
            {
                table[i][j] = table[j][i] = _umul128(x->u64[i], x->u64[j], &high[i][j]);
                high[j][i] = high[i][j];
            }
        }

        for (int i = 0; i < 4; i++)
        {
            carry = 0;
            for (int j = 0; j < 4; j++)
            {
                carry = _addcarry_u64(0, y_tmp->u64[i + j], carry, y_tmp->u64 + i + j);
                carry += _addcarry_u64(0, y_tmp->u64[i + j], table[i][j], y_tmp->u64 + i + j);
                carry += high[i][j];
            }
            y_tmp->u64[i + 4] = carry;
        }

        *y = *y_tmp;
    }

#else

    void UInt256_Mul(const UInt256* x, const UInt256* y, UInt512* z)
    {
        UInt512 _z_tmp = { 0 };
        UInt512* z_tmp = &_z_tmp;
        uint64_t carry = 0;

        for (int i = 0; i < 8; i++)
        {
            carry = 0;
            for (int j = 0; j < 8; j++)
            {
                carry += (uint64_t)z_tmp->u32[i + j] + (uint64_t)x->u32[i] * (uint64_t)y->u32[j];
                z_tmp->u32[i + j] = (uint32_t)carry;
                carry >>= 32;
            }
            z_tmp->u32[i + 8] = (uint32_t)carry;
        }

        *z = *z_tmp;
    }

    void UInt256_Sqr(const UInt256* x, UInt512* y)
    {
        UInt512 _y_tmp = { 0 };
        UInt512* y_tmp = &_y_tmp;
        uint64_t carry = 0;
        uint64_t table[8][8] = { 0 };
        for (int i = 0; i < 8; i++)
        {
            table[i][i] = (uint64_t)x->u32[i] * (uint64_t)x->u32[i];
            for (int j = i + 1; j < 8; j++)
            {
                table[i][j] = table[j][i] = (uint64_t)x->u32[i] * (uint64_t)x->u32[j];
            }
        }

        for (int i = 0; i < 8; i++)
        {
            carry = 0;
            for (int j = 0; j < 8; j++)
            {
                carry += (uint64_t)y_tmp->u32[i + j] + table[i][j];
                y_tmp->u32[i + j] = (uint32_t)carry;
                carry >>= 32;
            }
            y_tmp->u32[i + 8] = (uint32_t)carry;
        }

        *y = *y_tmp;
    }

#endif

uint8_t UInt512_Add(const UInt512* x, const UInt512* y, UInt512* z)
{
    uint32_t i = 0;
    uint64_t carry = 0;
    uint64_t tmp = 0;
    for (i = 0; i < 8; i++)
    {
        tmp = x->u64[i] + carry;
        carry = tmp < x->u64[i];
        z->u64[i] = tmp + y->u64[i];
        carry += z->u64[i] < tmp;
    }
    return (uint8_t)carry;
}

#ifdef _DEBUG

void UInt256_Print(const UInt256* x, int sep)
{
    printf("0x");
    for (int i = 0; i < 32; i++)
    {
        if (i > 0 && i % sep == 0)
        {
            printf("_");
        }
        printf("%02X", x->u8[31 - i]);
    }
}

#endif // _DEBUG

