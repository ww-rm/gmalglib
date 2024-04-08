#include <stdint.h>
#include <gmalglib/bignum.h>

int UInt256_Cmp(const UInt256* x, const UInt256* y)
{
    uint64_t* x_parts = (uint64_t*)x;
    uint64_t* y_parts = (uint64_t*)y;

    if (x_parts[3] > y_parts[3]) return  1;
    else if (x_parts[3] < y_parts[3]) return -1;
    else if (x_parts[2] > y_parts[2]) return  1;
    else if (x_parts[2] < y_parts[2]) return -1;
    else if (x_parts[1] > y_parts[1]) return  1;
    else if (x_parts[1] < y_parts[1]) return -1;
    else if (x_parts[0] > y_parts[0]) return  1;
    else if (x_parts[0] < y_parts[0]) return -1;

    return 0;
}

uint8_t UInt256_Add(const UInt256* x, const UInt256* y, UInt256* z)
{
    uint64_t* x_parts = (uint64_t*)x;
    uint64_t* y_parts = (uint64_t*)y;
    uint64_t* z_parts = (uint64_t*)z;
    uint32_t i = 0;
    uint64_t carry = 0;
    uint64_t tmp = 0;
    for (i = 0; i < 4; i++)
    {
        tmp = x_parts[i] + carry;
        carry = tmp < x_parts[i];
        z_parts[i] = tmp + y_parts[i];
        carry += z_parts[i] < tmp;
    }
    return (uint8_t)carry;
}

uint8_t UInt256_Sub(const UInt256* x, const UInt256* y, UInt256* z)
{
    uint64_t* x_parts = (uint64_t*)x;
    uint64_t* y_parts = (uint64_t*)y;
    uint64_t* z_parts = (uint64_t*)z;
    uint32_t i = 0;
    uint64_t borrow = 0;
    uint64_t tmp = 0;
    for (i = 0; i < 4; i++)
    {
        tmp = x_parts[i] - borrow;
        borrow = tmp > x_parts[i];
        z_parts[i] = tmp - y_parts[i];
        borrow += z_parts[i] > tmp;
    }
    return (uint8_t)borrow;
}

void UInt256_Mul(const UInt256* x, const UInt256* y, UInt512* z)
{
    uint32_t* x_parts = (uint32_t*)x;
    uint32_t* y_parts = (uint32_t*)y;
    uint32_t* z_parts = (uint32_t*)z;

    UInt512 z_tmp = { 0 };
    uint32_t* z_tmp_parts = (uint32_t*)&z_tmp;
    uint64_t carry = 0;

    uint32_t i;
    uint32_t j;

    for (i = 0; i < 8; i++)
    {
        carry = 0;
        for (j = 0; j < 8; j++)
        {
            carry += (uint64_t)z_tmp_parts[i + j] + (uint64_t)x_parts[i] * (uint64_t)y_parts[j];
            z_tmp_parts[i + j] = (uint32_t)carry;
            carry >>= 32;
        }
        z_tmp_parts[i + 8] = (uint32_t)carry;
    }

    for (i = 0; i < 16; i++)
    {
        z_parts[i] = z_tmp_parts[i];
    }
}

uint8_t UInt512_Add(const UInt512* x, const UInt512* y, UInt512* z)
{
    uint64_t* x_parts = (uint64_t*)x;
    uint64_t* y_parts = (uint64_t*)y;
    uint64_t* z_parts = (uint64_t*)z;
    uint32_t i = 0;
    uint64_t carry = 0;
    uint64_t tmp = 0;
    for (i = 0; i < 8; i++)
    {
        tmp = x_parts[i] + carry;
        carry = tmp < x_parts[i];
        z_parts[i] = tmp + y_parts[i];
        carry += z_parts[i] < tmp;
    }
    return (uint8_t)carry;
}
